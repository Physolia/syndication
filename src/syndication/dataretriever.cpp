/*
 * dataretriever.cpp
 *
 * Copyright (c) 2001, 2002, 2003 Frerich Raabe <raabe@kde.org>
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. For licensing and distribution details, check the
 * accompanying file 'COPYING'.
 */

#include "dataretriever.h"
#include "global.h"
#include "syndication_version.h"

#include <kio/job.h>

#include <kprocess.h>
#include <QUrl>

#include <QtCore/QBuffer>
#include <QtCore/QTimer>

namespace Syndication
{

DataRetriever::DataRetriever()
{
}

DataRetriever::~DataRetriever()
{
}

struct FileRetriever::FileRetrieverPrivate {
    FileRetrieverPrivate()
        : buffer(NULL),
          lastError(0), job(NULL)
    {
    }

    ~FileRetrieverPrivate()
    {
        delete buffer;
    }

    QBuffer *buffer;
    int lastError;
    KIO::TransferJob *job;
};

FileRetriever::FileRetriever()
    : d(new FileRetrieverPrivate)
{
}

FileRetriever::~FileRetriever()
{
    delete d;
}

bool FileRetriever::m_useCache = true;
QString FileRetriever::m_userAgent = QStringLiteral("Syndication %1").arg(QStringLiteral(SYNDICATION_VERSION_STRING));

void FileRetriever::setUserAgent(const QString &userAgent)
{
    m_userAgent = userAgent;
}

void FileRetriever::setUseCache(bool enabled)
{
    m_useCache = enabled;
}

void FileRetriever::retrieveData(const QUrl &url)
{
    if (d->buffer) {
        return;
    }

    d->buffer = new QBuffer;
    d->buffer->open(QIODevice::WriteOnly);

    QUrl u = url;

    if (u.scheme() == QLatin1String("feed")) {
        u.setScheme(QStringLiteral("http"));
    }

    d->job = KIO::get(u, KIO::NoReload, KIO::HideProgressInfo);

    d->job->addMetaData(QStringLiteral("UserAgent"), m_userAgent);
    d->job->addMetaData(QStringLiteral("cache"), m_useCache ? QStringLiteral("refresh") : QStringLiteral("reload"));

    QTimer::singleShot(1000 * 90, this, SLOT(slotTimeout()));

    connect(d->job, &KIO::TransferJob::data, this, &FileRetriever::slotData);
    connect(d->job, &KIO::TransferJob::result, this, &FileRetriever::slotResult);
    connect(d->job, &KIO::TransferJob::permanentRedirection, this, &FileRetriever::slotPermanentRedirection);
}

void FileRetriever::slotTimeout()
{
    abort();

    delete d->buffer;
    d->buffer = NULL;

    d->lastError = KIO::ERR_SERVER_TIMEOUT;

    emit dataRetrieved(QByteArray(), false);
}

int FileRetriever::errorCode() const
{
    return d->lastError;
}

void FileRetriever::slotData(KIO::Job *, const QByteArray &data)
{
    d->buffer->write(data.data(), data.size());
}

void FileRetriever::slotResult(KJob *job)
{
    QByteArray data = d->buffer->buffer();
    data.detach();

    delete d->buffer;
    d->buffer = NULL;

    d->lastError = job->error();
    emit dataRetrieved(data, d->lastError == 0);
}

void FileRetriever::slotPermanentRedirection(KIO::Job *, const QUrl &,
        const QUrl &newUrl)
{
    emit permanentRedirection(newUrl);
}

void FileRetriever::abort()
{
    if (d->job) {
        d->job->kill();
        d->job = NULL;
    }
}

struct OutputRetriever::OutputRetrieverPrivate {
    OutputRetrieverPrivate() : process(0L), buffer(0L), lastError(0)
    {
    }

    ~OutputRetrieverPrivate()
    {
        delete process;
        delete buffer;
    }

    KProcess *process;
    QBuffer *buffer;
    int lastError;
};

OutputRetriever::OutputRetriever() : d(new OutputRetrieverPrivate)
{
}

OutputRetriever::~OutputRetriever()
{
    delete d;
}

void OutputRetriever::retrieveData(const QUrl &url)
{
    // Ignore subsequent calls if we didn't finish the previous job yet.
    if (d->buffer || d->process) {
        return;
    }

    d->buffer = new QBuffer;
    d->buffer->open(QIODevice::WriteOnly);

    d->process = new KProcess();
    connect(d->process, SIGNAL(finished(int,QProcess::ExitStatus)),
            SLOT(slotFinished(int,QProcess::ExitStatus)));
    d->process->setShellCommand(url.path());
    d->process->start();
}

int OutputRetriever::errorCode() const
{
    return d->lastError;
}

void OutputRetriever::slotFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    if (!d->process->exitCode()) {
        d->lastError = d->process->exitCode();
    }

    QByteArray data = d->process->readAllStandardOutput();

    delete d->buffer;
    d->buffer = NULL;

    int code = d->process->exitCode();

    delete d->process;
    d->process = NULL;

    emit dataRetrieved(data, exitStatus == QProcess::NormalExit && code == 0);
}

} // namespace Syndication


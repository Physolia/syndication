/*
 * This file is part of the syndication library
 *
 * SPDX-FileCopyrightText: 2006 Frank Osterfeld <osterfeld@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 *
 */

#include "item.h"
#include "category.h"
#include "enclosure.h"
#include "person.h"
#include "tools.h"

#include <QList>
#include <QString>

namespace Syndication
{

Item::~Item()
{
}

QString Item::debugInfo() const
{
    QString info = QLatin1String("# Item begin ######################\n");

    QString did = id();
    if (!did.isNull()) {
        info += QLatin1String("id: #") + did + QLatin1String("#\n");
    }

    QString dtitle = title();
    if (!dtitle.isNull()) {
        info += QLatin1String("title: #") + dtitle + QLatin1String("#\n");
    }

    QString dlink = link();
    if (!dlink.isNull()) {
        info += QLatin1String("link: #") + dlink + QLatin1String("#\n");
    }

    QString ddescription = description();
    if (!ddescription.isNull()) {
        info += QLatin1String("description: #") + ddescription + QLatin1String("#\n");
    }

    QString dcontent = content();
    if (!dcontent.isNull()) {
        info += QLatin1String("content: #") + dcontent + QLatin1String("#\n");
    }

    QString pubdate = dateTimeToString(datePublished());
    if (!pubdate.isNull()) {
        info += QLatin1String("datePublished: #") + pubdate + QLatin1String("#\n");
    }

    QString update = dateTimeToString(dateUpdated());
    if (!update.isNull()) {
        info += QLatin1String("dateUpdated: #") + update + QLatin1String("#\n");
    }

    QString dlanguage = language();
    if (!dlanguage.isNull()) {
        info += QLatin1String("language: #") + dlanguage + QLatin1String("#\n");
    }

    QList<PersonPtr> dauthors = authors();
    QList<PersonPtr>::ConstIterator itp = dauthors.constBegin();
    QList<PersonPtr>::ConstIterator endp = dauthors.constEnd();

    for (; itp != endp; ++itp) {
        info += (*itp)->debugInfo();
    }

    QList<CategoryPtr> dcategories = categories();
    QList<CategoryPtr>::ConstIterator itc = dcategories.constBegin();
    QList<CategoryPtr>::ConstIterator endc = dcategories.constEnd();

    for (; itc != endc; ++itc) {
        info += (*itc)->debugInfo();
    }

    QList<EnclosurePtr> denclosures = enclosures();
    QList<EnclosurePtr>::ConstIterator ite = denclosures.constBegin();
    QList<EnclosurePtr>::ConstIterator ende = denclosures.constEnd();

    for (; ite != ende; ++ite) {
        info += (*ite)->debugInfo();
    }

    int dcommentsCount = commentsCount();
    if (dcommentsCount != -1) {
        info += QLatin1String("commentsCount: #") + QString::number(dcommentsCount) + QLatin1String("#\n");
    }

    QString dcommentsLink = commentsLink();
    if (!dcommentsLink.isNull()) {
        info += QLatin1String("commentsLink: #") + dcommentsLink + QLatin1String("#\n");
    }

    QString dcommentsFeed = commentsFeed();
    if (!dcommentsFeed.isNull()) {
        info += QLatin1String("commentsFeed: #") + dcommentsFeed + QLatin1String("#\n");
    }

    QString dcommentPostUri = commentPostUri();
    if (!dcommentPostUri.isNull()) {
        info += QLatin1String("commentPostUri: #") + dcommentPostUri + QLatin1String("#\n");
    }

    info += QLatin1String("# Item end ########################\n");

    return info;
}

} // namespace Syndication

/*
 * This file is part of the syndication library
 *
 * Copyright (C) 2006 Frank Osterfeld <frank.osterfeld@kdemail.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "property.h"
#include "statement.h"
#include "syndicationinfo.h"
#include "syndicationvocab.h"

#include <tools.h>

#include <QString>

namespace Syndication {
namespace RDF {
    
    SyndicationInfo::SyndicationInfo(ResourcePtr resource) : ResourceWrapper(resource)
{
}

SyndicationInfo::~SyndicationInfo()
{
}

SyndicationInfo::Period SyndicationInfo::updatePeriod() const
{
    return stringToPeriod(resource()->property(SyndicationVocab::self()->updatePeriod())->asString());
}

int SyndicationInfo::updateFrequency() const
{
    QString freqStr =  resource()->property(SyndicationVocab::self()->updateFrequency())->asString();
    
    if (freqStr.isEmpty())
        return 1; // 1 is default
    
    bool ok = false;
    int freq = freqStr.toInt(&ok);
    
    if (ok)
        return freq;
    else
        return 1; // 1 is default
}

time_t SyndicationInfo::updateBase() const
{
    QString str =  resource()->property(SyndicationVocab::self()->updateBase())->asString();
    
    return parseDate(str, ISODate);
}

QString SyndicationInfo::debugInfo() const
{
    QString info;
    if (updatePeriod() != Daily)
        info += QString("syn:updatePeriod: #%1#\n").arg(periodToString(updatePeriod()));
    info += QString("syn:updateFrequency: #%1#\n").arg(QString::number(updateFrequency()));
    
    QString dbase = dateTimeToString(updateBase());
    if (!dbase.isNull())
        info += QString("syn:updateBase: #%1#\n").arg(dbase);

    return info;
}

QString SyndicationInfo::periodToString(Period period)
{
    switch (period)
    {
        case Daily:
            return QString::fromUtf8("daily");
        case Hourly:
            return QString::fromUtf8("hourly");
        case Monthly:
            return QString::fromUtf8("monthly");
        case Weekly:
            return QString::fromUtf8("weekly");
        case Yearly:
            return QString::fromUtf8("yearly");
            default: // should never happen
            return QString();
    }
}

SyndicationInfo::Period SyndicationInfo::stringToPeriod(const QString& str)
{
    if (str.isEmpty())
        return Daily; // default is "daily"
    
    if (str == QString::fromUtf8("hourly"))
        return Hourly;
    if (str == QString::fromUtf8("monthly"))
        return Monthly;
    if (str == QString::fromUtf8("weekly"))
        return Weekly;
    if (str == QString::fromUtf8("yearly"))
        return Yearly;

    return Daily;  // default is "daily"
}

} // namespace RDF
} // namespace Syndication

/*
 * This file is part of the syndication library
 *
 * Copyright (C) 2006 Frank Osterfeld <osterfeld@kde.org>
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

#ifndef SYNDICATION_MAPPER_IMAGERDFIMPL_H
#define SYNDICATION_MAPPER_IMAGERDFIMPL_H

#include <QString>
#include <image.h>
#include <rdf/image.h>

namespace Syndication
{

class ImageRDFImpl;
typedef QSharedPointer<ImageRDFImpl> ImageRDFImplPtr;

/**
 *
 * @internal
 * @author Frank Osterfeld
 */
class ImageRDFImpl : public Syndication::Image
{
public:

    explicit ImageRDFImpl(const Syndication::RDF::Image &image);

    bool isNull() const Q_DECL_OVERRIDE;

    QString url() const Q_DECL_OVERRIDE;

    QString title() const Q_DECL_OVERRIDE;

    QString link() const Q_DECL_OVERRIDE;

    QString description() const Q_DECL_OVERRIDE;

    uint width() const Q_DECL_OVERRIDE;

    uint height() const Q_DECL_OVERRIDE;

private:
    Syndication::RDF::Image m_image;
};

} // namespace Syndication

#endif // SYNDICATION_MAPPER_IMAGERDFIMPL_H

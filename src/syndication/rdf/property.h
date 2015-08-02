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

#ifndef SYNDICATION_RDF_PROPERTY_H
#define SYNDICATION_RDF_PROPERTY_H

#include <syndication/rdf/resource.h>

class QString;

namespace Syndication
{
namespace RDF
{

//@cond PRIVATE
class Property;
typedef QSharedPointer<Property> PropertyPtr;
//@endcond

/**
 * a property is node type that represents properties of things,
 * like "name" is a property of a person, or "color" is a property of e.g.
 * a car. Properties can be used as predicates in statements.
 *
 * @author Frank Osterfeld
 */
class SYNDICATION_EXPORT Property : public Resource
{
public:

    /**
     * creates a null property
     */
    Property();

    /**
     * creates a property with a given URI
     *
     * @param uri the URI of the property
     */
    explicit Property(const QString &uri);

    /**
     * destructor
     */
    virtual ~Property();

    /**
     * Used by visitors for double dispatch. See NodeVisitor
     * for more information.
     * @param visitor the visitor calling the method
     * @param ptr a shared pointer object for this node
     */
    void accept(NodeVisitor *visitor, NodePtr ptr) Q_DECL_OVERRIDE;

    /**
     * returns true for properties
     */
    bool isProperty() const Q_DECL_OVERRIDE;

    /**
     * creates a copy of the property object
     */
    Property *clone() const Q_DECL_OVERRIDE;
};

} // namespace RDF
} // namespace Syndication

#endif // SYNDICATION_RDF_PROPERTY_H

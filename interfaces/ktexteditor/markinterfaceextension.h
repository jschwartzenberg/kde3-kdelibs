/* This file is part of the KDE project
   Copyright (C) 2002 Joseph Wenninger <jowenn@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef __ktexteditor_markinterface_extension_h__
#define __ktexteditor_markinterface_extension_h__

#include <qptrlist.h>
#include <qpixmap.h>
#include <qstring.h>
#include "markinterface.h"

class QCString;

namespace KTextEditor {


/**
*  This is an interface for custom extensions of the MarkInterface.
*  It allows the developer to set pixmaps for custom marks and to indicate which
*  marks are settable by the user, for example (in kate), as actions in the iconborder's popup menu.
*/
class KTEXTEDITOR_EXPORT MarkInterfaceExtension {
    friend class PrivateMarkInterfaceExtension;

public:
    MarkInterfaceExtension();
    virtual ~MarkInterfaceExtension();

    unsigned int markInterfaceExtensionNumber() const;

protected:
    void setMarkInterfaceExtensionDCOPSuffix(const QCString &suffix);

public:
    virtual void setPixmap(MarkInterface::MarkTypes, const QPixmap &) = 0;
    virtual void setDescription(MarkInterface::MarkTypes, const QString &) = 0;
    virtual void setMarksUserChangable(uint markMask) = 0;

    enum MarkChangeAction
    {
        MarkAdded = 0,
        MarkRemoved = 1
    };
    //
    // slots !!!
    //
public:
    //
    // signals !!!
    //
public:
    virtual void markChanged(KTextEditor::Mark mark, KTextEditor::MarkInterfaceExtension::MarkChangeAction action) = 0;

private:
    class PrivateMarkInterfaceExtension *d;
    static unsigned int globalMarkInterfaceExtensionNumber;
    unsigned int myMarkInterfaceExtensionNumber;
};

KTEXTEDITOR_EXPORT MarkInterfaceExtension *markInterfaceExtension(class Document *doc);
}

#endif

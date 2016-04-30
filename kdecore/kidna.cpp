/*
    This file is part of the KDE libraries

    Copyright (c) 2003 Waldo Bastian <bastian@kde.org>

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

#include "kidna.h"

#include "kresolver.h"
#include <kdebug.h>

using namespace KNetwork;

QCString KIDNA::toAsciiCString(const QString &idna)
{
    return KResolver::domainToAscii(idna);
}

QString KIDNA::toAscii(const QString &idna)
{
    if(idna.length() && (idna[0] == "."))
    {
        QString host = QString::fromLatin1(toAsciiCString(idna.mid(1)));
        if(host.isEmpty())
            return QString::null; // Error
        return idna[0] + host;
    }
    return QString::fromLatin1(toAsciiCString(idna));
}

QString KIDNA::toUnicode(const QString &idna)
{
    if(idna.length() && (idna[0] == "."))
        return idna[0] + KResolver::domainToUnicode(idna.mid(1));
    return KResolver::domainToUnicode(idna);
}

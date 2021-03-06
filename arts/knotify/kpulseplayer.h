/************************************************************************\
 *
 *  (c) 2016 by Serghei Amelian <serghei.amelian@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
\************************************************************************/
#ifndef _KPULSEPLAYER_H_
#define _KPULSEPLAYER_H_

#include <qobject.h>
#include <qcstring.h>


class KPulsePlayerPrivate;


class KPulsePlayer : public QObject {

    Q_OBJECT
    friend class KPulsePlayerPrivate;

public:
    KPulsePlayer();
    ~KPulsePlayer();

    void play(const QCString &file);

signals:
    void finished();

private:
    KPulsePlayerPrivate *d;
};


#endif // _KPULSEPLAYER_H_

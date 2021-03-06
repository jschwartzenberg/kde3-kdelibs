/*  This file is part of the KDE libraries
 *  Copyright (C) 1999 Waldo Bastian <bastian@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef __ksycoca_h__
#define __ksycoca_h__

#include <dcopobject.h>
#include <qobject.h>
#include <qstringlist.h>
#include "ksycocatype.h"
#include <kdelibs_export.h>

class QDataStream;
class KSycocaPrivate;
class KSycocaFactory;
class KSycocaFactoryList;

/*
 * Sycoca file version number.
 * If the existing file is outdated, it will not get read
 * but instead we'll ask kded to regenerate a new one...
*/
#define KSYCOCA_VERSION 94

/**
 * @internal
 * Read-only SYstem COnfiguration CAche
 */
class KDECORE_EXPORT KSycoca : public QObject, public DCOPObject {
    Q_OBJECT
    K_DCOP

protected:
    /**
     * @internal
     * Building database
     */
    KSycoca(bool /* buildDatabase */);

public:
    /**
     * Read-only database
     */
    KSycoca();

    /**
     * Get or create the only instance of KSycoca (read-only)
     */
    static KSycoca *self();

    virtual ~KSycoca();

    static int version();

    /**
     * @internal - called by factories in read-only mode
     * This is how factories get a stream to an entry
     */
    QDataStream *findEntry(int offset, KSycocaType &type);
    /**
     * @internal - called by factories in read-only mode
     */
    QDataStream *findFactory(KSycocaFactoryId id);
    /**
     * @internal - returns kfsstnd stored inside database
     */
    QString kfsstnd_prefixes();
    /**
     * @internal - returns language stored inside database
     */
    QString language();

    /**
     * @internal - returns timestamp of database
     *
     * The database contains all changes made _before_ this time and
     * _might_ contain changes made after that.
     */
    Q_UINT32 timeStamp();

    /**
     * @internal - returns update signature of database
     *
     * Signature that keeps track of changes to
     * $KDEDIR/share/services/update_ksycoca
     *
     * Touching this file causes the database to be recreated
     * from scratch.
     */
    Q_UINT32 updateSignature();

    /**
     * @internal - returns all directories with information
     * stored inside sycoca.
     */
    QStringList allResourceDirs();

    /**
     * @internal - add a factory
     */
    void addFactory(KSycocaFactory *);

    /**
     * @internal
     * @return true if building (i.e. if a KBuildSycoca);
     */
    virtual bool isBuilding()
    {
        return false;
    }

    /**
     * @internal - disables launching of kbuildsycoca
     */
    void disableAutoRebuild();

    /**
     * Determine relative path for a .desktop file from a full path and a resource name
     */
    static QString determineRelativePath(const QString &_fullpath, const char *_resource);

    /**
     * When you receive a "databaseChanged" signal, you can query here if
     * a change has occurred in a specific resource type.
     * @see KStandardDirs for the various resource types.
     */
    static bool isChanged(const char *type);

    /**
     * A read error occurs.
     */
    static void flagError();

    /**
     * Returns read error status and clears flag.
     */
    static bool readError();

    k_dcop :
        /**
         * internal function for receiving kded/kbuildsycoca's signal, when the sycoca file changes
         */
        void
        notifyDatabaseChanged(const QStringList &);

signals:
    /**
         * Connect to this to get notified when the database changes
         * (Usually apps showing icons do a 'refresh' to take into account the new mimetypes)
         */
    void databaseChanged();

protected:
    bool checkVersion(bool abortOnError = true);
    bool openDatabase(bool openDummyIfNotFound = true);
    void closeDatabase();
    KSycocaFactoryList *m_lstFactories;
    QDataStream *m_str;
    bool bNoDatabase;
    size_t m_sycoca_size;
    const char *m_sycoca_mmap;
    Q_UINT32 m_timeStamp;

public:
    static KSycoca *_self; // Internal use only.

protected:
    virtual void virtual_hook(int id, void *data);

private:
    KSycocaPrivate *d;
};

#endif

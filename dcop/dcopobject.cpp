/*****************************************************************

Copyright (c) 1999,2000 Preston Brown <pbrown@kde.org>
Copyright (c) 1999 Matthias Ettrich <ettrich@kde.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/
#include <map>

#include <dcopobject.h>
#include <dcopclient.h>

std::map< QCString, DCOPObject * > *kde_dcopObjMap = nullptr;

static inline std::map< QCString, DCOPObject * > *objMap()
{
    if(!kde_dcopObjMap)
        kde_dcopObjMap = new std::map< QCString, DCOPObject * >;
    return kde_dcopObjMap;
}

class DCOPObject::DCOPObjectPrivate {
public:
    DCOPObjectPrivate()
    {
        m_signalConnections = 0;
        m_dcopClient = 0;
    }

    unsigned int m_signalConnections;
    DCOPClient *m_dcopClient;
};

DCOPObject::DCOPObject()
{
    d = new DCOPObjectPrivate;
    ident.sprintf("%p", (void *)this);
    objMap()->emplace(ident, this);
}

DCOPObject::DCOPObject(QObject *obj)
{
    d = new DCOPObjectPrivate;
    QObject *currentObj = obj;
    while(currentObj != 0L)
    {
        ident.prepend(currentObj->objectName());
        ident.prepend("/");
        currentObj = currentObj->parent();
    }
    if(ident[0] == '/')
        ident = ident.mid(1);

    objMap()->emplace(ident, this);
}

DCOPObject::DCOPObject(const QCString &_objId) : ident(_objId)
{
    d = new DCOPObjectPrivate;
    if(ident.isEmpty())
        ident.sprintf("%p", (void *)this);
    objMap()->emplace(ident, this);
}

DCOPObject::~DCOPObject()
{
    DCOPClient *client = DCOPClient::mainClient();
    if(d->m_signalConnections > 0 && client)
        client->disconnectDCOPSignal(0, 0, 0, objId(), 0);

    objMap()->erase(ident);
    delete d;
}

DCOPClient *DCOPObject::callingDcopClient()
{
    return d->m_dcopClient;
}

void DCOPObject::setCallingDcopClient(DCOPClient *client)
{
    d->m_dcopClient = client;
}

bool DCOPObject::setObjId(const QCString &objId)
{
    if(objMap()->find(objId) != objMap()->end())
        return false;

    DCOPClient *client = DCOPClient::mainClient();
    if(d->m_signalConnections > 0 && client)
        client->disconnectDCOPSignal(0, 0, 0, ident, 0);

    objMap()->erase(ident);
    ident = objId;
    objMap()->emplace(ident, this);
    return true;
}

QCString DCOPObject::objId() const
{
    return ident;
}

bool DCOPObject::hasObject(const QCString &_objId)
{
    if(objMap()->count(_objId))
        return true;
    else
        return false;
}

DCOPObject *DCOPObject::find(const QCString &_objId)
{
    const auto it = objMap()->find(_objId);

    if(it != objMap()->end())
        return it->second;

    return nullptr;
}

QValueList< DCOPObject* > DCOPObject::match(const QCString &partialId)
{
    QValueList< DCOPObject* > mlist;

    for(const auto it : *objMap())
        if(it.first.left(partialId.length()) == partialId) // found it?
            mlist.append(it.second);

    return mlist;
}


QCString DCOPObject::objectName(QObject *obj)
{
    if(obj == 0)
        return QCString();

    QCString identity;

    QObject *currentObj = obj;
    while(currentObj != 0)
    {
        identity.prepend(currentObj->objectName());
        identity.prepend("/");
        currentObj = currentObj->parent();
    }
    if(identity[0] == '/')
        identity = identity.mid(1);

    return identity;
}

bool DCOPObject::process(const QCString &fun, const QByteArray &data, QCString &replyType, QByteArray &replyData)
{
    if(fun == "interfaces()")
    {
        replyType = "KStringList";
        QDataStream reply(&replyData, IO_WriteOnly);
        reply << interfaces();
        return true;
    }
    else if(fun == "functions()")
    {
        replyType = "KStringList";
        QDataStream reply(&replyData, IO_WriteOnly);
        reply << functions();
        return true;
    }
    return processDynamic(fun, data, replyType, replyData);
}

bool DCOPObject::processDynamic(const QCString &, const QByteArray &, QCString &, QByteArray &)
{
    return false;
}
KStringList DCOPObject::interfacesDynamic()
{
    KStringList result;
    return result;
}

KStringList DCOPObject::functionsDynamic()
{
    KStringList result;
    return result;
}
KStringList DCOPObject::interfaces()
{
    KStringList result = interfacesDynamic();
    result << "DCOPObject";
    return result;
}

KStringList DCOPObject::functions()
{
    KStringList result = functionsDynamic();
    result.prepend("KStringList functions()");
    result.prepend("KStringList interfaces()");
    return result;
}

void DCOPObject::emitDCOPSignal(const QCString &signal, const QByteArray &data)
{
    DCOPClient *client = DCOPClient::mainClient();
    if(client)
        client->emitDCOPSignal(objId(), signal, data);
}

bool DCOPObject::connectDCOPSignal(const QCString &sender, const QCString &senderObj, const QCString &signal, const QCString &slot, bool Volatile)
{
    DCOPClient *client = DCOPClient::mainClient();

    if(!client)
        return false;

    d->m_signalConnections++;
    return client->connectDCOPSignal(sender, senderObj, signal, objId(), slot, Volatile);
}

bool DCOPObject::disconnectDCOPSignal(const QCString &sender, const QCString &senderObj, const QCString &signal, const QCString &slot)
{
    DCOPClient *client = DCOPClient::mainClient();

    if(!client)
        return false;

    d->m_signalConnections--;
    return client->disconnectDCOPSignal(sender, senderObj, signal, objId(), slot);
}


QValueList< DCOPObjectProxy* > *DCOPObjectProxy::proxies = 0;

DCOPObjectProxy::DCOPObjectProxy()
{
    if(!proxies)
        proxies = new QValueList< DCOPObjectProxy* >;
    proxies->append(this);
}

DCOPObjectProxy::DCOPObjectProxy(DCOPClient *)
{
    if(!proxies)
        proxies = new QValueList< DCOPObjectProxy* >;
    proxies->append(this);
}

DCOPObjectProxy::~DCOPObjectProxy()
{
    if(proxies)
        proxies->removeOne(this);
}

bool DCOPObjectProxy::process(const QCString & /*obj*/, const QCString & /*fun*/, const QByteArray & /*data*/, QCString & /*replyType*/,
                              QByteArray & /*replyData*/)
{
    return false;
}

void DCOPObject::virtual_hook(int, void *)
{ /*BASE::virtual_hook( id, data );*/
}

void DCOPObjectProxy::virtual_hook(int, void *)
{ /*BASE::virtual_hook( id, data );*/
}

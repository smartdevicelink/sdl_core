#include "buttonsadaptor.h"

#include <QDBusContext>

#include <qqml.h>

ButtonsAdaptor::ButtonsAdaptor(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{

}

void ButtonsAdaptor::setButtonsApi(QQuickItem *api)
{
    buttonsApi = api;

    //connect(api, SIGNAL(notify(QString)), this, SIGNAL(Notify(QString)));
}

QList<ButtonCapabilities> ButtonsAdaptor::GetCapabilities(OptionalArgument<PresetBankCapabilities> &pbc)
{
    QVariant arg1;
    QDBusContext* ctx = dynamic_cast<QDBusContext*>(parent());
    QList<ButtonCapabilities> rc;

    QVariant arg2bool = QVariant::fromValue(false);
    QVariantMap arg2map;
    arg2map.insert("testItem", arg2bool);


    if (QMetaObject::invokeMethod(buttonsApi, "getCapabilities", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QVariant, (arg1)),
                                  Q_ARG(QVariant, (arg2map))))
    {
        if (arg1.type() != QVariant::Map)
        {
            if (ctx != NULL) {
                ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 1");
            }
        }

        QMap<QString, QVariant> map1 = arg1.toMap();

        if (!map1.contains("buttonCapabilities") || map1["buttonCapabilities"].type() != QVariant::List)
        {
            if (ctx != NULL) {
                ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 2");
            }
        }

        QList<QVariant> list1 = map1["buttonCapabilities"].toList();

        for (int i = 0; i < list1.length(); ++i)
        {
            if(list1[i].type() != QVariant::Map)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 3");
                }
            }
            QMap<QString, QVariant> map2 = list1[i].toMap();

            if(!map2.contains("name") || map2["name"].type() != QVariant::Int)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 4");
                }
            }
            if(!map2.contains("shortPressAvailable") || map2["shortPressAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 5");
                }
            }
            if(!map2.contains("longPressAvailable") || map2["longPressAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 6");
                }
            }
            if(!map2.contains("upDownAvailable") || map2["upDownAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 7");
                }
            }

            ButtonCapabilities bc1;
            bc1.name = map2["name"].toInt();
            bc1.shortPressAvailable = map2["shortPressAvailable"].toBool();
            bc1.longPressAvailable = map2["longPressAvailable"].toBool();
            bc1.upDownAvailable = map2["upDownAvailable"].toBool();
            rc.append(bc1);
        }

        if (map1.contains("presetBankCapabilities"))
        {
            pbc.presence = true;
            if(map1["presetBankCapabilities"].type() != QVariant::Map)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 8");
                }
            }
            QMap<QString, QVariant> map2 = map1["presetBankCapabilities"].toMap();
            if (!map2.contains("onScreenPresetsAvailable") || map2["onScreenPresetsAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid 9");
                }
            }
            pbc.onScreenPresetsAvailable = map2["onScreenPresetsAvailable"].toBool();
        }
        else
        {
            pbc.presence = false;
        }
    }
    else
    {
        if (ctx != NULL) {
            ctx->sendErrorReply(QDBusError::InternalError, "method not found");
        }
    }
    return rc;
}

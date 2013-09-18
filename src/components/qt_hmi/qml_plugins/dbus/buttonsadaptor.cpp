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

    connect(api, SIGNAL(notify(QString)), this, SIGNAL(Notify(QString)));
}

QList<ButtonCapabilities> ButtonsAdaptor::GetCapabilities(OptionalArgument<PresetBankCapabilities> &pbc)
{
    QVariant arg1;
    QDBusContext* ctx = dynamic_cast<QDBusContext*>(parent());
    QList<ButtonCapabilities> rc;

    if (QMetaObject::invokeMethod(buttonsApi, "getCapabilities", Qt::BlockingQueuedConnection,
                                  Q_RETURN_ARG(QVariant, (arg1))))
    {
        if (arg1.type() != QVariant::Map)
        {
            if (ctx != NULL) {
                ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
            }
        }

        QMap<QString, QVariant> map1 = arg1.toMap();

        if (!map1.contains("buttonCapabilities") || map1["buttonCapabilities"].type() != QVariant::List)
        {
            if (ctx != NULL) {
                ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
            }
        }

        QList<QVariant> list1 = map1["buttonCapabilities"].toList();

        for (int i = 0; i < list1.length(); ++i)
        {
            if(list1[i].type() != QVariant::Map)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
                }
            }
            QMap<QString, QVariant> map2 = list1[i].toMap();

            if(!map2.contains("name") || map2["name"].type() != QVariant::Int)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
                }
            }
            if(!map2.contains("shortPressAvailable") || map2["shortPressAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
                }
            }
            if(!map2.contains("longPressAvailable") || map2["longPressAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
                }
            }
            if(!map2.contains("upDownAvailable") || map2["upDownAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
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
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
                }
            }
            QMap<QString, QVariant> map2 = map1["presetBankCapabilities"].toMap();
            if (!map2.contains("onScreenPresetsAvailable") || map2["onScreenPresetsAvailable"].type() != QVariant::Bool)
            {
                if (ctx != NULL) {
                    ctx->sendErrorReply(QDBusError::InternalError, "Returned value is invalid");
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

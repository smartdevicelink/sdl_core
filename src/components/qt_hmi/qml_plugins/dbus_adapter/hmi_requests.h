#ifndef HMI_REQUEST_
#define HMI_REQUEST_

#include <QObject>
#include <QJSValue>
#include <QDBusPendingCall>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDBusAbstractInterface>
#include <QDBusInterface>
#include <QJSEngine>

#include <QDebug>
#include <qml_dbus.h>


class HMIRequest: public QObject {
    Q_OBJECT
  public:
    HMIRequest(QDBusInterface *interface, QString name, QList<QVariant> args, QJSValue hmi_callback):
    interface_(interface),
    name_(name),
    args_(args),
    hmi_callback_(hmi_callback) {
        QDBusPendingCall pcall = interface->asyncCallWithArgumentList(name, args);
        watcher_ = new QDBusPendingCallWatcher(pcall);
        QObject::connect( watcher_, SIGNAL(finished(QDBusPendingCallWatcher*)),
                          this, SLOT(invokeCallback()) );
    }
  protected:
    virtual void fillArgsList() = 0;
    QDBusPendingCallWatcher *watcher_;
    QJSValueList qjsValueList_;
    QJSValue hmi_callback_;
  private:
    QDBusInterface *interface_;
    QString name_;
    QList<QVariant> args_;

  public slots:
    void invokeCallback() {
        this->fillArgsList();
        qDebug() << "?!";
        hmi_callback_.call(qjsValueList_);
        watcher_->deleteLater();
        this->deleteLater();
    }
};

class HMIRequest_Request1: public HMIRequest {
    Q_OBJECT
  public:
    HMIRequest_Request1(QDBusInterface *interface, QString name, QList<QVariant> args, QJSValue hmi_callback):
    HMIRequest(interface, name, args, hmi_callback) {}
  private:
    void fillArgsList() {
        QDBusPendingReply<QString, QString, QString> reply = *watcher_;
        qjsValueList_.append(reply.argumentAt<0>());
        qjsValueList_.append(reply.argumentAt<1>());
        qjsValueList_.append(reply.argumentAt<2>());
    }
};

class HMIRequest_Request2: public HMIRequest {
    Q_OBJECT
  public:
    HMIRequest_Request2(QDBusInterface *interface, QString name, QList<QVariant> args, QJSValue hmi_callback):
    HMIRequest(interface, name, args, hmi_callback) {}
  private:
    void fillArgsList() {
        QDBusPendingReply<QStringList> reply = *watcher_;
        QStringList list = reply.argumentAt<0>();
        QJSValue qjs_array = hmi_callback_.engine()->newArray(0);
        QJSValue pushMethod = qjs_array.prototype().property("push");

        qDebug() << list.length();
        foreach (QString str, list) {
            QJSValueList lst;
            lst.append(str);
            pushMethod.callWithInstance(qjs_array, lst);  // qjsarray.push(str)
        }
        qjsValueList_.append(qjs_array);
    }
};

class HMIRequest_Request3: public HMIRequest {
    Q_OBJECT
  public:
    HMIRequest_Request3(QDBusInterface *interface, QString name, QList<QVariant> args, QJSValue hmi_callback):
        HMIRequest(interface, name, args, hmi_callback) {}
  private:
    void fillArgsList() {
        qDebug() << "1";
        QDBusPendingReply< OptionalArgument<QList<Common_ServiceInfo>> > reply = *watcher_;

        QJSValue qjs_array = hmi_callback_.engine()->newArray(0);
        QJSValue pushMethod = qjs_array.prototype().property("push");

        qDebug() << "2";
        if (reply.argumentAt<0>().presence) {
            qDebug() << "Mandatory True";
            QList<Common_ServiceInfo> qlst = reply.argumentAt<0>().val;
            foreach (Common_ServiceInfo i, qlst) {
                qDebug() << "3";
                QJSValue qjs_obj = hmi_callback_.engine()->newObject();
                qjs_obj.setProperty("policyAppId", i.policyAppId.presence ? QJSValue(i.policyAppId.val) : QJSValue(QJSValue::UndefinedValue));
                qjs_obj.setProperty("url", i.url);
                QJSValueList lst;
                lst.append(qjs_obj);
                pushMethod.callWithInstance(qjs_array, lst);
            }
            qDebug() << "4";
            qjsValueList_.append(qjs_array);
        }
        qDebug() << "5";
    }
};


#endif

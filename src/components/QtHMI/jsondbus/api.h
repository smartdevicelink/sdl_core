#ifndef API_H
#define API_H

#include <QQuickItem>

class SdlCoreApiInterface;

class Api : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Api)
    
    QString name_;
    QString path_;
    SdlCoreApiInterface *api_;

    inline void init();

public:
    explicit Api(QQuickItem *parent = 0);
    ~Api();

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    Q_INVOKABLE QString name() const;
    Q_INVOKABLE QString path() const;

Q_SIGNALS:
    void receive(const QString& json);
    void nameChanged();
    void pathChanged();

public Q_SLOTS:
    void send(const QString& json);
    void setName(const QString& name);
    void setPath(const QString& path);
};

QML_DECLARE_TYPE(Api)

#endif // API_H


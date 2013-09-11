#ifndef ATTRIBUTEDMOUSEEVENT_H
#define ATTRIBUTEDMOUSEEVENT_H

#include <QObject>
#include <qqml.h>
#include <QQuickItem>

class AttributedMouseEvent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* item READ item)
public:
    AttributedMouseEvent(QQuickItem *item)
        : item_(item) { }

    QQuickItem *item() const;
private:
    QQuickItem *item_;
private:
    Q_DISABLE_COPY(AttributedMouseEvent)
};

QML_DECLARE_TYPE(AttributedMouseEvent)

#endif // ATTRIBUTEDMOUSEEVENT_H

#ifndef MASKED_CONTAINER_H
#define MASKED_CONTAINER_H

#include <vector>

#include <QQuickItem>
#include <QString>

#include "attributedmouseevent.h"

class MaskedContainer : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(MaskedContainer)
    
public:
    MaskedContainer(QQuickItem *parent = 0);
    ~MaskedContainer();
signals:
    void pressed(AttributedMouseEvent *attr);
    void released(AttributedMouseEvent *attr);
protected:
    virtual void componentComplete();
    // virtual bool childMouseEventFilter(QQuickItem *item, QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
    std::vector<QQuickItem*> images;
    int *mask;
};

QML_DECLARE_TYPE(MaskedContainer)

#endif // MASKED_CONTAINER_H

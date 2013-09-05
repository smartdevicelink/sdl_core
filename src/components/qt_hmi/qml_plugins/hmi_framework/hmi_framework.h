#ifndef HMIFRAMEWORK_H
#define HMIFRAMEWORK_H

#include <QQuickItem>

class hmiframework : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(hmiframework)
    
public:
    hmiframework(QQuickItem *parent = 0);
    ~hmiframework();
};

QML_DECLARE_TYPE(hmiframework)

#endif // HMIFRAMEWORK_H


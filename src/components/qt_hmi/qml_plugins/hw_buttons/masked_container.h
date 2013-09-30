/**
 * \file masked_container.h
 * \brief MaskedContainer class header file.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_HW_BUTTONS_MASKED_CONTAINER_H_
#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_HW_BUTTONS_MASKED_CONTAINER_H_

#include <vector>

#include <QQuickItem>
#include <QString>

#include "attributed_mouse_event.h"

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

/**
 * \file masked_container.cpp
 * \brief MaskedContainer class source file.
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

#include "masked_container.h"

#include <QImage>
#include <QEvent>

MaskedContainer::MaskedContainer(QQuickItem *parent):
    QQuickItem(parent),
    mask(NULL)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

void MaskedContainer::componentComplete() {
    QQuickItem::componentComplete();

    for (QObjectList::ConstIterator it = children().begin(); it != children().end(); ++it) {
        QQuickItem *item = qobject_cast<QQuickItem*>(*it);
        if (item && item->inherits("QQuickImage") && item->opacity() > 0 && item->isVisible()) {
            images.push_back(item);
        }
    }

    int height = this->height();
    int width  =this->width();

    for (int i = 0; i < images.size(); ++i) {
        QQuickItem *item = images[i];
        int itemWidth = item->width();
        int itemHeight = item->height();
        int itemX = item->x();
        int itemY = item->y();
        if (height < itemHeight + itemY)
            height = itemHeight + itemY;
        if (width < itemWidth + itemX)
            width = itemWidth + itemX;
    }

    setHeight(height);
    setWidth(width);

    mask = new int[height * width];
    std::fill(mask, mask + height * width, -1);

    for (int i = 0; i < images.size(); ++i) {
        QQuickItem *item = images[i];
        int itemWidth = item->width();
        int itemHeight = item->height();
        int itemX = item->x();
        int itemY = item->y();

        QUrl url = item->property("source").toUrl();
        QImage img(url.path());
        const QRgb* bits = reinterpret_cast<const QRgb*>(img.constBits());
        for (int x = 0; x < itemWidth; ++x) {
            for (int y = 0; y < itemHeight; ++y) {
                if (qAlpha(bits[y * itemWidth + x]) > 128) {
                    mask[(itemY + y) * width + (x + itemX)] = i;
                }
            }
        }
    }
}

void MaskedContainer::mousePressEvent(QMouseEvent *mouse) {

    if (width() * mouse->y() + mouse->x() > width() * height())
    {
        mouse->ignore();
        return;
    } else {
        int idx = mask[mouse->y() * static_cast<int>(width()) +
                mouse->x()];
        if (idx >= 0) {
            AttributedMouseEvent ev(images[idx]);
            emit pressed(&ev);
            grabMouse();
            mouse->accept();
        } else {
            mouse->ignore();
        }
    }
}

void MaskedContainer::mouseReleaseEvent(QMouseEvent *mouse) {
    if (width() * mouse->y() + mouse->x() > width() * height()) {
        return;
    } else {
        int idx = mask[mouse->y() * static_cast<int>(width()) +
                mouse->x()];
        if (idx >= 0) {
            AttributedMouseEvent ev(images[idx]);
            emit released(&ev);
        } else {
            emit released(NULL);
        }
    }
    ungrabMouse();
}

MaskedContainer::~MaskedContainer()
{
    delete[] mask;
}


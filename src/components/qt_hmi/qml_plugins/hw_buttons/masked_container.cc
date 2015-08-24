/*
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

#if QT_4
#  include <QtCore/QPointF>
#  include <QtGui/QGraphicsSceneMouseEvent>
#  define IMAGE "QDeclarativeImage"
#elif QT_5
#  include <QtGui/QImage>
#  include <QtCore/QEvent>
#  define IMAGE "QQuickImage"
#endif  // QT_VERSION

MaskedContainer::MaskedContainer(Item *parent)
    : Item(parent),
      mask_(NULL) {
  setAcceptedMouseButtons(Qt::LeftButton);
}

void MaskedContainer::componentComplete() {
    Item::componentComplete();

    for (QObjectList::ConstIterator it = children().begin();
         it != children().end(); ++it) {
        Item *item = qobject_cast<Item*>(*it);
        if (item && item->inherits(IMAGE) && item->isVisible()) {
            images_.push_back(item);
        }
    }

    int height = this->height();
    int width  =this->width();

    for (size_t i = 0; i < images_.size(); ++i) {
        Item *item = images_[i];
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

    mask_ = new int[height * width];
    std::fill(mask_, mask_ + height * width, -1);

    for (size_t i = 0; i < images_.size(); ++i) {
        Item *item = images_[i];
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
                    mask_[(itemY + y) * width + (x + itemX)] = i;
                }
            }
        }
    }
}

void MaskedContainer::mousePressEvent(MouseEvent *mouse) {
#if QT_4
  qreal x = mouse->pos().x();
  qreal y = mouse->pos().y();
#elif QT_5
  int x = mouse->x();
  int y = mouse->y();
#endif  // QT_VERSION

  if (width() * y + x > width() * height()) {
    mouse->ignore();
  } else {
    int idx = mask_[indexOfMask(x, y)];
    if (idx >= 0) {
      AttributedMouseEvent ev(images_[idx]);
      emit pressed(&ev);
      grabMouse();
      mouse->accept();
    } else {
      mouse->ignore();
    }
  }
}

void MaskedContainer::mouseReleaseEvent(MouseEvent *mouse) {
#if QT_4
  qreal x = mouse->pos().x();
  qreal y = mouse->pos().y();
#elif QT_5
  int x = mouse->x();
  int y = mouse->y();
#endif  // QT_VERSION

  if (width() * y + x > width() * height()) {
    return;
  } else {
    int idx = mask_[indexOfMask(x, y)];
    if (idx >= 0) {
      AttributedMouseEvent ev(images_[idx]);
      emit released(&ev);
    } else {
      emit released(NULL);
    }
  }
  ungrabMouse();
}

MaskedContainer::~MaskedContainer() {
  delete[] mask_;
}


/**
 * @file PagedFlickable.qml
 * @brief Animated row.
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

import QtQuick 1.1

Item
{
    id: flickablePage
    height: container.height + pager.height
    default property alias content: containerRow.children
    property alias spacing: containerRow.spacing
    property int elementWidth
    property int snapTo
    property int count: 0

    Flickable {
        id: container
        anchors.bottom: parent.bottom
        maximumFlickVelocity: 1500
        contentWidth: containerRow.width
        height: containerRow.height
        width: parent.width
        interactive: contentWidth > width

        onMovementEnded: {
            var rest = contentX % snapTo
            var t = 0.25
            if (rest > parent.snapTo / 2) {
                rest = rest - parent.snapTo
            }
            var vel = 2 * rest / t
            flickDeceleration = Math.abs(vel) / t
            flick(vel, 0)
            flickDeceleration = 1500
        }
        Row {
            id: containerRow
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Pager {
        id: pager
        space: 7
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        pages: {
            if ( (container.contentWidth % container.width) >= (flickablePage.elementWidth / 2 + flickablePage.spacing)) {
                return Math.ceil(container.contentWidth / container.width)
            }
            else {
                return Math.floor(container.contentWidth / container.width)
            }
        }

        activePage: {
            if (container.contentX <= 0) {
                return 0
            }
            else if ( (container.contentWidth - container.contentX) < container.width) {
                return pages -1
            }
            else {
                if ( (container.contentX % container.width) >= (flickablePage.elementWidth / 2 + flickablePage.spacing)) {
                    return Math.ceil(container.contentX / container.width)
                }
                else if ( (container.contentX % container.width) > 0) {
                    return Math.floor(container.contentX / container.width)
                }
            }
        }
    }
}

/**
 * @file VIPopUp.qml
 * @brief Pop up window with information about vehicle.
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
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../controls"

PopUp {
    //TODO{ALeshin}: Redraw this window as ListView or Column, when we'll get requirements
    Column {
        anchors.fill: parent
        Text {
            id: viText
            width: parent.width
            height: 1/5 * parent.height
            text: "Vehicle Information"
            font.pixelSize: Constants.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: Constants.primaryColor
        }

        Row {
            height: 1/5 * parent.height
            width: parent.width

            Text {
                width: 1/4 * parent.width
                height: 1/5 * parent.height
                anchors.verticalCenter: parent.verticalCenter
                text: "PRNDL: "
                font.pixelSize: Constants.fontSize
                verticalAlignment: Text.AlignVCenter
                color: Constants.primaryColor
            }

            ComboBox {
                anchors.verticalCenter: parent.verticalCenter
                id: comboBox
                model: prndlList

                onCurrentIndexChanged: {
                    console.debug("PRNDL:", currentIndex)
                    dataContainer.vehicleInfoModel.prndl = model.get(currentIndex).id
                }

                textRole: "name"
                ListModel {
                    id: prndlList
                    Component.onCompleted: {
                        append({ id: -1, name: "<NO DATA (prndl)>" })
                        for (var name in Common.PRNDL) {
                            append({ id: Common.PRNDL[name], name: name });
                        }
                    }
                }
            }
            z: 1000
        }

        Text {
            width: parent.width
            height: 1/5 * parent.height
            text: "ECU 1:     " + dataContainer.vehicleInfoModel.ecuDIDData.data1
            font.pixelSize: Constants.fontSize
            verticalAlignment: Text.AlignVCenter
            color: Constants.primaryColor
        }

        Text {
            width: parent.width
            height: 1/5 * parent.height
            text: "ECU 2:     " + dataContainer.vehicleInfoModel.ecuDIDData.data2
            font.pixelSize: Constants.fontSize
            verticalAlignment: Text.AlignVCenter
            color: Constants.primaryColor
        }
    }
}

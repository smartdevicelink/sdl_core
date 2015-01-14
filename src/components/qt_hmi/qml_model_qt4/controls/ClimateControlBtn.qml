/**
 * @file ClimateControlBtn.qml
 * @brief Round button, which is used at climat menu.
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

Item
{
    id: climateControlBtn
    width: climateBtnImg.width
    height: climateBtnImg.height
    property string name: ""
    property string txt: ""
    state: "off"

    Image {
        id: climateBtnImg
        source: climateControlBtn.state === "off" ? "../res/climate/climate_round_off_btn.png" : "../res/climate/climate_round_on_btn.png"

        Image {
            id: hoverImg
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            property string st: climateControlBtn.state === "off" ? "_off" : "_on"
            source: climateControlBtn.name === "" ? "": "../res/climate/" + climateControlBtn.name + st + ".png"
        }

        Text {
            id: hoverText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: climateControlBtn.txt
            font.pixelSize: Constants.fontSize
            color: climateControlBtn.state === "off" ? Constants.primaryColor : Constants.secondaryColor
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(climateControlBtn.state === "on") {
                    climateControlBtn.state = "off"
                    climateBtnImg.source = "../res/climate/climate_round_off_btn.png"
                    if(climateControlBtn.txt != "") {
                        hoverText.color = Constants.primaryColor
                    }
                    if(climateControlBtn.name != "") {
                        hoverImg.source = "../res/climate/" + climateControlBtn.name + "_off.png"
                    }
                } else {
                    climateControlBtn.state = "on"
                    climateBtnImg.source = "../res/climate/climate_round_on_btn.png"
                    if(climateControlBtn.txt != "") {
                        hoverText.color = Constants.secondaryColor
                    }
                    if(climateControlBtn.name != "") {
                        hoverImg.source = "../res/climate/" + climateControlBtn.name + "_on.png"
                    }
                }
            }
        }
    }
}

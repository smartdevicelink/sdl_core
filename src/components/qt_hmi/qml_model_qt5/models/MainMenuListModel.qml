/**
 * @file MainMenuListModel.qml
 * @brief Main menu list of elements.
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
import QtQuick 2.0

ListModel
{
    ListElement {
        name: "Climate"
        icon: "../res/snow.png"
        qml: "./views/ClimateControlView.qml"
    }

    ListElement {
        name: "Navigation"
        icon: "../res/arrow.png"
        qml: "./views/NavigationNoRouteGridView.qml"
    }

    ListElement {
        name: "Media"
        icon: "../res/notes.png"
        qml: "./views/MusicSourceView.qml"
    }

    ListElement {
        name: "Settings"
        icon: "../res/gear.png"
        qml: "./views/SettingsSourceView.qml"
    }

    ListElement {
        name: "Apps"
        icon: "../res/apps.png"
        qml: "./views/ApplicationListView.qml"
    }

    ListElement {
        name: "Phone"
        icon: "../res/phone/phone.png"
        qml: "./views/PhoneMenuGridView.qml"
    }

    ListElement {
        name: "Car"
        icon: "../res/car.png"
        qml: "./views/CarMenuGridView.qml"
    }
}

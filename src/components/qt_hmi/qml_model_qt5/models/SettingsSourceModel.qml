/**
 * @file SettingsSourceModel.qml
 * @brief Settings source menu list of elements.
 * Copyright (c) 2014, Ford Motor Company
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
        title: "Allow SDL Functionality"
        qml: "./views/SDLFunctionalityView.qml"
        appId: 0
        action: ""
    }
    ListElement {
        title: "Update SDL"
        qml: ""
        appId: 0
        action: "update_sdl"
    }
    ListElement {
        title: "Policy table update status"
        qml: ""
        appId: 0
        action: "get_status_update"
    }
    ListElement {
        title: "Send request GetURLS"
        qml: ""
        appId: 0
        action: "get_urls"
    }
    ListElement {
        title: "Statistics info settings"
        qml: "./views/StatisticsInfoView.qml"
        appId: 0
        action: ""
    }
    ListElement {
        title: "App permissions"
        qml: "./views/PolicyAppListView.qml"
        appId: 0
        action: ""
    }
    ListElement {
        title: "Device state change"
        qml: "./views/DeviceStateChangedView.qml"
        appId: 0
        action: ""
    }
    ListElement {
        title: "System Error"
        qml: "./views/SystemErrorView.qml"
        appId: 0
        action: ""
    }
    ListElement {
        title: "System Request"
        qml: "./views/SystemRequestView.qml"
        appId: 0
        action: ""
    }
}

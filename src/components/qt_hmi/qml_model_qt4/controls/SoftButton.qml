/**
 * @file SoftButton.qml
 * @brief Soft button view
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
import "../hmi_api/Common.js" as Common
import "SoftButton.js" as SoftButton

OvalButton {
    property variant button
    property int appId
    visible: !!button

    signal defaultAction;
    signal stealFocus;
    signal keepContext;

    property int actionOrder: SoftButton.Action.doOnClicked

    highlighted: button ? button.isHighlighted : false
    onPressed: {
        actionOrder = SoftButton.Action.doOnClicked
        sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonEventMode.BUTTONDOWN, button.softButtonID)
    }

    onReleased: {
        if (actionOrder === SoftButton.Action.doOnReleased) {
            switch (button.systemAction) {
            case Common.SystemAction.DEFAULT_ACTION:
                defaultAction();
                break;
            case Common.SystemAction.STEAL_FOCUS:
                if (dataContainer.currentApplication.isMediaApplication) {
                    contentLoader.go("views/SDLPlayerView.qml", appId);
                } else {
                    contentLoader.go("views/SDLNonMediaView.qml", appId);
                }
                stealFocus();
                break;
            case Common.SystemAction.KEEP_CONTEXT:
                keepContext();
                break;
            }
        }

        sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonEventMode.BUTTONUP, button.softButtonID)
    }

    onClicked: {
        sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonPressMode.SHORT, button.softButtonID);
        switch (button.systemAction) {
        case Common.SystemAction.DEFAULT_ACTION:
            defaultAction();
            break;
        case Common.SystemAction.STEAL_FOCUS:
            if (dataContainer.currentApplication.isMediaApplication) { contentLoader.go("views/SDLPlayerView.qml", appId); }
                else { contentLoader.go("views/SDLNonMediaView.qml", appId); }
            stealFocus();
            break;
        case Common.SystemAction.KEEP_CONTEXT:
            keepContext();
            break;
        }
    }

    onPressAndHold: {
        actionOrder = SoftButton.Action.doOnReleased; // action should be triggered on release
        sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonPressMode.LONG, button.softButtonID);
    }

    onButtonChanged: {
        icon = button && button.type !== Common.SoftButtonType.SBT_TEXT ? button.image : undefined;
        text = button && button.type !== Common.SoftButtonType.SBT_IMAGE ? button.text : ""
    }
}

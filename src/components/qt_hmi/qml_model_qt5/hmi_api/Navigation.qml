/**
 * @file Navigation.qml
 * @brief Navigation.
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
import "Common.js" as Common

Item {
    function isReady () {
        return {
            available: dataContainer.hmiNavigationAvailable
        }
    }

    function showConstantTBT(navigationTexts, turnIcon, nextTurnIcon, distanceToManeuver,
                             distanceToManeuverScale, maneuverComplete,
                             softButtons, appID) {
        console.debug("enter")
        var app = dataContainer.getApplication(appID)
        var dataToUpdate = {}

        navigationTexts.forEach(fillTexts, dataToUpdate) // Check
        if (turnIcon !== undefined) { dataToUpdate.turnIcon = turnIcon }
        if (nextTurnIcon !== undefined) { dataToUpdate.nextTurnIcon = nextTurnIcon }
        if (distanceToManeuver !== undefined) {
            dataToUpdate.distanceToManeuver = distanceToManeuver
        } else {
            DBus.sendReply( {__retCode: Common.Result.INVALID_DATA, __message: "distanceToManeuver absence"} )
        }
        if (distanceToManeuverScale !== undefined) {
            dataToUpdate.distanceToManeuverScale = distanceToManeuverScale
        } else {
            DBus.sendReply( {__retCode: Common.Result.INVALID_DATA, __message: "distanceToManeuverScale absence"} )
        }
        if (maneuverComplete !== undefined) {
            dataToUpdate.maneuverComplete = maneuverComplete
        }
        if (softButtons !== undefined) {
            app.navigationSoftButtons.clear()
            softButtons.forEach(function(x) { app.navigationSoftButtons.append(x); });
        }
        if (appID !== undefined) {
            dataToUpdate.appID = appID
        } else {
            dBus.sendReply( {__retCode: Common.Result.INVALID_DATA, __message: "appID absence"} )
        }

        dataContainer.setApplicationProperties(appID, { navigationModel : dataToUpdate } )
        contentLoader.go("./views/TurnByTurnView.qml", appID)
    }

    function alertManeuver(softButtons) {
        console.debug("enter")
        console.debug("exit")
        return { __retCode: Common.Result.SUCCESS }
    }

    function updateTurnList(turnList, softButtons, appID) {
        console.debug("enter")
        if (turnList !== undefined) {
            dataContainer.getApplication(appID).turnList.clear();
            turnList.forEach(fillTurnList, dataContainer.getApplication(appID).turnList);
        }
        if (softButtons !== undefined) {
            dataContainer.getApplication(appID).turnListSoftButtons.clear();
            softButtons.forEach(fillSoftButtons, dataContainer.getApplication(appID).turnListSoftButtons);
        }
        dataContainer.navigationModel.appId = appID;
        console.debug("exit")
    }

    function fillTexts(element, index, array) {
        switch (element.fieldName) {
        case Common.TextFieldName.navigationText1:
            this.text1 = element.fieldText;
            break;
        case Common.TextFieldName.navigationText2:
            this.text2 = element.fieldText;
            break;
        case Common.TextFieldName.ETA:
            this.eta = element.fieldText;
            break;
        case Common.TextFieldName.totalDistance:
            this.totalDistance = element.fieldText;
            break;
        case Common.TextFieldName.timeToDestination:
            this.timeToDestination = element.fieldText;
            break;
        }
    }

    function fillSoftButtons(element, index, array) {
        this.append({
                        type: element.type,
                        text: element.text,
                        image: element.image,
                        isHighlighted: element.isHighlighted,
                        buttonId: element.softButtonID,
                        systemAction: element.systemAction
                    });
    }

    function fillTurnList(element, index, array) {
        this.append({                        
                        navigationText: element.navigationText,
                        turnIcon: element.turnIcon
                    });
        console.debug(element.navigationText)
    }    
}

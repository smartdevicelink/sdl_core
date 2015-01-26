/**
 * @file DataStorage.qml
 * @brief Container for data.
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
import "../hmi_api/Common.js" as Common
import "Internal.js" as Internal
import "Constants.js" as Constants

QtObject {
    property string contactsFirstLetter // first letter of contact's name that need to find at contact list
    property ApplicationModel currentApplication: ApplicationModel { }
    property SliderModel uiSlider: SliderModel { }
    property PerformAudioPassThruModel uiAudioPassThru: PerformAudioPassThruModel { }
    property ScrollableMessageModel scrollableMessageModel: ScrollableMessageModel { }

    function getApplication(appId) {
        console.log("enter");
        for(var i = 0; i < applicationList.count; i++) {
            if(applicationList.get(i).appId === appId) {
                console.log("Application found", applicationList.get(i));
                return applicationList.get(i);
            }
        }
        console.log("exit");
    }

    function getStashedApplication(appId) {
        console.log("enter getStashedApplication function");
        for(var i = 0; i < stashedApplicationsList.count; i++) {
            if(stashedApplicationsList.get(i).appId === appId) {
                console.log("Application found", stashedApplicationsList.get(i));
                return stashedApplicationsList.get(i);
            }
        }
        console.log("exit getStashedApplication: application was not found");
    }

    onApplicationListChanged: {
        console.log("onApplicationListChanged()");
        setCurrentApplication(appId);
    }

    function setCurrentApplication(appId) {
        console.log("Enter setCurrentApplication function");
        var oldApplicationContext = applicationContext;
        if (currentApplication.appId !== appId) {
            applicationContext = false;
        }

        for(var i = 0; i < applicationList.count; i++) {
            if(applicationList.get(i).appId === appId) {
                currentApplication.appId = appId
                var application = applicationList.get(i)

                currentApplication.appName = application.appName
                currentApplication.appType = application.appType
                currentApplication.appIcon.source = application.icon
                currentApplication.playPauseState = application.playPauseState
                currentApplication.options = application.options
                currentApplication.softButtons = application.softButtons
                currentApplication.navigationSoftButtons = application.navigationSoftButtons
                currentApplication.alertManeuverSoftButtons = application.alertManeuverSoftButtons
                currentApplication.customPresets = application.customPresets

                if (application.navigationModel.text1 !== undefined) {
                    currentApplication.navigationModel.text1 = application.navigationModel.text1
                }
                if (application.navigationModel.text2 !== undefined) {
                    currentApplication.navigationModel.text2 = application.navigationModel.text2
                }
                if (application.navigationModel.totalDistance !== undefined) {
                    currentApplication.navigationModel.totalDistance = application.navigationModel.totalDistance
                }
                if (application.navigationModel.eta !== undefined) {
                    currentApplication.navigationModel.eta = application.navigationModel.eta
                }
                if (application.navigationModel.timeToDestination !== undefined) {
                    currentApplication.navigationModel.timeToDestination = application.navigationModel.timeToDestination
                }
                if (application.navigationModel.turnIcon !== undefined) {
                    currentApplication.navigationModel.turnIcon = application.navigationModel.turnIcon
                }
                if (application.navigationModel.nextTurnIcon !== undefined) {
                    currentApplication.navigationModel.nextTurnIcon = application.navigationModel.nextTurnIcon
                }
                if (application.navigationModel.distanceToManeuver !== undefined) {
                    currentApplication.navigationModel.distanceToManeuver = application.navigationModel.distanceToManeuver
                }
                if (application.navigationModel.distanceToManeuverScale !== undefined) {
                    currentApplication.navigationModel.distanceToManeuverScale = application.navigationModel.distanceToManeuverScale
                }
                if (application.navigationModel.maneuverComplete !== undefined) {
                    currentApplication.navigationModel.maneuverComplete = application.navigationModel.maneuverComplete
                }
                if (application.navigationModel.appID !== undefined) {
                    currentApplication.navigationModel.appID = application.navigationModel.appID
                }
                if (application.hmiUIText.mainField1 !== undefined) {
                    currentApplication.hmiUIText.mainField1 = application.hmiUIText.mainField1
                }
                if (application.hmiUIText.mainField2 !== undefined) {
                    currentApplication.hmiUIText.mainField2 = application.hmiUIText.mainField2
                }
                if (application.hmiUIText.mainField3 !== undefined) {
                    currentApplication.hmiUIText.mainField3 = application.hmiUIText.mainField3
                }
                if (application.hmiUIText.mainField4 !== undefined) {
                    currentApplication.hmiUIText.mainField4 = application.hmiUIText.mainField4
                }
                if (application.hmiUIText.statusBar !== undefined) {
                    currentApplication.hmiUIText.statusBar = application.hmiUIText.statusBar
                }
                if (application.hmiUIText.mediaTrack !== undefined) {
                    currentApplication.hmiUIText.mediaTrack = application.hmiUIText.mediaTrack
                }
                if (application.hmiUIText.image !== undefined) {
                    currentApplication.hmiUIText.image = application.hmiUIText.image
                }
                if (application.hmiUIText.secondaryImage!== undefined) {
                    currentApplication.hmiUIText.secondaryImage= application.hmiUIText.secondaryImage
                }
                if (application.hmiUITextAlignment !== undefined) {
                    currentApplication.hmiUITextAlignment = application.hmiUITextAlignment
                }
                if (application.helpPrompt !== undefined) {
                    currentApplication.helpPrompt = application.helpPrompt
                }
                if (application.timeoutPrompt !== undefined) {
                    currentApplication.timeoutPrompt = application.timeoutPrompt
                }
                if (application.mediaClock.startTime !== undefined) {
                    currentApplication.mediaClock.startTime = application.mediaClock.startTime
                }
                if (application.mediaClock.endTime !== undefined) {
                    currentApplication.mediaClock.endTime = application.mediaClock.endTime
                }
                // Check fields with mandatory = false

                if (application.menuIcon !== undefined) {
                    currentApplication.menuIcon.source = application.menuIcon
                }
                currentApplication.vrHelpTitle = application.vrHelpTitle
                currentApplication.vrHelpTitleDefault = application.vrHelpTitleDefault
                currentApplication.vrHelpTitlePerformInteraction = application.vrHelpTitlePerformInteraction
                currentApplication.menuTitle = application.menuTitle
                currentApplication.vrHelpItems = application.vrHelpItems
                currentApplication.vrHelpItemsPerformInteraction = application.vrHelpItemsPerformInteraction
                currentApplication.deviceName = application.deviceName
                currentApplication.isMediaApplication = application.isMediaApplication
                currentApplication.turnList = application.turnList
                currentApplication.turnListSoftButtons = application.turnListSoftButtons
                currentApplication.mediaClock.updateMode = application.mediaClock.updateMode
                currentApplication.mediaClock.runningMode = application.mediaClock.runningMode
                if (application.mediaClock.startTimeForProgress !== undefined) {
                    currentApplication.mediaClock.startTimeForProgress = application.mediaClock.startTimeForProgress
                }
                currentApplication.languageTTSVR = application.languageTTSVR
                currentApplication.hmiDisplayLanguageDesired = application.hmiDisplayLanguageDesired
                currentApplication.allowedFunctions = application.allowedFunctions
                // This place is for adding new properties
            }
        }
        applicationContext = oldApplicationContext;
        currentApplicationChanged()
        console.log("Exit setCurrentApplication function")
    }

    function addApplication(app) {
        console.log("enter")
        applicationList.append({
            appName: app.appName,
            ngnMediaScreenAppName: app.ngnMediaScreenAppName,
            icon: app.icon,
            menuIcon: {
                value: "",
                imageType: 1
            },
            deviceName: app.deviceName,
            appId: app.appId,
            hmiDisplayLanguageDesired: app.hmiDisplayLanguageDesired,
            isMediaApplication: app.isMediaApplication,
            appType: app.appType,
            helpPrompt: "",
            timeoutPrompt: "",
            customPresets: app.customPresets ? app.customPresets : [],
            playPauseState: 'Pause',
            hmiUIText: app.hmiUIText,
            hmiUITextAlignment: app.hmiUITextAlignment,
            options: [],
            turnList: [],
            turnListSoftButtons: [],
            mediaClock: app.mediaClock,
            languageTTSVR: Common.Language.EN_US,
            softButtons: [],
            vrHelpTitle: "",
            vrHelpTitlePerformInteraction: "",
            vrHelpTitleDefault: "VR HELP",
            menuTitle: "",
            keyboardProperties: [],
            vrHelpItems: [],
            vrHelpItemsPerformInteraction: [],
            vrHelpItemsDefault: {
                   text: "VrHelpItems - defaultText1",
                   image: "",
                   position: 1
            },
            navigationSoftButtons: [],
            alertManeuverSoftButtons: [],
            navigationModel: {
                           appID: -1,
                           text1: "",
                           text2: "",
                           totalDistance: "",
                           eta: "",
                           timeToDestination: "",
                           turnIcon: {
                               value: "",
                               imageType: -1
                           },
                           nextTurnIcon: {
                               value: "",
                               imageType: -1
                           },
                           distanceToManeuver: 0,
                           distanceToManeuverScale: 0,
                           maneuverComplete: false
            },
            allowedFunctions: []
            // This place is for adding new properties
        })
        if (app.isMediaApplication) {
            musicSourceModel.insert(0, {
                "title": app.appName,
                "qml": "views/SDLPlayerView.qml",
                "appId": app.appId
            })
        }

        console.log("exit")
    }

    function setApplicationProperties(appId, props) {
        console.log("Enter setApplicationProperties function");

        var app = getApplication(appId)
        for (var p in props) {
            console.debug(p)
            if (props[p] !== undefined) {
                app[p] = props[p]
            }
        }
        if (currentApplication.appId === appId) {
            setCurrentApplication(appId); // copy new values to current application
        }
        console.log("Exit setApplicationProperties function");
    }

    function removeApplication(appId) {
        console.log("enter removeApplication");
        for (var j = 0; j < musicSourceModel.count; ++j) {
            if (musicSourceModel.get(j).appId === appId) {
                musicSourceModel.remove(j);
                break;
            }
        }
        for (var i = 0; i < applicationList.count; i++) {
            if (applicationList.get(i).appId === appId) {
                applicationList.remove(i);
                break;
            }
        }
        console.log("exit removeApplication");
    }

    function stashApplication(appId) {
        console.log("stashApplication enter");
        stashedApplicationsList.append(getApplication(appId))
        console.log("stashApplication exit");
    }

    function getResumeResult (appID) {

        if (getStashedApplication(appId) && getApplication(appID)) {
            var stashedApp = getStashedApplication(appId),
                app = getApplication(appID);
            if (stashedApp.options.count == app.options.count) {
                for (var optionIndex = 0; optionIndex < stashedApp.options.count; ++optionIndex) {
                    if (app.options.get(optionIndex).type === Internal.MenuItemType.MI_SUBMENU) {
                        //comparison subMenu compare

                        if ( !((app.options.get(optionIndex).id === stashedApp.options.get(optionIndex).id) &&
                             (app.options.get(optionIndex).name === stashedApp.options.get(optionIndex).name) &&
                             (app.options.get(optionIndex).position === stashedApp.options.get(optionIndex).position) &&
                             (app.options.get(optionIndex).icon === stashedApp.options.get(optionIndex).icon))) {
                            return {
                                result: false
                            }
                        }

                        var stashedAppSubMenu = stashedApp.options.get(optionIndex).subMenu,
                            appSubMenuArray = app.options.get(optionIndex).subMenu;

                        if (stashedAppSubMenu.length != appSubMenuArray.length) {
                            return {
                                result: false
                            }
                        }

                        for (var subMenuIndex = 0; subMenuIndex < stashedAppSubMenu.length; subMenuIndex++) {
                            if ( !((appSubMenuArray[subMenuIndex].id === stashedAppSubMenu[subMenuIndex].id) &&
                                 (appSubMenuArray[subMenuIndex].name === stashedAppSubMenu[subMenuIndex].name) &&
                                 (appSubMenuArray[subMenuIndex].position === stashedAppSubMenu[subMenuIndex].position) &&
                                 (appSubMenuArray[subMenuIndex].icon === stashedAppSubMenu[subMenuIndex].icon)
                                )) {
                                return {
                                    result: false
                                }
                            }
                        }
                    } else {
                        //comparison Command compare
                       if ( !((app.options.get(optionIndex).id === stashedApp.options.get(optionIndex).id) &&
                            (app.options.get(optionIndex).name === stashedApp.options.get(optionIndex).name) &&
                            (app.options.get(optionIndex).position === stashedApp.options.get(optionIndex).position) &&
                            (app.options.get(optionIndex).icon === stashedApp.options.get(optionIndex).icon)
                        )) {
                           return {
                               result: false
                           }
                       }
                    }
                }
                // if all commands and subMenus were same
                // rerturn SUCCESS result
                return {
                    result: true
                }
            }
        }

        return {
            result: false
        }
    }

    property int systemContext
    property int hmiContext
    property bool applicationContext: false
    property bool applicationSavedContext
    property string routeText: ""
    property PlayerState cdPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState ipodPlayerState: PlayerState {
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
        playPauseState: 'Pause'
    }
    property PlayerState usbPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState linePlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState btPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState amPlayerState: PlayerState {
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: [ "1130", "950", "760", "1270", "1400", "2100", "940", "1001" ]
    }
    property PlayerState fmPlayerState: PlayerState {
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: ["96.3", "107.9", "104.3", "101.9", "105.3", "100.5", "107.9", "103.4"]
    }
    property PlayerState siriusPlayerState: PlayerState {
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: [ "Lithium", "Spectrum", "ESPN", "Alt Nation", "Lithium", "Spectrum", "ESPN", "Alt Nation" ]
    }

    property bool hmiVRAvailable: false
    property bool hmiTTSAvailable: false
    property bool hmiNavigationAvailable: false
    property bool hmiVehicleInfoAvailable: false
    property bool hmiUIAvailable: false

    property int hmiUILanguage: Common.Language.EN_US
    property int hmiTTSVRLanguage: Common.Language.EN_US

    property ListModel deviceList: ListModel { }
    property ListModel applicationList: ListModel { }
    property ListModel stashedApplicationsList: ListModel { }
    property ListModel vrCommands: ListModel {}

    function reset () {
        console.log("dataContainer reset enter");
        routeText = ""
        console.log("dataContainer reset exit");
    }

    function changeRegistrationUI (language, appID) {
        console.log("dataContainer changeRegistrarionUI enter");
        setApplicationProperties(appID, { hmiDisplayLanguageDesired: language });
        console.log("dataContainer changeRegistrarionUI exit");
    }

    function changeRegistrationTTSVR(language, appID) {
        console.log("dataContainer changeRegistrationTTSVR enter");
        setApplicationProperties(appID, { languageTTSVR: language });
        console.log("dataContainer changeRegistrationTTSVR exit");
    }

    function addCommand (cmdID, menuParams, cmdIcon, appID) {
        console.debug("enter: " +
                      cmdID +
                      ", " +
                      (menuParams ?
                          "{" + menuParams.parentID + ", " + menuParams.position + ", " + menuParams.menuName + "}" : menuParams) +
                          ", " +
                      (cmdIcon ?
                          "{" + cmdIcon.value + ", " + cmdIcon.imageType + "}" : cmdIcon) +
                      ", " +
                      appID
        )
        var commandToAddPosition
        var app = getApplication(appID)
        var currentMenu = app.options
        var maximumCommands = Constants.maximumCommandsPerSubmenu
        var index = 0

        if ((menuParams) && (menuParams.parentID)) { // Work with sub menu
            var parentNotFound = true
            for (var optionIndex = 0; optionIndex < app.options.count; ++optionIndex) {
                if ((app.options.get(optionIndex).type === Internal.MenuItemType.MI_SUBMENU) && (app.options.get(optionIndex).id === menuParams.parentID)) {
                    currentMenu = app.options.get(optionIndex).subMenu
                    parentNotFound = false
                    maximumCommands += 1
                }
            }
            if (parentNotFound) {
                console.log("UI::addCommand(): parentID " + menuParams.parentID + " not found")
            }
        }

        if (currentMenu.count < maximumCommands) {
            if (menuParams && (menuParams.position !== undefined)) {
                commandToAddPosition = menuParams.position
            } else {
                commandToAddPosition = Constants.positionOfElementWithoutPosition
            }
            for (var i = 0; i < currentMenu.count; i++) {
                if (commandToAddPosition >= currentMenu.get(i).position) {
                    index = i + 1
                } else {
                    break
                }
            }
            currentMenu.insert( index,
                               { id: cmdID,
                                 name: menuParams.menuName,
                                 type: Internal.MenuItemType.MI_NODE,
                                 position: commandToAddPosition,
                                 icon: cmdIcon ? cmdIcon : {},
                                 subMenu: []
                               })
        } else {
            console.log("addCommand(): too many commands in menu: ", menuParams.parentID, " rejecting")
            throw Common.Result.REJECTED
        }
        console.debug("exit")
    }

    function deleteCommand (cmdID, appID) {
        console.debug("enter: " + cmdID + ", " + appID)
        for (var optionIndex = 0; optionIndex < getApplication(appID).options.count; ++optionIndex) {
            var option = getApplication(appID).options.get(optionIndex)
            if (option.type === Internal.MenuItemType.MI_NODE) {
                if (option.id === cmdID) {
                    getApplication(appID).options.remove(optionIndex)
                    break
                }
            }
            if (option.type === Internal.MenuItemType.MI_SUBMENU) {
                var subMenu = option.subMenu
                var idMatchFound = false
                for (var subOptionIndex = 0; subOptionIndex < subMenu.count; ++subOptionIndex) {
                    if (subMenu.get(subOptionIndex).id === cmdID) {
                        idMatchFound = true
                        if (subMenu !== currentApplication.currentSubMenu) {
                            subMenu.remove(subOptionIndex)
                        }
                        else {
                            console.log("UI::deleteCommand(): cannot remove item from current submenu")
                            throw Common.Result.IN_USE
                        }
                        break
                    }
                }
                if (idMatchFound) {
                    break
                }
            }
        }
        console.debug("exit")
    }

    function addSubMenu (menuID, menuParams, appID) {
        console.debug("enter: " + menuID + ", {" + menuParams.parentID + ", " + menuParams.position + ", " + menuParams.menuName + "}, " + appID)
        var app = getApplication(appID)
        var index = 0
        var count = app.options.count
        var subMenuToAddPosition

        if (count < Constants.maximumSubmenus) {
            if (menuParams.position !== undefined) {
                subMenuToAddPosition = menuParams.position
            } else {
                subMenuToAddPosition = Constants.positionOfElementWithoutPosition
            }
            for (var i = 0; i < count; i++) {
                if (subMenuToAddPosition >= app.options.get(i).position) {
                    index = i + 1
                } else {
                    break
                }
            }
            app.options.insert(index, {
                "id": menuID,
                "name": menuParams.menuName,
                "position": subMenuToAddPosition,
                "type": Internal.MenuItemType.MI_SUBMENU,
                "icon": undefined,
                "subMenu": [{
                    "id": -1,
                    "name": menuParams.menuName,
                    "position": -1,
                    "type": Internal.MenuItemType.MI_PARENT,
                    "icon": {
                        "imageType": Common.ImageType.DYNAMIC,
                        "value": "../res/nav/turnArrow.png"
                    },
                    "subMenu": getApplication(appID).options
                }]
            })
        }
        else {
            console.log("addSubMenu(): too many submenus, rejecting")
            throw Common.Result.REJECTED
        }
        console.debug("exit")
    }

    function deleteSubMenu (menuID, appID) {
        console.debug("enter: " + menuID + ", " + appID)
        for (var optionIndex = 0; optionIndex < getApplication(appID).options.count; ++optionIndex) {
            var option = getApplication(appID).options.get(optionIndex)
            if ((option.type === Internal.MenuItemType.MI_SUBMENU) && (option.id === menuID)) {
                if (option.subMenu !== currentApplication.currentSubMenu) {
                    getApplication(appID).options.remove(optionIndex)
                }
                else {
                    console.log("UI::deleteSubMenu(): cannot remove current submenu")
                    throw Common.Result.IN_USE
                }
                break
            }
        }
        console.debug("exit")
    }
    property VehicleInfoModel vehicleInfoModel: VehicleInfoModel { }
    property bool activeVR: false
    property int driverDistractionState: Common.DriverDistractionState.DD_OFF
    onDriverDistractionStateChanged: {
        sdlUI.onDriverDistraction(driverDistractionState);
    }
    property bool activeTTS: false
    property var activePopup: [] // Stack of names of active pop ups
    property int popups: 0

    property bool activeAlert: false

    onActiveVRChanged: setSystemContext()
    onActiveAlertChanged: setSystemContext()
    onPopupsChanged: setSystemContext()

    function setSystemContext () {
        console.debug("enter")
        if (popups > 0) {
            systemContext = Common.SystemContext.SYSCTXT_HMI_OBSCURED
        }
        else if (activeAlert) {
            systemContext = Common.SystemContext.SYSCTXT_ALERT
        }
        else if (activeVR) {
            systemContext = Common.SystemContext.SYSCTXT_VRSESSION
        }
        else if (contentLoader.item !== null) {
            systemContext = contentLoader.item.systemContext
        }
        console.debug("exit")
    }
    property MusicSourceModel musicSourceModel: MusicSourceModel {
    }

    property SettingsSourceModel settingsSourceModel: SettingsSourceModel {
    }
}

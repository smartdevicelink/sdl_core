import QtQuick 2.0
import QtQuick.Controls 1.1
import "."

Rectangle {
    id: root
    color: "gray"
    width: parent.width
    height: Style.componentHeight
    anchors.margins: 10

    //OnPresetsChanged
    property var presetsChanged: {
        "preset0": "DEFAULT",
        "preset1": "DEFAULT",
        "preset2": "DEFAULT",
        "preset3": "DEFAULT",
        "preset4": "DEFAULT",
        "preset5": "DEFAULT"
    }

    //TuneRadio
        //RadioStation
    property var radioStation: {
        "frequency": 105,
        "fraction": 3,
        "availableHDs": 5,
        "currentHD": 2
    }

    //OnRadioDetails
        //EventDetails
    property var eventDetails: {
        "eventName": "DEFAULT",
        "phoneNumber": "DEFAULT",
        "price": 5.5,
        "eventTime": {
            "hours": 0,
            "minutes": 0,
            "seconds": 0,
            "year": 0,
            "month": 0,
            "day": 0,
            "TZD": 0
        },
        "location": root.location
    }
        //Advertisement
    property var advertisement: {
        "productName": "DEFAULT",
        "companyName": "DEFAULT",
        "phoneNumber": root.eventDetails.phoneNumber,
        "location": root.location
    }
        //WebActivity
    property var webActivity: {
        "url": "DEFAULT",
        "actionCode": 0
    }
        //SongInfo
    property var songInfo: {
        "name": "DEFAULT",
        "artist": "DEFAULT",
        "genre": "DEFAULT",
        "album": "DEFAULT",
        "year": "DEFAULT",
        "duration": 0
    }

    property var location: {
        "address": {
            "state": "DEFAULT",
            "zipcode": "DEFAULT",
            "city": "DEFAULT",
            "street": "DEFAULT"
        },
        "gpsCoordinates": "DEFAULT"
    }

    ScrollView {

        width: parent.width
        height: parent.height

        Row {

            id: apiRow

            spacing: 20

            Column {

                id: requestsColumn

                Button {
                    id: controlButton
                    width: Style.buttonWidth
                    text: "OnControlChanged"
                    objectName: text
                    onClicked: requestButtonClick(this)
                }

                Button {
                    id: presetButton
                    width: Style.buttonWidth
                    text: "OnPresetsChanged"
                    objectName: text
                    onClicked: requestButtonClick(this)
                }

                Button {
                    id: radioDetailsButton
                    width: Style.buttonWidth
                    text: "OnRadioDetails"
                    objectName: text
                    onClicked: requestButtonClick(this)
                }

                Button {
                    id: playerDetailsButton
                    width: Style.buttonWidth
                    text: "OnPlayerDetails"
                    objectName: text
                    onClicked: requestButtonClick(this)
                }

            }
            Column {

                id: responsesColumn

                Button {
                    id: accessButton
                    width: Style.buttonWidth
                    text: "GrantAccess"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: cancelButton
                    width: Style.buttonWidth
                    text: "CancelAccess"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: tuneUpButton
                    width: Style.buttonWidth
                    text: "TuneUp"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: tuneDownButton
                    width: Style.buttonWidth
                    text: "TuneDown"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: tuneButton
                    width: Style.buttonWidth
                    text: "TuneRadio"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: startScan
                    width: Style.buttonWidth
                    text: "StartScan"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: stopScan
                    width: Style.buttonWidth
                    text: "StopScan"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: prevTrackButton
                    width: Style.buttonWidth
                    text: "PrevTrack"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: playTrackButton
                    width: Style.buttonWidth
                    text: "PlayTrack"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                Button {
                    id: nextTrackButton
                    width: Style.buttonWidth
                    text: "NextTrack"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }
            }
            Column {
                id: dispayedItems

                spacing: 2

                Rectangle {

                    border.color: "orange"
                    width: tuneRadioContainer.width + 20
                    height: tuneRadioContainer.height + 20
                    color: Qt.rgba(0.24, 0.22, 0.53)
                    radius: Style.radius

                    Column {

                        id: tuneRadioContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: frequencyBox

                            dataText: root.radioStation.frequency
                            textLabel: "frequency:"
                        }

                        LabelText {
                            id: fractionBox

                            dataText: root.radioStation.fraction
                            textLabel: "fraction:"
                        }

                        LabelText {
                            id: availableHDs

                            dataText: root.radioStation.availableHDs
                            textLabel: "availableHDs:"
                        }

                        LabelText {
                            id: currentHD

                            dataText: root.radioStation.currentHD
                            textLabel: "currentHD:"
                        }
                    }
                }

                Rectangle {

                    border.color: "white"
                    width: presetsContainer.width + 20
                    height: presetsContainer.height + 20
                    color: Qt.rgba(0.22, 0.53, 0.498)
                    radius: Style.radius

                    Column {

                        id: presetsContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: preset0

                            dataText: root.presetsChanged.preset0
                            textLabel: "preset0:"
                        }

                        LabelText {
                            id: preset1

                            dataText: root.presetsChanged.preset1
                            textLabel: "preset1:"
                        }

                        LabelText {
                            id: preset2

                            dataText: root.presetsChanged.preset2
                            textLabel: "preset2:"
                        }

                        LabelText {
                            id: preset3

                            dataText: root.presetsChanged.preset3
                            textLabel: "preset3:"
                        }

                        LabelText {
                            id: preset4

                            dataText: root.presetsChanged.preset4
                            textLabel: "preset4:"
                        }

                        LabelText {
                            id: preset5

                            dataText: root.presetsChanged.preset5
                            textLabel: "preset5:"
                        }
                    }
                }

                Rectangle {

                    border.color: "blue"
                    width: eventDetailsContainer.width + 20
                    height: eventDetailsContainer.height + 20
                    color: Qt.rgba(0.53, 0.22, 0.51)
                    radius: Style.radius

                    Column {

                        id: eventDetailsContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: eventName

                            dataText: root.eventDetails.eventName
                            textLabel: "eventName:"
                        }

                        LabelText {
                            id: phoneNumber

                            dataText: root.eventDetails.phoneNumber
                            textLabel: "phoneNumber:"
                        }

                        LabelText {
                            id: price

                            dataText: root.eventDetails.price
                            textLabel: "price:"
                        }

                        LabelText {
                            id: hours

                            dataText: root.eventDetails.eventTime.hours
                            textLabel: "hours:"
                        }

                        LabelText {
                            id: minutes

                            dataText: root.eventDetails.eventTime.minutes
                            textLabel: "minutes:"
                        }

                        LabelText {
                            id: seconds

                            dataText: root.eventDetails.eventTime.seconds
                            textLabel: "seconds:"
                        }

                        LabelText {
                            id: year

                            dataText: root.eventDetails.eventTime.year
                            textLabel: "year:"
                        }

                        LabelText {
                            id: month

                            dataText: root.eventDetails.eventTime.month
                            textLabel: "month:"
                        }

                        LabelText {
                            id: day

                            dataText: root.eventDetails.eventTime.day
                            textLabel: "day:"
                        }

                        LabelText {
                            id: tzd

                            dataText: root.eventDetails.eventTime.TZD
                            textLabel: "TZD:"
                        }
                    }
                }

                Rectangle {

                    border.color: "orange"
                    width: advertisementContainer.width + 20
                    height: advertisementContainer.height + 20
                    color: Qt.rgba(0.24, 0.22, 0.53)
                    radius: Style.radius

                    Column {

                        id: advertisementContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: productName

                            dataText: root.advertisement.productName
                            textLabel: "productName:"
                        }

                        LabelText {
                            id: companyName

                            dataText: root.advertisement.companyName
                            textLabel: "companyName:"
                        }
                    }
                }

                Rectangle {

                    border.color: "orange"
                    width: webActivityContainer.width + 20
                    height: webActivityContainer.height + 20
                    color: Qt.rgba(0.22, 0.53, 0.498)
                    radius: Style.radius

                    Column {

                        id: webActivityContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: url

                            dataText: root.webActivity.url
                            textLabel: "url:"
                        }

                        LabelText {
                            id: actionCode

                            dataText: root.webActivity.actionCode
                            textLabel: "actionCode:"
                        }
                    }
                }

                Rectangle {

                    border.color: "orange"
                    width: songInfoContainer.width + 20
                    height: songInfoContainer.height + 20
                    color: Qt.rgba(0.53, 0.22, 0.51)
                    radius: Style.radius

                    Column {

                        id: songInfoContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: name

                            dataText: root.songInfo.name
                            textLabel: "name:"
                        }

                        LabelText {
                            id: artist

                            dataText: root.songInfo.artist
                            textLabel: "artist:"
                        }

                        LabelText {
                            id: genre

                            dataText: root.songInfo.genre
                            textLabel: "genre:"
                        }

                        LabelText {
                            id: album

                            dataText: root.songInfo.album
                            textLabel: "album:"
                        }

                        LabelText {
                            id: songYear

                            dataText: root.songInfo.year
                            textLabel: "year:"
                        }

                        LabelText {
                            id: duration

                            dataText: root.songInfo.duration
                            textLabel: "duration:"
                        }
                    }
                }

                Rectangle {

                    border.color: "orange"
                    width: locationContainer.width + 20
                    height: locationContainer.height + 20
                    color: Qt.rgba(0.31, 0.53, 0.22)
                    radius: Style.radius

                    Column {

                        id: locationContainer
                        anchors.margins: 10
                        anchors.topMargin: 10
                        anchors.top: parent.top

                        spacing: 10

                        LabelText {
                            id: state

                            dataText: root.location.address.state
                            textLabel: "state:"
                        }

                        LabelText {
                            id: zipcode

                            dataText: root.location.address.state
                            textLabel: "zipcode:"
                        }

                        LabelText {
                            id: city

                            dataText: root.location.address.state
                            textLabel: "city:"
                        }

                        LabelText {
                            id: street

                            dataText: root.location.address.state
                            textLabel: "street:"
                        }

                        LabelText {
                            id: gpsCoordinates

                            dataText: root.location.gpsCoordinates
                            textLabel: "gpsCoordinates:"
                        }
                    }
                }
            }
        }
   }

    signal requestButtonClick(QtObject item)

    onRequestButtonClick: {
        tcpLogsView.textColor = "green"
        tcpLogsView.append("Request " + item.objectName + " send")
    }

    signal responseButtonClick(QtObject item)

    onResponseButtonClick: {
        tcpLogsView.textColor = "red"
        tcpLogsView.append("Response " + item.objectName + " send")
    }
}

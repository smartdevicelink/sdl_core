import QtQuick 2.0
import QtQuick.Controls 1.1
import "."

Rectangle {
    id: root
    color: "gray"
    width: parent.width
    height: Style.componentHeight
    anchors.margins: 10

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

                            dataText: RequestHandler.frequency
                            textLabel: "frequency:"
                            Connections {
                                target: RequestHandler
                                onFrequencyChanged: {
                                    frequencyBox.dataText = RequestHandler.frequency
                                }
                            }
                            onDataTextChanged: {
                                RequestHandler.frequency = parseInt(dataText)
                            }
                        }

                        LabelText {
                            id: fractionBox

                            dataText: RequestHandler.fraction
                            textLabel: "fraction:"
                            onDataTextChanged: {
                                RequestHandler.fraction = parseInt(dataText)
                            }
                            Connections {
                                target: RequestHandler
                                onFractionChanged: {
                                    fractionBox.dataText = RequestHandler.fraction
                                }
                            }
                        }

                        LabelText {
                            id: availableHDs

                            dataText: RequestHandler.availableHDs
                            textLabel: "availableHDs:"
                            onDataTextChanged: {
                                RequestHandler.availableHDs = parseInt(dataText)
                            }
                            Connections {
                                target: RequestHandler
                                onAvailableHDsChanged: {
                                    availableHDs.dataText = RequestHandler.availableHDs
                                }
                            }
                        }

                        LabelText {
                            id: currentHD

                            dataText: RequestHandler.currentHD
                            textLabel: "currentHD:"
                            onDataTextChanged: {
                                RequestHandler.currentHD = parseInt(dataText)
                            }
                            Connections {
                                target: RequestHandler
                                onCurrentHDChanged: {
                                    currentHD.dataText = RequestHandler.currentHD
                                }
                            }
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

                            dataText: RequestHandler.preset0
                            textLabel: "preset0:"
                            onDataTextChanged: {
                                RequestHandler.preset0 = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPreset0Changed: {
                                    preset0.dataText = RequestHandler.preset0
                                }
                            }
                        }

                        LabelText {
                            id: preset1

                            dataText: RequestHandler.preset1
                            textLabel: "preset1:"
                            onDataTextChanged: {
                                RequestHandler.preset1 = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPreset1Changed: {
                                    preset1.dataText = RequestHandler.preset1
                                }
                            }
                        }

                        LabelText {
                            id: preset2

                            dataText: RequestHandler.preset2
                            textLabel: "preset2:"
                            onDataTextChanged: {
                                RequestHandler.preset2 = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPreset2Changed: {
                                    preset2.dataText = RequestHandler.preset2
                                }
                            }
                        }

                        LabelText {
                            id: preset3

                            dataText: RequestHandler.preset3
                            textLabel: "preset3:"
                            onDataTextChanged: {
                                RequestHandler.preset3 = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPreset3Changed: {
                                    preset3.dataText = RequestHandler.preset3
                                }
                            }
                        }

                        LabelText {
                            id: preset4

                            dataText: RequestHandler.preset4
                            textLabel: "preset4:"
                            onDataTextChanged: {
                                RequestHandler.preset4 = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPreset4Changed: {
                                    preset4.dataText = RequestHandler.preset4
                                }
                            }
                        }

                        LabelText {
                            id: preset5

                            dataText: RequestHandler.preset5
                            textLabel: "preset5:"
                            onDataTextChanged: {
                                RequestHandler.preset5 = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPreset5Changed: {
                                    preset5.dataText = RequestHandler.preset5
                                }
                            }
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

                            dataText: RequestHandler.eventName
                            textLabel: "eventName:"
                            onDataTextChanged: {
                                RequestHandler.eventName = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onEventNameChanged: {
                                    eventName.dataText = RequestHandler.eventName
                                }
                            }
                        }

                        LabelText {
                            id: phoneNumber

                            dataText: RequestHandler.phoneNumber
                            textLabel: "phoneNumber:"
                            onDataTextChanged: {
                                RequestHandler.phoneNumber = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPhoneNumberChanged: {
                                    phoneNumber.dataText = RequestHandler.phoneNumber
                                }
                            }
                        }

                        LabelText {
                            id: price

                            dataText: RequestHandler.price
                            textLabel: "price:"
                            onDataTextChanged: {
                                RequestHandler.price = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onPriceChanged: {
                                    price.dataText = RequestHandler.price
                                }
                            }
                        }

                        LabelText {
                            id: hours

                            dataText: RequestHandler.hours
                            textLabel: "hours:"
                            onDataTextChanged: {
                                RequestHandler.hours = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onHoursChanged: {
                                    hours.dataText = RequestHandler.hours
                                }
                            }
                        }

                        LabelText {
                            id: minutes

                            dataText: RequestHandler.minutes
                            textLabel: "minutes:"
                            onDataTextChanged: {
                                RequestHandler.minutes = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onMinutesChanged: {
                                    minutes.dataText = RequestHandler.minutes
                                }
                            }
                        }

                        LabelText {
                            id: seconds

                            dataText: RequestHandler.seconds
                            textLabel: "seconds:"
                            onDataTextChanged: {
                                RequestHandler.seconds = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onSecondsChanged: {
                                    seconds.dataText = RequestHandler.seconds
                                }
                            }
                        }

                        LabelText {
                            id: year

                            dataText: RequestHandler.year
                            textLabel: "year:"
                            onDataTextChanged: {
                                RequestHandler.year = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onYearChanged: {
                                    year.dataText = RequestHandler.year
                                }
                            }
                        }

                        LabelText {
                            id: month

                            dataText: RequestHandler.month
                            textLabel: "month:"
                            onDataTextChanged: {
                                RequestHandler.month = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onMonthChanged: {
                                    month.dataText = RequestHandler.month
                                }
                            }
                        }

                        LabelText {
                            id: day

                            dataText: RequestHandler.day
                            textLabel: "day:"
                            onDataTextChanged: {
                                RequestHandler.day = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onDayChanged: {
                                    day.dataText = RequestHandler.day
                                }
                            }
                        }

                        LabelText {
                            id: tzd

                            dataText: RequestHandler.tzd
                            textLabel: "TZD:"
                            onDataTextChanged: {
                                RequestHandler.tzd = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onTzdChanged: {
                                    tzd.dataText = RequestHandler.tzd
                                }
                            }
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

                            dataText: RequestHandler.productName
                            textLabel: "productName:"
                            onDataTextChanged: {
                                RequestHandler.productName = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onProductNameChanged: {
                                    productName.dataText = RequestHandler.productName
                                }
                            }
                        }

                        LabelText {
                            id: companyName

                            dataText: RequestHandler.companyName
                            textLabel: "companyName:"
                            onDataTextChanged: {
                                RequestHandler.companyName = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onCompanyNameChanged: {
                                    companyName.dataText = RequestHandler.companyName
                                }
                            }
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

                            dataText: RequestHandler.url
                            textLabel: "url:"
                            onDataTextChanged: {
                                RequestHandler.url = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onUrlChanged: {
                                    url.dataText = RequestHandler.url
                                }
                            }
                        }

                        LabelText {
                            id: actionCode

                            dataText: RequestHandler.actionCode
                            textLabel: "actionCode:"
                            onDataTextChanged: {
                                RequestHandler.actionCode = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onActionCodeChanged: {
                                    actionCode.dataText = RequestHandler.actionCode
                                }
                            }
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

                            dataText: RequestHandler.name
                            textLabel: "name:"
                            onDataTextChanged: {
                                RequestHandler.name = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onNameChanged: {
                                    name.dataText = RequestHandler.name
                                }
                            }
                        }

                        LabelText {
                            id: artist

                            dataText: RequestHandler.artist
                            textLabel: "artist:"
                            onDataTextChanged: {
                                RequestHandler.artist = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onArtistChanged: {
                                    artist.dataText = RequestHandler.artist
                                }
                            }
                        }

                        LabelText {
                            id: genre

                            dataText: RequestHandler.genre
                            textLabel: "genre:"
                            onDataTextChanged: {
                                RequestHandler.genre = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onGenreChanged: {
                                    genre.dataText = RequestHandler.genre
                                }
                            }
                        }

                        LabelText {
                            id: album

                            dataText: RequestHandler.album
                            textLabel: "album:"
                            onDataTextChanged: {
                                RequestHandler.album = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onAlbumChanged: {
                                    album.dataText = RequestHandler.album
                                }
                            }
                        }

                        LabelText {
                            id: songYear

                            dataText: RequestHandler.songYear
                            textLabel: "year:"
                            onDataTextChanged: {
                                RequestHandler.songYear = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onSongYearChanged: {
                                    songYear.dataText = RequestHandler.songYear
                                }
                            }
                        }

                        LabelText {
                            id: duration

                            dataText: RequestHandler.duration
                            textLabel: "duration:"
                            onDataTextChanged: {
                                RequestHandler.duration = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onDurationChanged: {
                                    duration.dataText = RequestHandler.duration
                                }
                            }
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

                            dataText: RequestHandler.state
                            textLabel: "state:"
                            onDataTextChanged: {
                                RequestHandler.state = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onStateChanged: {
                                    state.dataText = RequestHandler.state
                                }
                            }
                        }

                        LabelText {
                            id: zipcode

                            dataText: RequestHandler.zipcode
                            textLabel: "zipcode:"
                            onDataTextChanged: {
                                RequestHandler.zipcode = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onZipcodeChanged: {
                                    zipcode.dataText = RequestHandler.zipcode
                                }
                            }
                        }

                        LabelText {
                            id: city

                            dataText: RequestHandler.city
                            textLabel: "city:"
                            onDataTextChanged: {
                                RequestHandler.city = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onCityChanged: {
                                    city.dataText = RequestHandler.city
                                }
                            }
                        }

                        LabelText {
                            id: street

                            dataText: RequestHandler.street
                            textLabel: "street:"
                            onDataTextChanged: {
                                RequestHandler.street = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onStreetChanged: {
                                    street.dataText = RequestHandler.street
                                }
                            }
                        }

                        LabelText {
                            id: gpsCoordinates

                            dataText: RequestHandler.gpsCoordinates
                            textLabel: "gpsCoordinates:"
                            onDataTextChanged: {
                                RequestHandler.gpsCoordinates = dataText
                            }
                            Connections {
                                target: RequestHandler
                                onGpsCoordinatesChanged: {
                                    gpsCoordinates.dataText = RequestHandler.gpsCoordinates
                                }
                            }
                        }
                    }
                }
            }
        }
   }

    signal requestButtonClick(QtObject item)

    signal responseButtonClick(QtObject item)

    onResponseButtonClick: {



        tcpLogsView.textColor = "red"
        tcpLogsView.append("Response " + item.objectName + " send")
    }
}

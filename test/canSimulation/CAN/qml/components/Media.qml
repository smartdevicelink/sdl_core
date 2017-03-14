import QtQuick 2.0
import QtQuick.Controls 1.1
import "."

Rectangle {
    id: root
    width: parent.width
    height: Style.componentHeight
    anchors.margins: Style.margins

    signal requestButtonClick(QtObject item)

    signal responseButtonClick(QtObject item)

    gradient: Gradient {
        GradientStop { position: 0 ; color: Style.colorMainDarkGradient}
        GradientStop { position: 0.6 ; color: Style.colorMainLightGradient}
        GradientStop { position: 0.7 ; color: Style.colorMainLightGradient}
        GradientStop { position: 1 ; color: Style.colorMainDarkGradient}
    }

    ScrollView {

        width: parent.width
        height: parent.height

        Row {

            id: apiRow
            spacing: Style.rowSpacing

            /**
             * Notification buttons column
             */
            Column {

                id: requestsColumn
                spacing: Style.rowSpacing

                CastomButton {
                    id: radioDetailsButton
                    width: Style.buttonWidth
                    text: "OnRadioDetails"
                    objectName: text
                    onClicked: requestButtonClick(this)
                }

                CastomButton {
                    id: playerDetailsButton
                    width: Style.buttonWidth
                    text: "OnPlayerDetails"
                    objectName: text
                    onClicked: requestButtonClick(this)
                }
            }

            /**
             * Media buttons column
             */
            Column {

                id: responsesColumn
                spacing: Style.rowSpacing

                CastomButton {
                    id: tuneUpButton
                    width: Style.buttonWidth
                    text: "TuneUp"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: tuneDownButton
                    width: Style.buttonWidth
                    text: "TuneDown"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: tuneButton
                    width: Style.buttonWidth
                    text: "TuneRadio"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: startScan
                    width: Style.buttonWidth
                    text: "StartScan"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: stopScan
                    width: Style.buttonWidth
                    text: "StopScan"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: prevTrackButton
                    width: Style.buttonWidth
                    text: "PrevTrack"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: playTrackButton
                    width: Style.buttonWidth
                    text: "PlayTrack"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }

                CastomButton {
                    id: nextTrackButton
                    width: Style.buttonWidth
                    text: "NextTrack"
                    objectName: text
                    onClicked: responseButtonClick(this)
                }
            }

            /**
             * Column with API data structures
             */
            Column {
                id: dispayedItems

                spacing: Style.rowSpacing

                Rectangle {

                    id: tuneRadioRectangle

                    border.color: Style.colorLabelTextBackground
                    width: tuneRadioContainer.width + Style.dataStructureContainerMargin
                    height: tuneRadioContainer.height + Style.dataStructureContainerMargin

                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: tuneRadioContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top
                        spacing: Style.columnSpacing

                        Text {
                            text: "RadioStation"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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

                    border.color: Style.colorLabelTextBackground
                    width: presetsContainer.width + Style.dataStructureContainerMargin
                    height: presetsContainer.height + Style.dataStructureContainerMargin

                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: presetsContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top

                        spacing: Style.columnSpacing

                        Text {
                            text: "CustomPresets"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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

                    border.color: Style.colorLabelTextBackground
                    width: eventDetailsContainer.width + Style.dataStructureContainerMargin
                    height: eventDetailsContainer.height + Style.dataStructureContainerMargin
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: eventDetailsContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top

                        spacing: Style.columnSpacing

                        Text {
                            text: "EventDetails"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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

                    border.color: Style.colorLabelTextBackground
                    width: advertisementContainer.width + Style.dataStructureContainerMargin
                    height: advertisementContainer.height + Style.dataStructureContainerMargin
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: advertisementContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top

                        spacing: Style.columnSpacing

                        Text {
                            text: "Advertisement"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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

                    border.color: Style.colorLabelTextBackground
                    width: webActivityContainer.width + Style.dataStructureContainerMargin
                    height: webActivityContainer.height + Style.dataStructureContainerMargin
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: webActivityContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top

                        spacing: Style.columnSpacing

                        Text {
                            text: "WebActivity"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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

                    border.color: Style.colorLabelTextBackground
                    width: songInfoContainer.width + Style.dataStructureContainerMargin
                    height: songInfoContainer.height + Style.dataStructureContainerMargin
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: songInfoContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top

                        spacing: Style.columnSpacing

                        Text {
                            text: "SongInfo"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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

                    border.color: Style.colorLabelTextBackground
                    width: locationContainer.width + Style.dataStructureContainerMargin
                    height: locationContainer.height + Style.dataStructureContainerMargin
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.4 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 0.5 ; color: Style.colorButtonGradTop }
                        GradientStop { position: 1 ; color: Style.colorButtonGradBot }
                    }
                    radius: Style.radius

                    Column {

                        id: locationContainer
                        anchors.margins: Style.margins
                        anchors.top: parent.top

                        spacing: Style.columnSpacing

                        Text {
                            text: "Location"
                            color: Style.colorButtonText
                            width: parent.width
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

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
}

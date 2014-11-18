pragma Singleton
import QtQuick 2.0

Item {
    id: dataHandler

    //OnPresetsChanged
    property string preset0: "DEFAULT";
    property string preset1: "DEFAULT";
    property string preset2: "DEFAULT";
    property string preset3: "DEFAULT";
    property string preset4: "DEFAULT";
    property string preset5: "DEFAULT";

    //TuneRadio
        //RadioStation
    property int frequency: 105;
    property int fraction: 3;
    property int availableHDs: 5;
    property int currentHD: 2;

    //OnRadioDetails
        //EventDetails
    property string eventName: "DEFAULT";
    property string phoneNumber: "DEFAULT";
    property real price: 5.5;
    property int hours: 0;
    property int minutes: 0;
    property int seconds: 0;
    property int year: 0;
    property int month: 0;
    property int day: 0;
    property real tzd: 0;
        //Advertisement
    property string productName: "DEFAULT";
    property string companyName: "DEFAULT";
        //WebActivity
    property string url: "DEFAULT";
    property string actionCode: "DEFAULT";
        //SongInfo
    property string name: "DEFAULT";
    property string artist: "DEFAULT";
    property string genre: "DEFAULT";
    property string album: "DEFAULT";
    property string songYear: "DEFAULT";
    property string duration: "DEFAULT";
        //location
    property string state: "DEFAULT";
    property string zipcode: "DEFAULT";
    property string city: "DEFAULT";
    property string street: "DEFAULT";
    property string gpsCoordinates: "DEFAULT";

    Timer {
        id: tuneUpTimer
        interval: 100;
        running: false;
        repeat: true
        onTriggered: tuneUp()
    }


    function receivedMessage(result) {

        switch (result.method) {
            case "CAN.TuneRadio": {

                if (result.params.radioStation.frequency) {
                    dataHandler.frequency = result.params.radioStation.frequency
                    console.log("RequestHandler receivedMessage radioStation.frequency", result.params.radioStation.frequency);
                    console.log("RequestHandler dataHandler frequency", dataHandler.frequency);
                }
                if (result.params.radioStation.fraction) {
                    dataHandler.fraction = result.params.radioStation.fraction
                }
                if (result.params.radioStation.availableHDs) {
                    dataHandler.availableHDs = result.params.radioStation.availableHDs
                }
                if (result.params.radioStation.currentHD) {
                    dataHandler.currentHD = result.params.radioStation.currentHD
                }

                break;
            }
            case "CAN.TuneUp": {

                tuneUp();

                break;
            }
            case "CAN.TuneDown": {

                tuneDown();

                break;
            }
            case "CAN.StartScan": {

                startScan();

                break;
            }
            case "CAN.StopScan": {

                stopScan();

                break;
            }
        }

        return successResponse(result.id, result.method);
    }

    function tuneUp() {

        dataHandler.fraction += 2;
        if (dataHandler.fraction == 10) {
            dataHandler.fraction = 0;
            dataHandler.frequency += 1;
        } else if (dataHandler.fraction == 11) {
            dataHandler.fraction = 1;
            dataHandler.frequency += 1;
        }

        if (dataHandler.frequency == 108) {
            dataHandler.frequency = 87;
            dataHandler.fraction = 8;
        }
    }

    function tuneDown() {

        dataHandler.fraction -= 2;
        if (dataHandler.fraction == -2) {
            dataHandler.fraction = 8;
            dataHandler.frequency -= 1;
        } else if (dataHandler.fraction == -1) {
            dataHandler.fraction = 9;
            dataHandler.frequency -= 1;
        }

        if (dataHandler.frequency == 87 && dataHandler.fraction < 8) {
            dataHandler.frequency = 108;
            dataHandler.fraction = 0;
        }
    }

    function startScan() {
        tuneUpTimer.start();
    }

    function stopScan() {
        tuneUpTimer.stop();
    }

    function successResponse (id, method) {

        var response;

        response = {
            "id": id,
            "jsonrpc": "2.0",
            "result": {
                "method": method,
                "code": 0
            }
        }

        return response;
    }

    function onRadioDetailsFractionChanged() {

        var response;

        response = {
            "jsonrpc": "2.0",
            "method": "CAN.OnRadioDetails",
            "params": {
                "radioStation":{
                    "frequency": dataHandler.frequency,
                    "fraction": dataHandler.fraction
                }
            }
        }

        return response;
    }

    function onRadioDetails()
    {
        var response;

        response = {
            "jsonrpc": "2.0",
            "method": "CAN.OnRadioDetails",
            "params": {
                "radioStation":{
                    "frequency": dataHandler.frequency,
                    "fraction": dataHandler.fraction,
                    "availableHDs": dataHandler.availableHDs,
                    "currentHD": dataHandler.currentHD
                },
                "event":{
                    "eventName": dataHandler.eventName,
                    "phoneNumber": dataHandler.phoneNumber,
                    "price": dataHandler.price,
                    "eventTime": {
                        "hours": dataHandler.hours,
                        "minutes": dataHandler.minutes,
                        "seconds": dataHandler.seconds,
                        "year": dataHandler.year,
                        "month": dataHandler.month,
                        "day": dataHandler.day,
                        "tzd": dataHandler.tzd
                    },
                    "location": {
                        "address": {
                            "state": dataHandler.state,
                            "zipcode": dataHandler.zipcode,
                            "city": dataHandler.city,
                            "street": dataHandler. street
                        },
                        "gpsCoordinates": dataHandler.gpsCoordinates
                    }
                },
                "advertisement":{
                    "productName": dataHandler.productName,
                    "companyName": dataHandler.companyName,
                    "phoneNumber": dataHandler.phoneNumber,
                    "location": {
                        "address": {
                            "state": dataHandler.state,
                            "zipcode": dataHandler.zipcode,
                            "city": dataHandler.city,
                            "street": dataHandler. street
                        },
                        "gpsCoordinates": dataHandler.gpsCoordinates
                    }

                },
                "activity":{
                    "url": dataHandler.url,
                    "actionCode": dataHandler.actionCode
                },
                "songInfo":{
                    "name": dataHandler.name,
                    "artist": dataHandler.artist,
                    "genre": dataHandler.genre,
                    "year": dataHandler.songYear,
                    "duration": dataHandler.duration

                },
                "location":{
                    "address": {
                        "state": dataHandler.state,
                        "zipcode": dataHandler.zipcode,
                        "city": dataHandler.city,
                        "street": dataHandler. street
                    },
                    "gpsCoordinates": dataHandler.gpsCoordinates
                }
            }
        }

        return response;
    }
}

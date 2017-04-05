pragma Singleton
import QtQuick 2.0

Item {
    id: dataHandler


    signal sendMessageHMI(variant message);
    signal sendMessageSDL(variant message);

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
    property string tzd: "DEFAULT";
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
    property int duration: 35;
        //location
    property string state: "DEFAULT";
    property string zipcode: "DEFAULT";
    property string city: "DEFAULT";
    property string street: "DEFAULT";
    property string gpsCoordinates: "DEFAULT";

    property var stationsData: {
        "1001": {
                    "radioStation": {
                        "frequency": 100,
                        "fraction": 1,
                        "availableHDs": 0,
                        "currentHD": 0
                    },
                    "songInfo": {
                        "name": "Song2",
                        "artist": "Artist2",
                        "genre": "Genre2",
                        "album": "Album2",
                        "year": 2002,
                        "duration": 20
                    }
                },
        "895":  {
                    "radioStation": {
                        "frequency": 89,
                        "fraction": 5,
                        "availableHDs": 0,
                        "currentHD": 0
                    },
                    "songInfo": {
                        "name": "Song1",
                        "artist": "Artist1",
                        "genre": "Genre1",
                        "album": "Album1",
                        "year": 2001,
                        "duration": 10
                    }
                },
        "953":  {
                    "radioStation": {
                        "frequency": 95,
                        "fraction": 3,
                        "availableHDs": 0,
                        "currentHD": 0
                    },
                    "songInfo": {
                        "name": "Song3",
                        "artist": "Artist3",
                        "genre": "Genre3",
                        "album": "Album3",
                        "year": 2003,
                        "duration": 30
                    }
                },
        "1035":  {
                    "radioStation": {
                        "frequency": 103,
                        "fraction": 5,
                        "availableHDs": 0,
                        "currentHD": 0
                    },
                    "songInfo": {
                        "name": "Song4",
                        "artist": "Artist4",
                        "genre": "Genre4",
                        "album": "Album4",
                        "year": 2004,
                        "duration": 40
                    }
                },
        "1053":  {
                    "radioStation": {
                        "frequency": 105,
                        "fraction": 3,
                        "availableHDs": 0,
                        "currentHD": 0
                    },
                    "songInfo": {
                        "name": "Song5",
                        "artist": "Artist5",
                        "genre": "Genre5",
                        "album": "Album5",
                        "year": 2005,
                        "duration": 50
                    }
                }
    }

    Timer {
        id: tuneUpTimer

        // Specify a timeout interval of 150 miliseconds for scan interval emulation
        interval: 150;
        running: false;
        repeat: true
        onTriggered: tuneUp()
    }

    Timer {
        id: pauseTimer

        // Specify a timeout interval of 3 seconds for Scan emulation
        interval: 3000

        onTriggered: {
            tuneUpTimer.start();
            pauseTimer.stop();
        }
    }

    /**
     * Method to process requsets and notifications came from TCP client
     * @param result - Incoming message from TCP
     */
    function receivedMessageTCP(result) {


        var notify, response;

        //Requsets parser by method name
        switch (result.method) {
            case "CAN.TuneRadio": {

                if ("frequency" in result.params.radioStation && result.params.radioStation.frequency <= 108 && result.params.radioStation.frequency >= 87) {
                    dataHandler.frequency = result.params.radioStation.frequency
                }
                if ("availableHDs" in result.params.radioStation) {
                    dataHandler.availableHDs = result.params.radioStation.availableHDs
                }
                if ("currentHD" in result.params.radioStation) {
                    dataHandler.currentHD = result.params.radioStation.currentHD
                }
                if ("fraction" in result.params.radioStation && result.params.radioStation.fraction <= 9  && result.params.radioStation.fraction >= 0) {
                    dataHandler.fraction = result.params.radioStation.fraction
                }

                checkRadioDetailsSongInfo();

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

                notify = {
                    "jsonrpc": "2.0",
                    "method": "CAN.StartScan"
                }

                sendMessageHMI(notify);

                startScan();

                break;
            }
            case "CAN.StopScan": {

                notify = {
                    "jsonrpc": "2.0",
                    "method": "CAN.StopScan"
                }

                sendMessageHMI(notify);

                stopScan();

                break;
            }
            case "CAN.GetSeatControl": {

                //TO DO
                //Add handler for this request

                break;
            }
            case "CAN.ClimateControlOn": {

                response = {
                    "jsonrpc": "2.0",
                    "id": result.id,
                    "result": {
                        "code": 0,
                        "capabilities": {
                            "isRearEnabled": true,
                            "isCooledSeatEnabled": true,
                            "isHeatedSteerWheelEnabled": true
                        },
                        "method": result.method
                    }
                }

                return response;
            }
        }

        return successResponse(result.id, result.method);

    }


    /**
     * Method to process requsets and notifications came from WS client
     * @param result - Incoming message from WS
     */
    function receivedMessageWS(result) {

        switch (result.method) {
            case "CAN.TuneRadio": {

                if ("frequency" in result.params.radioStation && result.params.radioStation.frequency <= 108 && result.params.radioStation.frequency >= 87) {
                    dataHandler.frequency = result.params.radioStation.frequency
                }
                if ("availableHDs" in result.params.radioStation) {
                    dataHandler.availableHDs = result.params.radioStation.availableHDs
                }
                if ("currentHD" in result.params.radioStation) {
                    dataHandler.currentHD = result.params.radioStation.currentHD
                }
                if ("fraction" in result.params.radioStation && result.params.radioStation.fraction <= 9  && result.params.radioStation.fraction >= 0) {
                    dataHandler.fraction = result.params.radioStation.fraction
                }

                checkRadioDetailsSongInfo();

                break;
            }
            case "CAN.TuneUp": {

                tuneUp();

                break;
            }
            case "CAN.OnRadioDetails": {

                if ("frequency" in result.params.radioStation && result.params.radioStation.frequency <= 108 && result.params.radioStation.frequency >= 87) {
                    dataHandler.frequency = result.params.radioStation.frequency
                }
                if ("availableHDs" in result.params.radioStation) {
                    dataHandler.availableHDs = result.params.radioStation.availableHDs
                }
                if ("currentHD" in result.params.radioStation) {
                    dataHandler.currentHD = result.params.radioStation.currentHD
                }
                if ("fraction" in result.params.radioStation && result.params.radioStation.fraction <= 9  && result.params.radioStation.fraction >= 0) {
                    dataHandler.fraction = result.params.radioStation.fraction
                }

                checkRadioDetailsSongInfo();

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
            case "CAN.OnPresetsChanged": {

                presetsChangeWS(result.params);

                break;
            }
        }

        return successResponse(result.id, result.method);
    }

    /**
     * Method to process requsets with data for RadioDetails structure
     * save data to model and send notification OnRadioDetails to WS and TCP
     */
    function checkRadioDetailsSongInfo() {

        var data = dataHandler.fraction + dataHandler.frequency * 10;

        if (dataHandler.stationsData[data]) {

            if (tuneUpTimer.running) {
                tuneUpTimer.stop();
                pauseTimer.start();
            }

            var notification;

            notification = {
                "jsonrpc": "2.0",
                "method": "CAN.OnRadioDetails",
                "params": dataHandler.stationsData[data]
            }

            sendMessageHMI(notification);
            sendMessageSDL(notification);
        } else {

            sendMessageSDL(onRadioDetailsFractionChanged());
            sendMessageHMI(onRadioDetailsFractionChanged());
        }
    }

    /**
     * Method to process requsets data for Presets structure
     * save data to model and send notification onPresetsChanged to WS
     */
    function presetsChangeWS(params) {

        dataHandler.preset0 = params.customPresets[0];
        dataHandler.preset1 = params.customPresets[1];
        dataHandler.preset2 = params.customPresets[2];
        dataHandler.preset3 = params.customPresets[3];
        dataHandler.preset4 = params.customPresets[4];
        dataHandler.preset5 = params.customPresets[5];

        sendMessageSDL(dataHandler.onPresetsChanged(params.customPresets));
    }

    /**
     * Method to process requsets data for Presets structure
     * save data to model and send notification onPresetsChanged to TCP
     */
    function presetsChangeTCP(params) {

        dataHandler.preset0 = params.customPresets[0];
        dataHandler.preset1 = params.customPresets[1];
        dataHandler.preset2 = params.customPresets[2];
        dataHandler.preset3 = params.customPresets[3];
        dataHandler.preset4 = params.customPresets[4];
        dataHandler.preset5 = params.customPresets[5];

        sendMessageHMI(dataHandler.onPresetsChanged(params.customPresets));
    }


    /**
     * Method to emulate TuneUp behavior
     * change data in model and send notification OnRadioDetails to WS and TCP
     */
    function tuneUp() {

        var data = dataHandler.fraction + dataHandler.frequency * 10;

        data += 2;

        if (data >= 1080) {
            data = 879;
        }

        dataHandler.frequency = Math.floor(data / 10);
        dataHandler.fraction = data % 10;

        checkRadioDetailsSongInfo(data);
    }

    /**
     * Method to emulate TuneDoen behavior
     * change data in model and send notification OnRadioDetails to WS and TCP
     */
    function tuneDown() {

        var data = dataHandler.fraction + dataHandler.frequency * 10;

        data -= 2;

        if (data <= 877) {
            data = 1079;
        }

        dataHandler.frequency = Math.floor(data / 10);
        dataHandler.fraction = data % 10;

        checkRadioDetailsSongInfo(data);
    }

    /**
     * Method to emulate Scan behavior
     * change data in model and send notification OnRadioDetails to WS and TCP
     */
    function startScan() {
        tuneUpTimer.start();
    }

    /**
     * Method to stop Scan behavior
     */
    function stopScan() {
        tuneUpTimer.stop();
        pauseTimer.stop();
    }

    function successResponse (id, method) {

        var response;

        if (id) {

            response = {
                "id": id,
                "jsonrpc": "2.0",
                "result": {
                    "method": method,
                    "code": 0
                }
            }
        } else {
            response = false;
        }

        return response;
    }

    /**
     * Method to prepare OnRadioDetails structure to sent to Client
     */
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

    /**
     * Method to prepare OnPresetsChanged structure to sent to Client
     */
    function onPresetsChanged(presets) {

        var response;

        response = {
            "jsonrpc": "2.0",
            "method": "CAN.OnPresetsChanged",
            "params": {
                "customPresets":presets
            }
        }

        return response;
    }

    /**
     * Method to prepare OnRadioDetails structure to sent to Client
     */
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

        sendMessageHMI(response);
        sendMessageSDL(response);
    }
}

var socketURL = "localhost:9092";
var socket;
var Socket = {
    webSocket: null,

    init: function (url) {
        ws = new WebSocket('ws://' + url);

        ws.onopen = function () {
            console.log('Socket opened at ' + url);
            $("#server_status_label").html("running ...");
        };

        ws.onclose = function () {
            console.log('Socket close at ' + url);
            $("#server_status_label").html("stopped");
        };

        ws.onmessage = function (e) {
            //console.log("Socket " + url + " message: " + e.data);
            updateDumpTextArea(e.data);
            parseMessage(e.data);
        };

        this.webSocket = ws;
    },
};

function parseMessage(message) {
    var jsonMessage = JSON.parse(message);
    //console.info("Parse message: " + jsonMessage);
    if (jsonMessage.hasOwnProperty("name") && jsonMessage.hasOwnProperty("value")) {
        switch (jsonMessage.name) {
            case "radio_frequency":
                updateRadioFrequency(jsonMessage.value);
                break;
            case "tuner_band":
                console.info("Band: " + jsonMessage.name + " " + jsonMessage.value);
                updateRadioBand(jsonMessage.value);
                break;
        }
    }
}

function updateRadioBand(radioBandValue) {
    console.info("Tuner band: " + radioBandValue);
    $('#tuner_band_select').val(radioBandValue);
}

function updateRadioFrequency(frequencyValue) {
    console.info("Radio frequency: " + frequencyValue);
    $('#frequency_input_get_id').html(frequencyValue);
}

function updateDumpTextArea(message) {
    var object = $('#dump_text_area');
    object.append(message + "<br/>");
    object.scrollTop(object[0].scrollHeight);
}

function init() {
    setListeners();
    Socket.init(socketURL);
    socket = Socket.webSocket;
}

function setListeners() {
    $('#server_start_btn').on('click', function() {
        console.info("Start server");
        Socket.init(socketURL);
        socket = Socket.webSocket;
    });
    $('#server_stop_btn').on('click', function() {
        console.info("Stop server");
        sendCommand("stop_server");
    });
    $('#station_up_btn_id').on('click', function() {
        console.info("Station +");
        sendCommand("station_up");
    });
    $('#station_down_btn_id').on('click', function() {
        console.info("Station -");
        sendCommand("station_down");
    });
    $('#step_up_btn_id').on('click', function() {
        console.info("Step +");
        sendCommand("step_up");
    });
    $('#step_down_btn_id').on('click', function() {
        console.info("Step -");
        sendCommand("step_down");
    });
    $('#frequency_button_set_id').on('click', function() {
        var val = $("#frequency_input_set_id").val();
        console.info("Set frequency: " + val);
        sendCommand("frequency_set_" + val)
    });
    $('#presets_store_btn_id').on('click', function() {
        var val = $("#presets_store_input_id").val();
        console.info("Store preset " + val);
        sendCommand("store_preset_" + val);
    });
    $('#preset_id_select').change(function() {
        var val = $("#preset_id_select option:selected").val();
        console.info("Preset change to: " + val);
        sendCommand("set_preset_" + val);
    });
    $('#tuner_band_select').change(function() {
        var val = $("#tuner_band_select option:selected").val();
        console.info("Tuner band change to: " + val);
        sendCommand("set_tuner_band_" + val);
    });
}

function sendCommand(command) {
    if (socket) {
        socket.send(command);
    }
}

window.onbeforeunload = function() {
    Socket.onclose = function () {}; // disable onclose handler first
    Socket.close()
};

$(document).ready(function() {
    init();
});
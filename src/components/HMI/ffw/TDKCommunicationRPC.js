/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: ·
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. · Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. · Neither the name of the Ford Motor Company nor the
 * names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
/*
 * TDK Communication RPC Client
 */

FFW.TDKCommunicationRPC = Em.Object.create({

    /*
     * transport layer for messages exchange
     */
    socket: null, // instance of websocket

    /*
     * url for message broker
     */
    url: "ws://localhost:9092",

    /*
     * Open WebSocket and initialize handlers
     */
    connect: function() {

        this.socket = new WebSocket(this.url);

        var self = this;

        this.socket.onopen = function(evt) {

            self.onWSOpen(evt)
        };
        this.socket.onclose = function(evt) {

            self.onWSClose(evt)
        };
        this.socket.onmessage = function(evt) {

            self.onWSMessage(evt)
        };
        this.socket.onerror = function(evt) {

            self.onWSError(evt)
        };

    },

    disconnect: function() {
        this.socket.close();
    },

    /*
     * WebSocket connection is opened
     */
    onWSOpen: function(evt) {
        Em.Logger.log("TDKCommunicationRPC: Connection is opened");
    },

    /*
     * When result is received from RPC server this function is called It
     */
    onWSMessage: function(evt) {

//            Em.Logger.log("TDKCommunicationRPC: Message received " + evt.data);

        var jsonObj = JSON.parse(evt.data);

        if (jsonObj.hasOwnProperty("name") && jsonObj.hasOwnProperty("value")) {
            switch (jsonObj.name) {
                case "radio_frequency":
                    Em.Logger.log("Update radio frequency: " + jsonObj.value);
                    this.updateRadioFrequency(jsonObj.value);
                    break;
                case "tuner_band":
                    Em.Logger.log("Update radio band: " + jsonObj.name + " " + jsonObj.value);
                    this.updateRadioBand(jsonObj.value);
                    break;
            }
        }
    },

    /*
     * WebSocket connection is closed
     */
    onWSClose: function(evt) {

        Em.Logger.log("TDKCommunicationRPC: Connection is closed");
    },

    /*
     * WebSocket connection errors handling
     */
    onWSError: function(evt) {
        Em.Logger.log("TDKCommunicationRPC: ERROR");
    },

    /*
     * Send command via socket connection
     */
    send: function(command) {

        if (this.socket.readyState == this.socket.OPEN) {
            Em.Logger.log("TDKCommunicationRPC: Send command " + command);
            this.socket.send(command);
        } else {
            Em.Logger.error("TDKCommunicationRPC: Can't send message since socket is not ready");
        }
    },

    updateRadioFrequency: function(value) {
        if (SDL.RadioModel.activeBand == 'fm'
            && !SDL.RadioModel.scanState
            && !SDL.RadioModel.tuneRadio
            && Number(SDL.RadioModel.station) != value/1000) {

            var frequencyArray = (value/1000).toString().split('.'),
                frequency = frequencyArray[0],
                fraction = frequencyArray[1];

            SDL.RadioModel.radioDetails.radioStation.frequency = frequency;
            SDL.RadioModel.radioDetails.radioStation.fraction = fraction;
            SDL.RadioModel.set('station', frequency + '.' + fraction);

            SDL.RadioModel.findStationPresets();

            FFW.VehicleInfo.OnRadioDetails({
                radioStation: {
                    frequency: frequency,
                    fraction: fraction
                }
            });
        }
    },

    updateRadioBand: function(value) {
        if (SDL.RadioModel.activeBand != value) {
            SDL.RadioModel.set('activeBand', value);
        }
    }
});
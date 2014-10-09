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
/**
 * @name SDL.RadioModel
 * @desc Radio Media data model
 * @category Model
 * @filesource app/model/media/RadioModel.js
 * @version 1.0
 */

SDL.RadioModel = Em.Object.create( {

    init: function() {

        var i,
            frequency = 87,
            fraction = 9;

        this._super();

        for (i = 879; i <= 1079; i++) {
            this.directTuneItems.push(i.toString().split(''));

            if (fraction < 9) {
                fraction++;
            } else {
                frequency++;
                fraction = 0;
            }
        }

        this.set('active', true);
    },

    active: false,

    station: '87.9',

    activeBand: 'fm',

    activePreset: 0,

    temp: null,

    scanState: false,

    tuneRadio: false,

    preset: [
        "87.9",
        "90.9",
        "105.1",
        "98.5",
        "106.3",
        "107.9"
    ],

    presetMetaData: [
        {
            songInfo: {
                genre: 'Pop',
                name: 'BlUE SKY',
                artist: 'THE MAX'
            }
        },
        {
            songInfo: {
                genre: 'Club',
                name: 'JUMP AND DOWN',
                artist: 'THE PROJECT X'
            }
        },
        {
            songInfo: {
                genre: 'Rock',
                name: 'WELCOME HOME',
                artist: 'TODD SULLIVAN'
            }
        },
        {
            songInfo: {
                genre: 'Pop',
                name: 'LETS DANCE',
                artist: 'MICHAEL JOHNSON'
            }
        },
        {
            songInfo: {
                genre: 'Pop Rock',
                name: 'YESTERDAY NIGHT',
                artist: 'JOHN SMITH'
            }
        },
        {
            songInfo: {
                genre: 'Classic',
                name: 'TENTH SYMPHONY',
                artist: 'SPENCER M.'
            }
        }
    ],

    activeMetaData: function() {
        return this.activePreset != null ? this.presetMetaData[this.activePreset] : null;
    }.property('activePreset'),

    directTuneItems: [],

    directTuneKeyItems: [],

    directTuneFinished: false,

    directTuneKeypressed: false,

    radioDetails:{
        radioStation:{
            frequency: 87,
            fraction: 9,
            availableHDs: 0,
            currentHD: 0
        },
        event:{
            EventName: "String",
            phoneNumber: "String",
            price: 14.7,
            eventTime: {
                hours: 18,
                minutes: 22,
                seconds: 46
            },
            location:{
                address: {
                    state: "String",
                    zipcode: "String",
                    city: "String",
                    street: "String"
                },
                gpsCoordinates: "String"
            }
        },
        advertisement: {
            productName: "String",
            companyName: "String",
            phoneNumber: "String",
            location:{
                address: {
                    state: "String",
                    zipcode: "String",
                    city: "String",
                    street: "String"
                },
                gpsCoordinates: "String"
            }
        },
        activity: {
            url: "String",
            actionCode: 5
        },
        songInfo: {
            name: "String",
            artist: "String",
            genre: "String",
            album: "String",
            year: 2013,
            duration: "String"
        },
        location: {
            address: {
                state: "String",
                zipcode: "String",
                city: "String",
                street: "String"
            },
            gpsCoordinates: "String"
        }
    },

    /**
     * Timer for emulation of scan activity
     */
    scanTimer: null,

    statusBar: 'FM Radio',

    saveStationToPreset: function(element){
        element.set('text', this.station);
        this.preset[element.preset] = this.station;

        SDL.RadioModel.set('activePreset', element.preset);
//        FFW.VehicleInfo.OnPresetsChanged(this.preset);
        FFW.TDKCommunicationRPC.send('frequency_set_' + Number(this.station)*1000);
        FFW.TDKCommunicationRPC.send('store_preset_' + (element.preset + 1));
    },

    directTune: function(){
        this.toggleProperty('tuneRadio');
        if (this.tuneRadio) {
            this.set('temp', this.station);
        } else {
            this.set('station', this.temp);
            this.set('directTuneFinished', false);
        }
    },

    /**
     * Keys for direct tune component
     *
     * @property directTuneKeyItems
     * @return {number}
     */
    directTuneKeys: function() {
        var i, keys = [];

        for ( i = 0; i < this.directTuneItems.length; i++ ) {
            if (this.directTuneKeyItems.toString() === this.directTuneItems[i].slice(0, this.directTuneKeyItems.length).toString()) {
                keys.push( Number( this.directTuneItems[i][ this.directTuneKeyItems.length ] ) );

                // Set true if find station
                if( this.directTuneKeyItems.length === this.directTuneItems[i].length ) {
                    this.set('directTuneFinished', true);
                }
            }
        }

        return keys;
    }.property('this.directTuneKeyItems.@each'),

    setStation: function(element){
        this.set('station', this.preset[element.preset]);

        SDL.RadioModel.set('activePreset', element.preset);
        SDL.RadioModel.radioDetails.radioStation.frequency = parseInt(SDL.RadioModel.station.slice(0, -1));
        SDL.RadioModel.radioDetails.radioStation.fraction = parseInt(SDL.RadioModel.station.slice(-1));
        FFW.VehicleInfo.OnRadioDetails({"radioStation": SDL.RadioModel.radioDetails.radioStation});
//        FFW.TDKCommunicationRPC.send('set_preset_' +(element.preset + 1));
        FFW.TDKCommunicationRPC.send('frequency_set_' + Number(this.station)*1000);

        SDL.SDLModel.resetControl();
    },

    tuneRadioStation: function(element){
        if (!this.directTuneKeypressed) {
            this.set('directTuneKeypressed', true);
            this.set('station', '');
        }

        if (this.station < 108 && !isNaN(element.preset)) {
            this.set('station', this.station + element.preset);
            this.directTuneKeyItems.pushObject(element.preset);
        }
        if (element.preset == 'Enter' && SDL.RadioModel.station.indexOf('.') < 0) {
            SDL.RadioModel.set('station', SDL.RadioModel.station.slice(0, -1) + '.' + SDL.RadioModel.station.slice(-1));
            SDL.RadioModel.radioDetails.radioStation.frequency = parseInt(SDL.RadioModel.station.slice(0, -1));
            SDL.RadioModel.radioDetails.radioStation.fraction = parseInt(SDL.RadioModel.station.slice(-1));
            this.set('temp', this.station);
            this.set('directTuneFinished', false);
            this.set('directTuneKeypressed', false);
            this.set('directTuneKeyItems', []);

            this.findStationPresets();

            FFW.VehicleInfo.OnRadioDetails({"radioStation": SDL.RadioModel.radioDetails.radioStation});
            FFW.TDKCommunicationRPC.send('frequency_set_' + Number(SDL.RadioModel.station)*1000);

            SDL.SDLModel.resetControl();
        }
        if (element.preset == 'X') {
            SDL.RadioModel.set('station', SDL.RadioModel.station.slice(0, -1));
            this.set('directTuneFinished', false);
            this.directTuneKeyItems.popObject();

            if (!this.directTuneKeyItems.length) {
                this.set('directTuneKeypressed', false);
                this.set('station', this.temp);
            }
        }

    },

    scanKeyPress: function(){
        if (!this.scanState) {
            this.startScan();
        } else {
            this.stopScan();
        }

        SDL.SDLModel.resetControl();
    },

    startScan: function(){

        this.toggleProperty('scanState');
        this.radioDetails.radioStation.frequency = 87;
        this.radioDetails.radioStation.fraction = 9;
        if (SDL.RadioModel) {
            this.scanTimer = setInterval(function(){

                if (SDL.RadioModel.radioDetails.radioStation.fraction < 9) {
                    SDL.RadioModel.radioDetails.radioStation.fraction++;
                } else if (SDL.RadioModel.radioDetails.radioStation.frequency < 107) {
                    SDL.RadioModel.radioDetails.radioStation.frequency++;
                    SDL.RadioModel.radioDetails.radioStation.fraction = 0;
                }
                SDL.RadioModel.set('station', SDL.RadioModel.radioDetails.radioStation.frequency + '.' + SDL.RadioModel.radioDetails.radioStation.fraction);

                SDL.RadioModel.findStationPresets();

                FFW.VehicleInfo.OnRadioDetails({"radioStation": SDL.RadioModel.radioDetails.radioStation});
                FFW.TDKCommunicationRPC.send('frequency_set_' + Number(SDL.RadioModel.station)*1000);

                if (SDL.RadioModel.radioDetails.radioStation.frequency == 107 && SDL.RadioModel.radioDetails.radioStation.fraction == 9) {
                    SDL.RadioModel.stopScan(true); // True parameter means that scan is finished
                }
            }, 100);
        }
    },

    /*
     * True parameter means that scan is finished
     *
     * @param {Boolean} timedOut True parameter means that scan is finished
     */
    stopScan: function(timedOut){

        this.toggleProperty('scanState');
        clearInterval(this.scanTimer);
        this.set('temp', this.station);
        if (timedOut && FFW.VehicleInfo.VIStartScanRequestID) {

            FFW.VehicleInfo.scanResponse(SDL.SDLModel.resultCode["SUCCESS"], FFW.VehicleInfo.VIStartScanRequestID, "VehicleInfo.StartScan");

        } else if (FFW.VehicleInfo.VIStartScanRequestID && !FFW.VehicleInfo.VIStopScanRequestID) {

            FFW.VehicleInfo.scanResponse(SDL.SDLModel.resultCode["ABORTED"], FFW.VehicleInfo.VIStartScanRequestID, "VehicleInfo.StartScan", "Scan stopped by driver.");

        } else if (FFW.VehicleInfo.VIStartScanRequestID && FFW.VehicleInfo.VIStopScanRequestID) {

            FFW.VehicleInfo.scanResponse(SDL.SDLModel.resultCode["ABORTED"], FFW.VehicleInfo.VIStartScanRequestID, "VehicleInfo.StartScan", "Scan stopped by mobile user.");
            FFW.VehicleInfo.scanResponse(SDL.SDLModel.resultCode["SUCCESS"], FFW.VehicleInfo.VIStopScanRequestID, "VehicleInfo.StopScan");

        }
    },

    radioTune: function(params){
        this.radioDetails.radioStation.frequency = params.radioStation.frequency;
        this.radioDetails.radioStation.fraction = params.radioStation.fraction;

        this.set('station', this.radioDetails.radioStation.frequency + '.' + this.radioDetails.radioStation.fraction);

        this.findStationPresets();

        FFW.VehicleInfo.sendVIResult(SDL.SDLModel.resultCode["SUCCESS"], FFW.VehicleInfo.VITuneRadioRequestID, "VehicleInfo.TuneRadio");
        FFW.TDKCommunicationRPC.send('frequency_set_' + Number(SDL.RadioModel.station)*1000);
    },

    tuneUpPress: function () {
        this.tuneUp();

        SDL.SDLModel.resetControl();
    },

    tuneDownPress: function () {
        this.tuneDown();

        SDL.SDLModel.resetControl();
    },

    tuneUp: function () {
        FFW.TDKCommunicationRPC.send('step_up');
    },

    tuneDown: function () {
        FFW.TDKCommunicationRPC.send('step_down');
    },

    findStationPresets: function() {
        var i = 0;

        this.set('activePreset', null);

        for (i; i < this.preset.length; i++) {
            if (this.station == this.preset[i]) {
                this.set('activePreset', i);

                break;
            }
        }
    }
});
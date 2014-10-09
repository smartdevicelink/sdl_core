/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  · Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *  · Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *  · Neither the name of the Ford Motor Company nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 * @name SDL.VehicleInfo
 * @desc VehicleInfo module visual representation
 * @category View
 * @filesource app/view/sdl/VehicleInfo.js
 * @version 1.0
 */

SDL.VehicleInfo = Em.ContainerView.create( {

    elementId: 'VehicleInfo',

    classNames: 'VehicleInfo',

    classNameBindings:
        [
            'active'
        ],

    childViews:
        [
            'prndl',
            'vehicleInfoLabel',
            'prndlSelect',
            'ecu1Title',
            'ecu1',
            'ecu2',
            'ecu1Data',
            'ecu2Data',
            'odometrTitle',
            'odometrInput'
        ],

    /**
     * Title of odometr group of parameters stored data in VehicleInfo model
     */
    odometrTitle: SDL.Label.extend( {

        elementId: 'odometrTitle',

        classNames: 'odometrTitle',

        content: 'Odometr'
    } ),

    /**
     * Input for odometr value changes
     */
    odometrInput: Ember.TextField.extend({
        elementId: "odometrInput",
        classNames: "odometrInput",
        keyUp: function(event, view) {
            if(event.which == 13){
                SDL.SDLVehicleInfoModel.set('odometrInput', parseInt(this.value));
            }
        }
    }),

    /**
     * Label with stored data in VehicleInfo model
     */
    ecu2Data: SDL.Label.extend( {

        elementId: 'ecu2Data',

        classNames: 'ecu2Data',

        contentBinding: 'SDL.SDLVehicleInfoModel.ecuDIDData.1.data'
    } ),

    /**
     * Label with stored data in VehicleInfo model
     */
    ecu1Data: SDL.Label.extend( {

        elementId: 'ecu1Data',

        classNames: 'ecu1Data',

        contentBinding: 'SDL.SDLVehicleInfoModel.ecuDIDData.0.data'
    } ),

    /**
     * Label with name of some parameter stored data in VehicleInfo model
     */
    ecu2: SDL.Label.extend( {

        elementId: 'ecu2',

        classNames: 'ecu2',

        content: 'ECU 2:'
    } ),

    /**
     * Label with name of some parameter stored data in VehicleInfo model
     */
    ecu1: SDL.Label.extend( {

        elementId: 'ecu1',

        classNames: 'ecu1',

        content: 'ECU 1:'
    } ),

    /**
     * Title of ecu group of parameters stored data in VehicleInfo model
     */
    ecu1Title: SDL.Label.extend( {

        elementId: 'ecu1Title',

        classNames: 'ecu1Title',

        content: 'ECU'
    } ),

    /**
     * Title of VehicleInfo PopUp view
     */
    vehicleInfoLabel: SDL.Label.extend( {

        elementId: 'vehicleInfoLabel',

        classNames: 'vehicleInfoLabel',

        content: 'Vehicle Information'
    } ),

    /**
     * Property indicates the activity state of VehicleInfo PopUp
     */
    active: false,

    /**
     * Title of prndl group of parameters stored in VehicleInfo model
     */
    prndl: SDL.Label.extend( {

        elementId: 'prndl',

        classNames: 'prndl',

        content: 'PRNDL'
    } ),

    /**
     * HMI element Select with parameters of transmission state from VehicleInfo
     * Model
     */
    prndlSelect: Em.Select.extend( {

        elementId: 'prndlSelect',

        classNames: 'prndlSelect',

        contentBinding: 'SDL.SDLVehicleInfoModel.vehicleInfoPRNDL',

        valueBinding: 'SDL.SDLVehicleInfoModel.prndlSelectState'
    } ),

    /**
     * Trigger function that activates and deactivates VehicleInfo PopUp
     */
    toggleActivity: function() {
        this.set( 'active', !this.active );
    },
    
    /**
     * This event triggered when component is placed to
     * document DOM structure
     */
    didInsertElement: function() {
        this._super();
    }
} );
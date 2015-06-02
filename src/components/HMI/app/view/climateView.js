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
 * @name SDL.PlayerControllsView
 * @desc Video player visual representation
 * @category View
 * @filesource app/view/player/PlayerView.js
 * @version 1.0
 */
SDL.ClimateView = Em.ContainerView.create( {
    /** View Id */
    elementId: 'climateView',

    classNameBindings:
        [
             'SDL.States.climate.active:active_state:inactive_state'
        ],
    
    childViews:
        [
            'climateView'
        ],


    climateView: Em.ContainerView.extend( {
    
        elementId: 'climate_control',
    
        childViews: [
            'fanSpeed',
            'temperature',
            //'desiredTemp',
            'acEnable',
            'recirculateAirEnable',
            'autoModeEnable',
            'defrostZone',
            'dualModeEnable',
            'passengerTemperature',
            'radio'
            //'frequencyDisplay',
            //'activeBand',
            //'tuneUp',
            //'tuneDown',
            //'amSelect',
            //'fmSelect'
            //'radioEnable'

        ],
        dualModeEnable: Em.ContainerView.extend({
            elementId: 'dualModeEnable_container',
            childViews: [
                'dualModeEnable_label',
                'dualModeEnable_on',
                //'dualModeEnable_off',
                'dualModeEnable_status'
            ],
            
            dualModeEnable_label: SDL.Label.extend({
                elementId:'dualModeEnable_label',
                content: 'Dual Mode'
            }),

            dualModeEnable_on: SDL.Button.extend({
                elementId: 'dualModeEnable_on',
                text: 'Dual',
                action: 'toggleDualMode',
                target: 'SDL.ClimateController.model'
            }),

            dualModeEnable_status: SDL.Label.extend({
                elementId: 'dualModeEnable_status',
                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.dualModeEnableString')
            })


        }),

        defrostZone: Em.ContainerView.extend({
            elementId: 'defrostZone',
            childViews: [
                'defrostZone_label',
                'defrostZone_front',
                'defrostZone_rear',
                'defrostZone_all',
                'defrostZone_status'
            ],

            defrostZone_label: SDL.Label.extend({
                elementId: 'defrostZone_label',
                content: 'Defrost Zone'
            }),
            defrostZone_front: SDL.Button.extend({
                elementId: 'defrostZone_front',
                text: 'Front',
                action: 'defrostFrontEnable',
                target: 'SDL.ClimateController.model'

            }),
            defrostZone_rear: SDL.Button.extend({
                elementId: 'defrostZone_rear',
                text: 'Rear',
                action: 'defrostRearEnable',
                target: 'SDL.ClimateController.model'


            }),
            defrostZone_all: SDL.Button.extend({
                elementId: 'defrostZone_all',
                text:'All',
                action: 'defrostAllEnable',
                target: 'SDL.ClimateController.model'

            }),
            defrostZone_status: SDL.Label.extend({
                elementId: "defrostZone_status",
                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.defrostZone')        
            })
        }),

        autoModeEnable: Em.ContainerView.extend({
            elementId: 'autoModeEnable_container',
            childViews: [
                'autoModeEnable_label',
                'autoModeEnable_on',
               // 'autoModeEnable_off',
                'autoModeEnable_status'
            ],
            
            autoModeEnable_label: SDL.Label.extend({
                elementId:'autoModeEnable_label',
                content: 'Auto Mode'
            }),

            autoModeEnable_on: SDL.Button.extend({
                elementId: 'autoModeEnable_on',
                text: 'Auto',
                action: 'toggleAutoMode',
                target: 'SDL.ClimateController.model'
            }),

            autoModeEnable_status: SDL.Label.extend({
                elementId: 'autoModeEnable_status',

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.autoModeEnableString')            })


        }),


        recirculateAirEnable: Em.ContainerView.extend({
            elementId: 'recirculateAirEnable_container',
            childViews: [
                'recirculateAirEnable_label',
                'recirculateAirEnable_on',
                //'recirculateAirEnable_off',
                'recirculateAirEnable_status'
            ],
            
            recirculateAirEnable_label: SDL.Label.extend({
                elementId:'recirculateAirEnable_label',
                content: 'Recirculate Air'
            }),

            recirculateAirEnable_on: SDL.Button.extend({
                elementId: 'recirculateAirEnable_on',
                text: 'Recirculate',
                action: 'toggleRecirculateAir',
                target: 'SDL.ClimateController.model'
            }),

            recirculateAirEnable_status: SDL.Label.extend({
                elementId: 'recirculateAirEnable_status',
                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.recirculateAirEnableString')
            })


        }),
    
        acEnable: Em.ContainerView.extend({
            elementId: 'acEnable_container',
            childViews: [
                'acEnable_label',
                'acEnable_on',
                //'acEnable_off',
                'acEnable_status'
            ],
            
            acEnable_label: SDL.Label.extend({
                elementId:'acEnable_label',
                content: 'AC Enable:'
            }),

            acEnable_on: SDL.Button.extend({
                elementId: 'acEnable_on',
                text: 'AC',
                action: 'toggleAcEnable',
                target: 'SDL.ClimateController.model'
            }),

            acEnable_status: SDL.Label.extend({
                elementId: 'acEnable_status',
                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.acEnableString')
            })


        }),

        temperature: Em.ContainerView.extend({
            elementId: 'temperature_container',
            childViews: [
                'tempLabel',
                'tempUpButton',
                'tempDownButton',
                'tempLevel'

            ],

            tempLabel: SDL.Label.extend({
                elementId: 'temp_label',

                content: "Temperature",
            }),

            tempUpButton: SDL.Button.extend({
                elementId: 'temp_up_button',

                text: 'Up',

                action: 'tempUp',

                target: 'SDL.ClimateController.model'
            }),

            tempDownButton: SDL.Button.extend({
                elementId: 'temp_down_button',

                text: 'Down',

                action: 'tempDown',

                target: 'SDL.ClimateController.model',

            }),

            tempLevel: SDL.Label.extend({
                elementId: "temp_level",

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.desiredTemp')
            })
        }),

        fanSpeed: Em.ContainerView.extend({
            elementId: 'fan_speed_container',
            childViews: [
                'fanSpeedLabel',
                'fanSpeedUpButton',
                'fanSpeedDownButton',
                'fanSpeedLevel'
            ],

            fanSpeedLabel: SDL.Label.extend({
                elementId: 'fan_speed_label',

                content: "Fan Speed"
            }),

            fanSpeedUpButton: SDL.Button.extend({
                elementId: 'fan_speed_up_button',

                text: 'Up',

                action: 'fanSpeedUp',

                target: "SDL.ClimateController.model"


            }),

            fanSpeedDownButton: SDL.Button.extend({
                elementId: 'fan_speed_down_button',

                text: 'Down',

                action: 'fanSpeedDown',

                target: "SDL.ClimateController.model",

            }),

            fanSpeedLevel: SDL.Label.extend({

                elementId: 'fan_speed_level',

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.currentFanSpeed')

            })
        }),
        passengerTemperature: Em.ContainerView.extend({
            elementId: 'passenger_temperature_container',
            childViews: [
                'passengerTempLabel',
                'passengerTempUpButton',
                'passengerTempDownButton',
                'passengerTempLevel'

            ],

            passengerTempLabel: SDL.Label.extend({
                elementId: 'passenger_temp_label',

                content: "Passenger Temperature",
            }),

            passengerTempUpButton: SDL.Button.extend({
                elementId: 'passenger_temp_up_button',

                text: 'Up',

                action: 'passengerTempUp',

                target: 'SDL.ClimateController.model'
            }),

            passengerTempDownButton: SDL.Button.extend({
                elementId: 'passenger_temp_down_button',

                text: 'Down',

                action: 'passengerTempDown',

                target: 'SDL.ClimateController.model',

            }),

            passengerTempLevel: SDL.Label.extend({
                elementId: "passenger_temp_level",

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.passengerDesiredTemp')
            })
        }),

        radio: Em.ContainerView.extend({
            elementId: 'radio_container',
            childViews:[
                'frequencyDisplay',
                'activeBand',
                'tuneUp',
                'tuneDown',
                'amSelect',
                'fmSelect',
                'sourceLabel'
               // 'radioEnable'                
            ],
            frequencyDisplay: SDL.Label.extend({

                elementId: 'media_player_frequency',
                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.frequencyDisplay')

            }),
            activeBand: SDL.Label.extend({

                elementId: 'media_player_active_band',
                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.source')
            }),

            tuneUp: SDL.Button.extend({
                elementId: 'media_player_tune_up',
                text: 'Tune Up',
                action: 'tuneUp',
                target: 'SDL.ClimateController.model'
            }),

            tuneDown: SDL.Button.extend({
                elementId: 'media_player_tune_down',
                text: 'Tune Down',
                action: 'tuneDown',
                target: 'SDL.ClimateController.model'
            }),

            amSelect: SDL.Button.extend({
                elementId: 'media_player_am_select',
                text: 'AM',
                action: 'selectAM',
                target: 'SDL.ClimateController.model'
            }),

            fmSelect: SDL.Button.extend({
                elementId: 'media_player_fm_select',
                text: 'FM',
                action: 'selectFM',
                target: 'SDL.ClimateController.model'
            }),
            sourceLabel: SDL.Label.extend({
                elementId: 'media_player_source_label',
                content: 'Source:'
            })
        })


       /* */

    } )

} );


/*
    childViews: [
        'frequencyDisplay',
        //'activeBand',
        //'tuneUp',
        //'tuneDown',
        //'amSelect',
        //'fmSelect'
        //'radioEnable'
    ],

    frequencyDisplay: SDL.Label.extend({

        elementId: 'media_player_frequency',
        contentBinding: Em.Binding.oneWay('SDL.MediaController.model.frequency')

    })

   /* activeBand: SDL.Label.extend({

        elementId: 'media_player_active_band',
        contentBinding: Em.Binding.oneWay('SDL.MediaController.model.activeBand')
    }),

    tuneUp: SDL.Button.extend({
        elementId: 'media_player_tune_up',
        text: 'Tune Up',
        action: 'tuneUp',
        target: 'SDL.MediaController.model'
    }),

    tuneDown: SDL.Button.extend({
        elementId: 'media_player_tune_down',
        text: 'Tune Down',
        action: 'tuneDown',
        target: 'SDL.MediaController.model'
    }),

    amSelect: SDL.Button.extend({
        elementId: 'media_player_am_select',
        text: 'AM',
        action: 'selectAM',
        target: 'SDL.MediaController.model'
    }),

    fmSelect: SDL.Button.extend({
        elementId: 'media_player_fm_select',
        text: 'FM',
        action: 'selectFM',
        target: 'SDL.MediaController.model'
    })*/




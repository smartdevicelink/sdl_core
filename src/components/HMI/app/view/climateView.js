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
            'dualModeEnable'

        ],
        dualModeEnable: Em.ContainerView.extend({
            elementId: 'dualModeEnable_container',
            childViews: [
                'dualModeEnable_label',
                'dualModeEnable_on',
                'dualModeEnable_off',
                'dualModeEnable_status'
            ],
            
            dualModeEnable_label: SDL.Label.extend({
                elementId:'dualModeEnable_label',
                content: 'AC Enable'
            }),

            dualModeEnable_on: SDL.Button.extend({
                elementId: 'dualModeEnable_on',
                text: 'On',
                actionDown: function(){
                    SDL.ClimateController.setDualModeEnable(true);
                }
            }),

            dualModeEnable_off: SDL.Button.extend({
                elementId: 'dualModeEnable_off',
                text: 'Off',
                actionDown: function(){
                    SDL.ClimateController.setDualModeEnable(false);
                }
            }),

            dualModeEnable_status: SDL.Label.extend({
                elementId: 'dualModeEnable_status',

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.dualModeEnable')
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
                actionDown: function(){
                    SDL.ClimateController.model.setDefrostZone('FRONT');
                }
            }),
            defrostZone_rear: SDL.Button.extend({
                elementId: 'defrostZone_rear',
                text: 'Rear',
                actionDown: function(){
                    SDL.ClimateController.model.setDefrostZone('REAR');
                }

            }),
            defrostZone_all: SDL.Button.extend({
                elementId: 'defrostZone_all',
                text:'All',
                actionDown: function(){
                    SDL.ClimateController.model.setDefrostZone('ALL');
                }
            }),
            defrostZone_status: SDL.Label.extend({
                elementId: "defrostZone_status",
                contentBinding: Em.Binding.oneWay('SDL.CLimateController.model.defrostZone')
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
                'recirculateAirEnable_off',
                'recirculateAirEnable_status'
            ],
            
            recirculateAirEnable_label: SDL.Label.extend({
                elementId:'recirculateAirEnable_label',
                content: 'AC Enable'
            }),

            recirculateAirEnable_on: SDL.Button.extend({
                elementId: 'recirculateAirEnable_on',
                text: 'On',
                actionDown: function(){
                    SDL.ClimateController.setRecirculateAirEnable(true);
                }
            }),

            recirculateAirEnable_off: SDL.Button.extend({
                elementId: 'recirculateAirEnable_off',
                text: 'Off',
                actionDown: function(){
                    SDL.ClimateController.setRecirculateAirEnable(false);
                }
            }),

            recirculateAirEnable_status: SDL.Button.extend({
                elementId: 'recirculateAirEnable_status',

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.recirculateAirEnable')
            })


        }),
    
        acEnable: Em.ContainerView.extend({
            elementId: 'acEnable_container',
            childViews: [
                'acEnable_label',
                'acEnable_on',
                'acEnable_off',
                'acEnable_status'
            ],
            
            acEnable_label: SDL.Label.extend({
                elementId:'acEnable_label',
                content: 'AC Enable:'
            }),

            acEnable_on: SDL.Button.extend({
                elementId: 'acEnable_on',
                text: 'On',
                actionDown: function(){
                    SDL.ClimateController.setAcEnable(true);
                }
            }),

            acEnable_off: SDL.Button.extend({
                elementId: 'acEnable_off',
                text: 'Off',
                actionDown: function(){
                    SDL.ClimateController.setAcEnable(false);
                }
            }),

            acEnable_status: SDL.Button.extend({
                elementId: 'acEnable_status',

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.acEnable')
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

                target: 'SDL.ClimateController'
            }),

            tempDownButton: SDL.Button.extend({
                elementId: 'temp_down_button',

                text: 'Down',

                action: 'tempDown',

                target: 'SDL.ClimateController',

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

                action: 'climateUp',

                target: "SDL.ClimateController"


            }),

            fanSpeedDownButton: SDL.Button.extend({
                elementId: 'fan_speed_down_button',

                text: 'Down',

                action: 'climateDown',

                target: "SDL.ClimateController",

            }),

            fanSpeedLevel: SDL.Label.extend({

                elementId: 'fan_speed_level',

                contentBinding: Em.Binding.oneWay('SDL.ClimateController.model.currentFanSpeed')

            })
        })

    } )

} );

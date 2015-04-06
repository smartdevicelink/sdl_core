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
 * @name SDL.WidgetMediaView
 *
 * @desc Component for Media Home Widget
 *
 * @category	view
 * @filesource	app/view/home/WidgetMediaView.js
 * @version		1.0
 */
SDL.WidgetMediaView = Em.ContainerView.extend({
	elementId:	'home_widgetMedia',

	/** Media components */
	childViews: [
		'radio'
	],
	/** AM , FM , Sirius modules */
	radio:  Em.ContainerView.extend({
		/** Element Id */
		elementId: 'home_widgetMedia_radio',

		/** View components*/
		childViews: [
			'presets',
			'info',
			'tuneButtons'
		],

		/** Info */
		info: Em.View.extend({
			/** Element Id*/
			elementId: 'home_widgetMedia_radio_info',

            /** Template */
            template:  Em.Handlebars.compile(
                '<div class="stationInfo">'+
                    '<div id="others_cont">'+
                        '<div class="stationimg"></div>'+
                    '</div>'+
                    '<span class="station">' +
                    '{{SDL.RadioModel.station}}' +
                    '</span>'+
                '</div>'
            )
		}),

        tuneButtons: Em.ContainerView.extend({
            elementId:	'home_widgetMedia_tuneButtons',

            childViews: [
                'tuneDown',
                'tuneUp'
            ],

            tuneUp: SDL.Button.extend({
                elementId: 'radio_media_tuneUp_button',
                target: 'SDL.RadioModel',
                action: 'tuneUpPress',
                text: '>>'
            }),

            tuneDown: SDL.Button.extend({
                elementId: 'radio_media_tuneDown_button',
                target: 'SDL.RadioModel',
                action: 'tuneDownPress',
                text: '<<'
            })

        }),

		/** Preset Buttons*/
        presets: Em.ContainerView.extend({

            elementId: 'radio_media_home_presetButtons_wrapper',

            classNames: 'preset-items',

            childViews:[
                '1',
                '2',
                '3',
                '4',
                '5',
                '6'
            ],

            1: SDL.RadioPresetButton.extend( {
                elementId: 'radio_media_home_preset_button1',
                classNames: 'a0',
                textBinding: 'SDL.RadioModel.preset.0',
                templateName: 'text',
                homePreset: true,
                preset: 0,
                presetName: 'PRESET_1'
            }),
            2: SDL.RadioPresetButton.extend( {
                elementId: 'radio_media_home_preset_button2',
                classNames: 'a1',
                textBinding: 'SDL.RadioModel.preset.1',
                templateName: 'text',
                homePreset: true,
                preset: 1,
                presetName: 'PRESET_2'
            }),
            3: SDL.RadioPresetButton.extend( {
                elementId: 'radio_media_home_preset_button3',
                classNames: 'a2',
                textBinding: 'SDL.RadioModel.preset.2',
                templateName: 'text',
                homePreset: true,
                preset: 2,
                presetName: 'PRESET_3'
            }),
            4: SDL.RadioPresetButton.extend( {
                elementId: 'radio_media_home_preset_button4',
                classNames: 'a3',
                textBinding: 'SDL.RadioModel.preset.3',
                templateName: 'text',
                homePreset: true,
                preset: 3,
                presetName: 'PRESET_4'
            }),
            5: SDL.RadioPresetButton.extend( {
                elementId: 'radio_media_home_preset_button5',
                classNames: 'a4',
                textBinding: 'SDL.RadioModel.preset.4',
                templateName: 'text',
                homePreset: true,
                preset: 4,
                presetName: 'PRESET_5'
            }),
            6: SDL.RadioPresetButton.extend( {
                elementId: 'radio_media_home_preset_button6',
                classNames: 'a5',
                textBinding: 'SDL.RadioModel.preset.5',
                templateName: 'text',
                homePreset: true,
                preset: 5,
                presetName: 'PRESET_6'
            } )
        })
	})

});
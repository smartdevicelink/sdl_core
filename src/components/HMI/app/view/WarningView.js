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
 * @name SDL.WarningViewg
 * @desc Warning representation
 * @category View
 * @filesource app/view/WarningView.js
 * @version 1.0
 */

SDL.warningView = Em.ContainerView
    .extend( {

        classNameBindings: [
            'fade:fadeAnimation:fadeWarning', 'hide:inactive_state',
        ],

        elementId: 'warning_view',

        childViews: [
            'content', 'button'
        ],

        content: Em.View
            .extend( {

                classNames: 'message',

                template: Ember.Handlebars
                    .compile('<div class="warning_text"> {{SDL.locale.label.view_warning}}</div>'
                        + '<div class="text">'
                        + '<br>'
                        + '<p>{{SDL.locale.label.view_warning_paragraph1}} </p><br>'
                        + '<p> {{SDL.locale.label.view_warning_paragraph2}} </p><br>'
                        + '</div>')
            }),

        button: Em.View.create( {

            elementId: 'warning_ok_button',

            classNameBindings: [
                'isReady: visible_display', 'pressed:pressed'
            ],

            classNames: [
                'okbut', 'ffw-button'
            ],
            /**
             * Check for webkit fillmode animation support Android < 4 version
             * doesnt support webkit animation fillmode
             */
            checkForCCS3AnimationSupport: function() {

                if (FFW.isAndroid) {
                    return $('body')[0].style.webkitAnimationFillMode === '';
                } else {
                    return false;
                }
            },

            template: Ember.Handlebars.compile('<span>OK</span>'),

            /* this method is called when the web part is fully loaded */
            appLoaded: function() {

                var self = this;
	        self.set('isReady', true);

                var components = Em.ContainerView.create({

                    classNames: 'components',

                    childViews: [
                        'BasicCommunication',
                        'Buttons',
                        'Navigation',
                        'TTS',
                        'UI',
                        'VI',
                        'VR'
                    ],

                    BasicCommunication: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'basicCommunicationCheckBox',

                            classNames: 'basicCommunicationCheckBox item',

                            checkedBinding: 'FLAGS.BC'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'basicCommunicationText item',

                            content: 'BasicCommunication'

                        })
                    }),

                    Buttons: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'buttonsCheckBox',

                            classNames: 'buttonsCheckBox item',

                            checkedBinding: 'FLAGS.Buttons'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'buttonsText item',

                            content: 'Buttons'

                        })
                    }),

                    Navigation: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'navigationCheckBox',

                            classNames: 'navigationCheckBox item',

                            checkedBinding: 'FLAGS.Navi'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'navigationText item',

                            content: 'Navigation'

                        })
                    }),

                    TTS: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'ttsCheckBox',

                            classNames: 'ttsCheckBox item',

                            checkedBinding: 'FLAGS.TTS'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'ttsText item',

                            content: 'TTS'

                        })
                    }),

                    UI: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'uiCheckBox',

                            classNames: 'uiCheckBox item',

                            checkedBinding: 'FLAGS.UI'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'uiText item',

                            content: 'UI'

                        })
                    }),

                    VI: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'viCheckBox',

                            classNames: 'viCheckBox item',

                            checkedBinding: 'FLAGS.VI'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'viText item',

                            content: 'VI'

                        })
                    }),

                    VR: Em.ContainerView.extend({

                        classNames: 'component',

                        childViews: [
                            'checkBox',
                            'text'
                        ],

                        checkBox: Em.Checkbox.extend( {

                            elementId: 'vrCheckBox',

                            classNames: 'vrCheckBox item',

                            checkedBinding: 'FLAGS.VR'

                        }),

                        text: SDL.Label.extend({

                            classNames: 'vrText item',

                            content: 'VR'

                        })
                    })

                });

                components.appendTo('#warning_view');

            }.observes('SDL.appReady'),

            actionDown: function(event) {

                this.set('pressed', true);
            },

            actionUp: function(event) {

                this.set('pressed', false);

                var self = this;

	        self._parentView.set('hide', true); 
                SDL.RPCController.ConnectToSDL();
            }
        })
    });

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
 * @name SDL.Button
 * @desc Universal button component for SDL application
 * @category Controlls
 * @filesource app/controlls/Button.js
 * @version 1.0
 */

SDL.Button = Em.View
    .extend(Ember.TargetActionSupport,
        {
            classNames: [
                'ffw-button', 'notpressed'
            ],

            classNameBindings: [
                'pressed', 'disabled', 'hidden'
            ],

            /** Pressed state binding */
            pressed: false,

            /** Disable actions on button */
            disabled: false,

            /** Button icon class */
            icon: null,

            /** Button text */
            text: null,

            rightText: null,

            target: this.target ? this.target : this,

            /** Arrow icon */
            arrow: false,

            /** Button timer flag */
            timer: 0,

            timerId: null,
            /** Touch leave event flag */
            touchleave: false,

            onDown: true,

            helpMode: false,
            /**  */
            targetElement: null,

            actionDown: function(event) {

                if (this.get('disabled')) { return; }

                var self = this;

                this.set('pressed', true);
                /** Set Mouse Leave Event Flag to false */
                this.set('mouseleave', false);

                // Default trigger action
                if (this.onDown) {
                    this.triggerAction();
                }

                // Call trigger with timeout
                if (this.timer) {
                    this.timerId = setInterval(function() {

                        self.triggerAction();
                    }, this.timer);
                }
            },

            actionUp: function(event) {

                this.set('pressed', false);

                if (this.timer) {
                    clearInterval(this.timerId);
                }

                if (this.get('disabled')) {
                    if (this.touchleave == true) {
                        this.set('touchleave', false);
                    }
                    return;
                }

                if (!this.onDown) {
                    this.triggerAction();
                }
            },

            /** Only for desktop */
            mouseLeave: function(event) {

                this.set('pressed', false);

                if (this.timer) {
                    clearInterval(this.timerId);
                }
            },

            /**
             * Only for IOS Simulation of mouseleave event for touch devices If
             * target element looses focus during touch move event events dont
             * trigger
             */
            touchMove: function(event) {

                /** Set Mouse Leave Event Flag to true */
                this.set('touchleave', this.targetElement !== document
                    .elementFromPoint(event.originalEvent.touches[0].pageX,
                        event.originalEvent.touches[0].pageY));
            },

            // component default template
            defaultTemplate: Em.Handlebars
                .compile('<img class="ico" {{bindAttr src="view.icon"}} />'
                    + '<span>{{view.text}}</span>'),

            templates: {
                text: Em.Handlebars.compile('<span class="text">{{view.text}}</span>'),

                icon: Em.Handlebars
                    .compile('<img class="ico" {{bindAttr src="view.icon"}} />'),

                rightText: Em.Handlebars
                    .compile('<img class="ico" {{bindAttr src="view.icon"}} />'
                        + '<span class="right_text">{{view.text}}</span>'),

                arrow: Em.Handlebars
                    .compile('<img class="ico" {{bindAttr src="view.icon"}} />'
                        + '<span>{{view.text}}</span>'
                        + '<img class="arrow-ico" src="images/common/arrow_ico.png" />'),

                rightIcon: Em.Handlebars
                    .compile('<img class="ico" {{bindAttr src="view.icon"}} />'
                        + '<span>{{view.text}}</span>'
                        + '<img class="right_ico" {{bindAttr src="view.righticon"}} />')
            }
        });
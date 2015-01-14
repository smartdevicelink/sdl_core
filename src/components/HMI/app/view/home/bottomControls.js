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
 * @name SDL.BottomControlls
 * @desc BottomControlls module visual representation
 * @category View
 * @filesource app/view/BottomControls.js
 * @version 1.0
 */

SDL.BottomControls = Em.ContainerView.extend( {

    elementId: 'app_bottom_controlls',

    childViews: [
        'information', 'home', 'settings'
    ],

    information: Em.View.extend( {
        elementId: 'info_but',
        classNameBindings: [
            'SDL.States.info.active:selected'
        ],

        template: Em.Handlebars.compile('<div id="info_but_click"></div>'),

        actionDown: function(event) {

            if (!SDL.States.info.active) {
                if (SDL.InfoController.activeState === 'info.nonMedia') {
                    SDL.NonMediaController.activateCurrentApp();
                } else {
                    SDL.States.goToStates(SDL.InfoController.activeState);
                }
            }
        }
    }),

    home: Em.View.extend( {
        elementId: 'home_but',
        classNameBindings: [
            'SDL.States.home.active:selected'
        ],

        template: Em.Handlebars.compile('<div id="home_but_click"></div>'),

        actionDown: function(event) {

            SDL.States.goToStates('home');
        }
    }),

    settings: Em.View.extend( {
        elementId: 'setting_but',

        classNameBindings: 'SDL.States.settings.active:selected',

        template: Em.Handlebars.compile('<div id="setting_but_click"></div>'),

        actionDown: function(event) {

            if (!SDL.States.settings.active) {
                SDL.States.goToStates(SDL.SettingsController.activeState);
            }
        }
    })
});
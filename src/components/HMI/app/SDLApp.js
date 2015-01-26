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
 * @name SDL
 * @desc Setup application namespace and settings
 * @category Application
 * @filesource app/App.js
 * @version 1.0
 */

SDL = Em.Application.create( {

    /** Override for mouse/touch events */
    customEvents: ('ontouchstart' in document.documentElement) ? {
        touchstart: 'actionDown',
        touchend: 'actionUp',
        touchmove: 'actionMove'
    } : {
        mousedown: 'actionDown',
        mouseup: 'actionUp',
        mousemove: 'actionMove'
    },

    /** Help mode flag {boolean} */
    helpMode: false,

    // determine home view {string}
    homeView: 'info_view',

    /** Set language for localization */
    localization: 'eng',

    // Application ready flag
    appReady: false,

    // Debug text
    debugText: 'debug&nbsp;mode',

    init: function() {

        Em.Logger.log('SDL: Application init!');

        this._super();
    },

    /** Application ready event */
    ready: function() {

        Em.Logger.log('SDL: Application ready!');

        /** State manager init */
        SDL.States = StateManager.create();

        /** container for all views */
        SDL.views = SDL.AppViews.create().appendTo('body');
    }
});

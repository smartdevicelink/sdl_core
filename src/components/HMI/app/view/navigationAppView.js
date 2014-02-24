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
 * @name SDL.NavigationAppView
 * @desc Navigation component
 * @category View
 * @filesource app/view/navigationAppView.js
 * @version 1.0
 */
SDL.NavigationAppView = Em.ContainerView.create( {
    /** View Id */
    elementId: 'NavigationAppView',

    classNameBindings: [
        'this.activeState:active_state:inactive_state'
    ],

    activeState: function(){
        if (SDL.TurnByTurnView.activeTBT) {
            return false;
        } else if (SDL.States.navigationApp.active) {
            return true;
        } else {
            return false;
        }
    }.property('SDL.States.navigationApp.active', 'SDL.TurnByTurnView.activeTBT'),

    childViews: [
        'videoView',
        SDL.BaseNavigationView
    ],

    actionMove:function(event){

        SDL.SDLModel.onTouchEvent(event);
    },

    actionUp:function(event){

        SDL.SDLModel.onTouchEvent(event);
    },

    actionDown:function(event){

        SDL.SDLModel.onTouchEvent(event);
    },

    videoView: Ember.View.create({
        templateName: "video",
        template: Ember.Handlebars.compile('<video id="html5Player"></video>')
    })

});

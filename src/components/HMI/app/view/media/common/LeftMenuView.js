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
 * @name SDL.LeftMenuView
 * @desc Media Left Menu module visual representation
 * @category View
 * @filesource app/view/media/SiriusView.js
 * @version 1.0
 */

SDL.LeftMenuView = Em.ContainerView.extend( {
    /** View ID */
    elementId: 'media_leftmenu',

    /** View components */
    childViews:
        [
            'border',
            'cdButton',
            'sdlButton'
        ],
    /** Border decoration */
    border: Em.View.extend( {
        elementId: 'media_left_menu_border',
        classNames: 'ls_border'
    } ),

    /** CD Station Button */
    cdButton: SDL.Button.extend( {
        classNameBindings:
            [
                'SDL.CDModel.active:active_state'
            ],
        elementId: 'media_cdButton',
        classNames: 'media-ls-item',
        action: 'turnOnCD',
        icon: 'images/media/ico_cd.png',
        target: 'SDL.MediaController',
        textBinding: Ember.Binding.oneWay( 'SDL.locale.label.view_media_cd' )
    } ),
    /** SDL Button */
    sdlButton: SDL.Button.extend( {
        classNameBindings:
            [
                'SDL.States.media.sdlmedia.active:active_state'
            ],
        elementId: 'media_sdlButton',
        classNames: 'media-ls-item',
        hidden: function() {
            return !SDL.SDLMediaController.currentAppId;
        }.property( 'SDL.SDLMediaController.currentAppId' ),
        textBinding: 'SDL.SDLMediaController.currentAppName',
        iconBinding: 'SDL.SDLMediaController.currentAppIcon',
        action: 'activateCurrentApp',
        target: 'SDL.SDLMediaController'
    } )

} );

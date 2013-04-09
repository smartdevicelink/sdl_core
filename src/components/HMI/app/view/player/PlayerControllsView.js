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
SDL.PlayerControllsView = Em.ContainerView.create( {
    /** View Id */
    elementId: 'video_player_view',
    /** Visual display of player controlls */
    classNameBindings:
        [
            "isHidden: hidden",
            "SDL.VideoPlayerController.disableControlls:hidden_display"
        ],
    /** By default controls are hidden */
    isHidden: true,
    /** Controller binding */
    controllerBinding: Ember.Binding.oneWay( 'SDL.VideoPlayerController' ),
    /** Child Views */
    childViews:
        [
            'time',
            'duration',
            'progressbar',
            'play',
            'frw',
            'rew',
            'stop'
        ],

    /** Current Time Lable */
    time: SDL.Label.extend( {
        elementId: "player_controls_curentTime",
        classNames: "current_time",
        contentBinding: Ember.Binding.oneWay( "parentView.controller.currentPlayBackTime" )
    } ),
    /** Total Time Lable */
    duration: SDL.Label.extend( {
        elementId: "player_controls_totalTime",
        classNames: 'total_time',
        contentBinding: Ember.Binding.oneWay( 'parentView.controller.totalDuration' )
    } ),
    /** Play button */
    play: SDL.Button.extend( {
        elementId: "play",
        icon: "images/player/pause.png",
        actionDown: function() {
            if( SDL.VideoPlayerController.model.isPaused ){
                SDL.VideoPlayerController.play();
            }else{
                SDL.VideoPlayerController.pause();
            }
            this._super();
        },
        // toggle play/pause icon
        togglePlayPauseIcon: function() {

            if( SDL.VideoPlayerController.model.isPaused ){
                this.set( 'icon', 'images/player/play.png' );
            }else{
                this.set( 'icon', 'images/player/pause.png' );
            }
        }.observes( 'parentView.controller.model.isPaused' )

    } ),
    /** FF button */
    frw: SDL.Button.extend( {
        elementId: "frw",
        icon: "images/player/frd_ico.png",
        target: "SDL.VideoPlayerController",
        action: "FF"
    } ),
    /** REW button */
    rew: SDL.Button.extend( {
        elementId: "back",
        icon: "images/player/rewind_ico.png",
        target: "SDL.VideoPlayerController",
        action: "REW"
    } ),
    /** STOP button */
    stop: SDL.Button.extend( {
        elementId: "stop",
        icon: "images/player/done_ico.png",
        target: "SDL.VideoPlayerController",
        action: "stop",
        onDown: false
    } ),
    /** Progressbar component */
    progressbar: Em.ContainerView.extend( {

        elementId: 'player_controlls_progressbar',

        classNames: 'progressline_holder',

        childViews:
            [
                'dragArea'
            ],

        layout: Ember.Handlebars.compile( '<div id="glide" class="progressline_width_fix">' + '{{yield}}' + '</div>'

        ),

        dragArea: Em.ContainerView.extend( {

            elementId: 'player_controlls_progressbar_dragarea',

            classNames: 'drag_area',

            childViews:
                [
                    'progressbarline',
                    'mark'
                ],

            progressbarline: Em.View.extend( {
                elementId: 'player_controlls_progressbarline',
                classNames: 'progressbar',
                attributeBindings:
                    [
                        'style'
                    ],
                styleBinding: Ember.Binding.oneWay( 'parentView.parentView.parentView.controller.progressBarStyle' )

            } ),

            mark: Em.View.extend( {
                elementId: 'player_controlls_progressbar_mark',
                elementId: 'mark',
                classNameBindings: 'active:active',
                actionDown: function() {
                    this.set( 'active', true );
                },
                actionUp: function() {
                    this.set( 'active', false );
                }
            } ),

            actionDown: function() {
                SDL.VideoPlayerController.tap();
            }
        } )
    } )

} );

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
/*
 * Reference implementation of VideoPlayer component.
 * 
 * This class is activate on Android OS only since embedded video playback is
 * not supported in Android 2.x and 3.x So Anrdoid native video player is used
 * to play video
 * 
 */

FFW.VideoPlayerRPC = FFW.RPCObserver.create( {

    /** Overal video duration in Seconds */
    totalDuration: 0,

    /** Ready State of video content to start playing */
    isReady: false,

    /** Video Paused state */
    isPaused: false,

    /** Video Error state */
    isError: false,

    /** Current video time */
    currentVideoTime: 0,

    /** New position for tapping */
    newPosition: -1,

    /*
     * identifiers for requests
     */
    startRequestId: -1,
    stopRequestId: -1,
    pauseRequestId: -1,
    resumeRequestId: -1,
    setPositionRequestId: -1,
    getPositionRequestId: -1,

    onAppStateChangedSubscribeRequestId: -1,
    onPositionChangedSubscribeRequestId: -1,

    /**
     * Start video palying (Current clicked element passed to function)
     */
    start: function( name ) {

        var videoName = name + ".mp4";

        this.startRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.startRequestId,
            "method": "VideoPlayer.start",
            "params": {
                "videoName": videoName,
                "scale": FFW.Scale.scalePoint,
                "position": {
                    "x": $( 'body' ).offset().left,
                    "y": $( 'body' ).offset().top
                }
            }
        };
        this.client.send( JSONMessage );

        // this.set('isReady', true);
    },

    /**
     * Change backround color to transparent for Android native player
     * visibility
     */
    hideBackground: function() {
        if( this.isReady ){
            $( 'body' ).css( 'background-color', 'transparent' );
            $( '#active_view' ).addClass( 'hidden' );
        }else{
            $( 'body' ).css( 'background-color', '#000000' );
            $( '#active_view' ).removeClass( 'hidden' );
        }
    }.observes( 'isReady' ),

    /** Abort play back */
    stop: function() {
        this.stopRequestId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.stopRequestId,
            "method": "VideoPlayer.stop"
        };
        this.client.send( JSONMessage );
        this.set( 'isReady', false );
    },

    /** Play video */
    play: function() {
        this.resumeRequestId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.resumeRequestId,
            "method": "VideoPlayer.resume"
        };
        this.client.send( JSONMessage );
        this.set( 'isPaused', false );
    },

    /** Pause Video */
    pause: function() {
        this.pauseRequestId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.pauseRequestId,
            "method": "VideoPlayer.pause"
        };
        this.client.send( JSONMessage );
        this.set( 'isPaused', true );
    },

    /** Set current seeked time to video */
    setPosition: function( seekedTime ) {
        Em.Logger.log( "FFW.VideoPlayer.set position = " + seekedTime );
        this.setPositionRequestId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.setPositionRequestId,
            "method": "VideoPlayer.setPosition",
            "params": {
                "position": seekedTime
            }
        };
        this.client.send( JSONMessage );
        this.newPosition = seekedTime;
        this.set( 'currentVideoTime', seekedTime );
    },

    /** Get current seeked time to video */
    getPosition: function( seekedTime ) {
        this.getPositionRequestId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.getPositionRequestId,
            "method": "VideoPlayer.getPosition"
        };
        this.client.send( JSONMessage );
        this.set( 'currentVideoTime', seekedTime );
    },

    /*
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "VideoPlayerClient"
    } ),

    connect: function() {
        if( !FLAGS.VIDEO_PLAYER_JS ){
            this.client.connect( this );
        }
    },

    disconnect: function() {
        this.client.disconnect();
    },

    onRPCRegistered: function() {
        this._super();
        Em.Logger.log( "FFW.VideoPlayer.onRPCRegistered" );

        // subscribe on position changed
        this.onPositionChangedSubscribeRequestId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": this.onPositionChangedSubscribeRequestId,
            "method": "MB.subscribeTo",
            "params": {
                "propertyName": "VideoPlayer.positionChanged"
            }
        };
        this.client.send( JSONMessage );
    },

    onRPCUnregistered: function() {
        this._super();
        Em.Logger.log( "FFW.VideoPlayer.onRPCUnregistered" );
    },

    /*
     * when result is received from RPC component this function is called It is
     * the propriate place to check results of reuqest execution Please use
     * previously store reuqestID to determine to which request repsonse belongs
     * to
     */
    onRPCResult: function( response ) {
        this._super();

        if( response.id == this.startRequestId ){
            this.set( 'totalDuration', response.result.totalDuration / 1000 );
            this.set( 'isReady', true );
        }else if( response.id == this.stopRequestId ){
        }else if( response.id == this.pauseRequestId ){
        }else if( response.id == this.resumeRequestId ){
        }else if( response.id == this.setPositionRequestId ){
        }else if( response.id == this.getPositionRequestId ){
            this.set( "currentVideoTime", response.result.position );
        }else if( response.id == this.onAppStateChangedSubscribeRequestId || response.id == this.onPositionChangedSubscribeRequestId ){
            this.checkSubscriptionResponse( response );
        }
    },

    /*
     */
    onRPCError: function( error ) {
        this._super();

        this.set( 'isError', true );
    },

    /*
     */
    onRPCNotification: function( notification ) {
        this._super();

        if( notification.method == "VideoPlayer.stateChanged" ){
            // TODO
        }else if( notification.method == "VideoPlayer.positionChanged" ){
            var currTime = notification.params.currentPosition / 1000;
            // Only fo Android
            // check if after tapping on video scroll bar and setting new
            // position
            // right current time is received.
            // Set 1 second for Android to change and send new current position
            if( FFW.isAndroid && SDL.VideoPlayerController.isTaped ){
                Em.Logger.log( "===============================" );
                Em.Logger.log( "newPosition = " + this.newPosition );
                Em.Logger.log( "currTime = " + currTime );
                Em.Logger.log( "isTaped = " + SDL.VideoPlayerController.isTaped );
                Em.Logger.log( "===============================" );
                if( this.newPosition >= 0 && currTime >= this.newPosition && currTime <= this.newPosition + 1 ){
                    this.newPosition = -1;
                    SDL.VideoPlayerController.isTaped = false;
                    this.set( "currentVideoTime", currTime );
                }
            }else{
                this.set( "currentVideoTime", currTime );
            }
        }
    },

    /*
     * Checks that the response to (un)subscription messages is normal.
     * Otherwise, prints a warning. Returns whether the response is normal.
     */
    checkSubscriptionResponse: function( response ) {
        if( response.result != "OK" ){
            Em.Logger.log( "FFW.VideoPlayer: subscription response is not OK: " + response.result );
            return false;
        }
        return true;
    }
} )
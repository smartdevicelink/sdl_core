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
 * @name SDL.TransitionIterator
 * @desc Transition iterator used for proper transitions between states
 * @category utils
 * @filesource app/util/TransitionIterator.js
 * @version 1.0
 */

SDL.TransitionIterator = Em.Object.create( {

    // this flag determine that some of views is loading now
    // need to prevent next state transition, before finish loading
    ready: true,

    // final state path
    finalPath: SDL.homeView,

    // active states
    stateOnEnter: [],

    // inactive states
    stateOnExit: [],

    // start transition timestamp
    // used in DEBUG_MODE
    startTime: 0,

    /**
     * Init transition: set destination state path block other transitions
     * before views inserted in DOM
     * 
     * @path {string}
     */
    init: function( path ) {

        if( FLAGS.DEBUG_MODE ){
            this.startTime = Date.now();
        }
        if( path ){
            this.set( 'finalPath', path );
        }
        this.set( 'ready', false );
    },

    /**
     * Start transitions: recursive activate all active states and deactivate
     * all inactive states
     * 
     * @state {Em.State}: target state object
     */
    start: function( state ) {

        // transition will start only in final state
        if( this.get( 'finalPath' ) != state.get( 'path' ) ){
            return;
        }

        // activation states
        while( this.get( 'stateOnEnter.length' ) ){
            this.get( 'stateOnEnter' ).pop().set( 'active', true );
        }

        // deactivation states
        while( this.get( 'stateOnExit.length' ) ){
            this.get( 'stateOnExit' ).pop().set( 'active', false );
        }

        // unblock transition
        this.set( 'ready', true );

        // show transition time
        // used in DEBUG_MODE
        if( this.startTime ){
            SDL.set( 'debugText', ( ( Date.now() - this.startTime ) / 1000 ).toFixed( 2 ) );
        }
    }
} );
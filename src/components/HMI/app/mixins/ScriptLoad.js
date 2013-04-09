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
 * @name SDL.LoadableView
 * @desc Used for initialization view in runtime
 * @category mixin
 * @filesource app/mixins/ScriptLoad.js
 * @version 1.0
 */

SDL.LoadableView = Ember.Mixin.create( {

    classNameBindings:
        [
            'stateObj.active:active_state:inactive_state'
        ],

    init: function() {
        this._super();

        this.set( 'stateObj', SDL.States.getState( FFW.AppendScript.getState() ) );

        // EXCEPTION
        if( !this.stateObj ){
            alert( this.get( 'elementId' ) + ' is not associated to state object' );
            SDL.States.back();

            return;
        }

        this.stateObj.set( 'viewLoaded', true );

        // for backwards compatible
        this.stateObj.set( 'view', this );

        // this.loadTime = Date.now();

        // for testing
        /*
         * var self = this; setTimeout(function(){ self.loadView(); },1000)
         */
        this.loadView();
    },

    loadView: function() {
        if( this.stateObj.parentState.get( 'path' ) ){
            // for child views
            this.stateObj.parentState.view.get( 'childViews' ).pushObject( this );
        }else{
            // for root views
            SDL.views.activeAview.get( 'childViews' ).pushObject( this );
        }
    },

    didInsertElement: function() {
        this._super();
        SDL.TransitionIterator.start( this.stateObj );

        // dispatch onLoad event to loader
        FFW.AppendScript.onLoad();

        // Em.Logger.log( this.get('elementId')+': ',(Date.now() -
        // this.loadTime));
    }
} );
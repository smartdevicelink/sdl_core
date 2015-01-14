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
 * @name SDL.SliderView
 * @desc Slider visual representation
 * @category View
 * @filesource app/view/sdl/shared/sliderView.js
 * @version 1.0
 */

SDL.SliderView = SDL.SDLAbstractView.create( {

    elementId: 'slider_view',

    childViews:
        [
            'backButton',
            'okButton',
            'captionText',
            'headerLabel',
            'footerLabel',
            'adjustControl'
        ],

    headerLabel: SDL.Label.extend( {
        classNames: 'slider-header',
        content: 'Header Label'
    } ),

    footerLabel: SDL.Label.extend( {
        classNames: 'slider-footer',
        content: 'Footer Label',
        data: []
    } ),

    /**
     * Identifier of current request
     */
    sliderRequestId: null,

    timer: null,

    timeout: null,

    /**
     * Extend deactivate method send SUCCESS response on deactivate with current
     * slider value
     */
    deactivate: function(timeout) {
        this._super();
        this.timeout = null;
        clearTimeout(this.timer);
        this.timer = null;

        if (timeout === true) {
                FFW.UI.sendSliderResult(SDL.SDLModel.resultCode["TIMED_OUT"], this.get('sliderRequestId'));
        } else if (timeout === false) {
            FFW.UI.sendSliderResult(SDL.SDLModel.resultCode["SUCCESS"], this.get('sliderRequestId'), this.get('adjustControl.sliderValue.value'));
        } else {
            FFW.UI.sendSliderResult(SDL.SDLModel.resultCode["ABORTED"], this.get('sliderRequestId'), this.get('adjustControl.sliderValue.value'));
        }

        SDL.SDLController.onSystemContextChange();
    },

    activate: function(text, timeout) {
        if( text ){
            this.set( 'caption', text );
        }

        this.set( 'active', true );

        this.set('timeout', timeout);

        this.timer = setTimeout(function () {
            if (SDL.SliderView.active) {
                SDL.SliderView.deactivate(true);
            }
        }, timeout);
    },

    dataChange: function(){
        if (this.timeout){
            var self = this;

            clearTimeout(this.timer);
            SDL.SDLController.onResetTimeout(SDL.SDLAppController.model.appID, "UI.Slider");
            this.timer = setTimeout(function () {
                self.deactivate(true);
            }, this.timeout);
        }
    }.observes('this.adjustControl.sliderValue.value'),

    okButton: SDL.Button.extend( {
        classNames: 'okButton',
        text: 'Submit',
        onDown: false,
        click: function() {
            SDL.SliderView.deactivate(false);
        }
    } ),

    adjustControl: Em.ContainerView.extend( {

        classNames: 'sliderControl',

        childViews:
            [
                'minusBtn',
                'led',
                'plusBtn'
            ],

        sliderValue: Em.Object.create( {
            range: 10,
            value: 0
        } ),

        minusBtn: SDL.Button.extend( {
            classNames: 'control minus',
            icon: 'images/common/minus-ico.png',
            actionDown: function() {
                this._super();
                if (this._parentView.sliderValue.value > 1) {
                	this.set( 'parentView.sliderValue.value', this._parentView.sliderValue.value - 1 );
                }
            }
        } ),

        led: SDL.Indicator.create( {
            classNames: 'ledContainer ico',
            contentBinding: 'parentView.sliderValue',
            indActiveClass: 'led',
            indDefaultClass: 'led-inactive'
        } ),

        plusBtn: SDL.Button.extend( {
            classNames: 'control plus',
            icon: 'images/common/plus-ico.png',
            actionDown: function() {
                this._super();
                if (this._parentView.sliderValue.value < this._parentView.sliderValue.range) {
                	this.set( 'parentView.sliderValue.value', this._parentView.sliderValue.value + 1 );
                }
            }
        } )
    } ),

    loadData: function( message ) {

        var data = message.params;

        this.set( 'sliderRequestId', message.id );

        this.set( 'headerLabel.content', data.sliderHeader );
        this.get( 'adjustControl.sliderValue' ).set( 'range', data.numTicks );
        this.get( 'adjustControl.sliderValue' ).set( 'value', data.position );

        setTimeout( function() {
            SDL.SliderView.adjustControl.rerender();
        }, 1 );

        if (!data.sliderFooter) {
            this.set('footerLabel.content', '');
            return;
        }

        this.footerLabel.data = data.sliderFooter;

        if (data.sliderFooter.length != data.numTicks) {
            this.set( 'footerLabel.content', data.sliderFooter[0] );
        } else {
            // Magick number is array index correction
            this.set( 'footerLabel.content', data.sliderFooter[data.position - 1] );
        }
    },

    /**
     * Change footer text depends on current slider position works only for
     * dynamic footer mode
     */
    changeFooterText: function() {
        if( this.footerLabel.data && this.footerLabel.data.length > 1 ){
            this.set( 'footerLabel.content', this.footerLabel.data[this.adjustControl.sliderValue.value - 1] );
        }
    }.observes( 'adjustControl.sliderValue.value' )
} );

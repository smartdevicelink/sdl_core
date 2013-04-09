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
 * @name SDL.ToggleIndicatorsButton
 * @desc Set active only to current seleted indicator
 * @category Controlls
 * @filesource app/Controlls/ToggleIndicatorsButton.js
 * @version 1.0
 */

SDL.ToggleIndicatorsButton = SDL.IndButton.extend( {

    // Toggle indicator handeler
    toggleIndicators: function() {

        for( var i = 0; i < this.indicators.length; i++ ){
            if( i != this.content.value ){
                this.indicators[i].set( 'className', this.indDefaultClass );
            }else{
                this.indicators[i].set( 'className', this.indActiveClass );
            }
        }
    },

    /** Define button template */
    template: Ember.Handlebars.compile( '{{#with view}}' + '{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}' + '{{#if text}}<p>{{text}}</p>{{/if}}'
                    + '{{#if enabled}}' + '{{#each indicators}}' + '<div {{bindAttr class="className"}}></div>' + '{{/each}}' + '{{/if}}' + '{{/with}}' )
} );

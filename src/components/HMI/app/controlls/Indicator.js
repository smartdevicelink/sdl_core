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
 * @name SDL.Indicator
 * @desc Universal button component with value indicator for SDL application
 * @category Controlls
 * @filesource app/controlls/Indicator.js
 * @version 1.0
 */

SDL.Indicator = Em.View.extend(Ember.TargetActionSupport, {

    /** Content binding */
    content: null,

    /** internal indicators array */
    indicators: null,

    /** indicator active class */
    indActiveClass: null,

    /** indicator default class */
    indDefaultClass: null,

    /** Set active indicator from the start */
    startFrom: null,

    /** binding property to enable/disable indicators */
    enabledBinding: 'content.enabled',

    /**
     * Before rendering view handeler need to generate indicators array based on
     * indicator range value
     */
    beforeRender: function() {

        // define variables
        var length = this.content.range, view = this, i;

        // generate indicators
        this.indicators = [];

        for (i = 0; i < length; i++) {
            this.indicators.push(Em.Object.create( {
                index: i,
                className: this.indDefaultClass
            }));
        }
        // apply indicator visualization rule
        view.setRecord();

        if (this.startFrom) {
            this.indicators[this.startFrom].set('className',
                this.indActiveClass);
        }

        // add observer to content record
        this.addObserver('content', function() {

            view.setRecord();
        });

        // view internal call
        this.applyAttributesToBuffer(this.buffer);
    },

    actionDown: function() {

        this.triggerAction();
    },

    // change record binding
    setRecord: function() {

        var view = this;

        if (this.content.observersForKey('value').length == 0) {

            this.content.addObserver('value', function() {

                view.toggleIndicators();
            });

        }

        this.toggleIndicators();
    },

    // Toggle indicator handeler
    toggleIndicators: function() {

        var length = this.indicators.length, i;

        for (i = 0; i < length; i++) {
            if (i >= this.content.value) {
                this.indicators[i].set('className', 'SDL_indicator '
                    + this.indDefaultClass);
            } else {
                this.indicators[i].set('className', 'SDL_indicator '
                    + this.indActiveClass);
            }
        }
    },

    /** Define indicator template */
    template: Ember.Handlebars.compile('{{#with view}}'
        + '{{#each indicators}}'
        + '<div {{bindAttr class="className view.enabled:show"}}></div>'
        + '{{/each}}' + '{{/with}}')

});

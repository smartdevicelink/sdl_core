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
 * @name Ranged Value
 * @desc Abstract ranged value class
 * @category Model
 * @filesource app/model/abstract/RangedValue.js
 * @version 1.0
 */

SDL.RangedValue = Em.Object.extend( {

    range: 3,
    minValue: 1,
    value: 0,
    cycle: true,
    enabled: true,

    /**
     * @desc Cyclically increases seat value until it reaches the range value
     *       than value is switched to off (0)
     */
    increase: function() {

        if (!this.enabled) {
            this.enable();
            return;
        }

        if (this.value < this.range) {
            this.set('value', this.value + 1);
        } else {
            if (!this.cycle) { return; }
            this.set('value', 0);
        }
    },
    /**
     * Toggle indicators one by one
     */
    toggleIndicators: function() {

        if (!this.enabled) {
            this.enable();
            return;
        }

        if (this.value < this.range - 1) {
            this.set('value', this.value + 1);
        } else {
            this.set('value', 0);
        }
    },
    /**
     * @desc Cyclically increases seat value until it reaches zero value (off)
     *       than value is changed to the range value
     */
    decrease: function() {

        if (!this.enabled) {
            this.enable();
            return;
        }

        if (this.value >= this.minValue) {
            if (!this.cycle && (this.value == this.minValue)) { return; }
            this.set('value', this.value - 1);
        } else {
            this.set('value', this.range);
        }
    },

    /**
     * @desc Switch off
     */
    off: function() {

        this.set('beforeOff', this.value);
        this.set('value', 0);
    },

    /**
     * Switch on
     */
    on: function() {

        this.set('value', this.range);
    },

    /**
     * Switch visual enable
     */
    enable: function() {

        this.set('enabled', true);
    },

    /**
     * Switch visual disable
     */
    disable: function() {

        this.set('enabled', false);
    },

    /**
     * Revert to value before toggle Off
     */
    revert: function() {

        if (this.beforeOff) {
            this.set('value', this.beforeOff);
        }
    }
})
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
 * @name SDL.RadioPresetButton
 * 
 * @desc Media view component for preset Button
 * 
 * @category	view
 * @filesource	app/view/media/RadioPresetButton.js
 * @version		1.0
 */

SDL.RadioPresetButton = SDL.Button.extend({
	
	/** Bind class for visual representation of active button */
	classNameBindings: ['highlight:active'],
	
	classNames: 'preset-item',

    homePreset: false,
	
    highlight: function() {
        return (this.preset == SDL.RadioModel.activePreset);
    }.property('SDL.RadioModel.activePreset'),

    presetNum: function() {
        return this.preset + 1;
    }.property(),

        /** Define button template */
    template: Ember.Handlebars.compile(
		'{{#unless view.homePreset}}<span>{{view.presetNum}}</span>{{/unless}}'+
		'<p>{{view.text}}</p>'
	),

    actionDown: function() {

        this._super();
        SDL.SDLController.onSoftButtonActionDown(this);

        var self = this;
        this.timerPreset = null;
        this.saveToPreset = false;
        this.timerPreset = setTimeout(function() {
            self.saveToPreset = true;
            SDL.RadioModel.saveStationToPreset(self);
        }, 1000);
    },

    actionUp: function() {

        this._super();
        SDL.SDLController.onSoftButtonActionUp(this);

        if (!this.saveToPreset) {
            SDL.RadioModel.setStation(this);
        }
        clearTimeout(this.timerPreset);
        this.timerPreset = null;
        this.saveToPreset = false;
    }

});
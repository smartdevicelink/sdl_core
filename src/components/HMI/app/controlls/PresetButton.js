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
 * @name SDL.PresetButton
 * 
 * @desc Media view component for preset Button
 * 
 * @category	view
 * @filesource	app/view/media/PresetButton.js
 * @version		1.0
 */
SDL.PresetButton = SDL.Button.extend({
	
	/** Bind class for visual representation of active button */
	classNameBindings: ['pressed','highlight:active'],
	
	classNames: 'preset-item',
	
	/** Computed property for hilighted state of button*/
	highlight: function(){
		if(SDL.MediaController.directTuneSelected){
			return false;
		}else{
			return (this.data.selectedIndex == (this.num -1));
		}
	}.property('this.data.selectedIndex','SDL.MediaController.directTuneSelected'),
	
	/** Form right frequency name according to station type (HD or not HD)*/
	frequency: function(){
		if(this.content.isHd)
			return  this.content.frequency+' HD2';
		else
			return  this.content.frequency;
	}.property('content.frequency'),
	
	/** Define button template */
	template: Ember.Handlebars.compile(
		'<span>{{view.num}}</span>'+
		'<p>{{view.frequency}}</p>'
	),
	
	/** Trigger action on mousedown */
	actionDown: function() {
			
			if (SDL.helpMode) {
				SDL.VideoPlayerController.start('ent_Presets');
				return;
			}
			
			this._super();
			
			var self = this;
			/** init timer for save preset logic*/
			this.timerId = setTimeout(function(){
				if(!SDL.MediaController.directTuneHide){
					SDL.MediaController.offDirectTune();
				}
				SDL.MediaController.onStorePreset(self.content,self.data,(self.num -1));
			},this.timer);				
	},
	
	/** Trigger action on mouseup */
	actionUp: function() {
		this._super();
		if (SDL.helpMode) {
			return;
		}
		SDL.MediaController.set('directTuneSelected', false);
		SDL.MediaController.onPresetButtonActiveStateChange((this.num-1), this.data);
	}

})
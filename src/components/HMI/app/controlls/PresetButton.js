/**
 * @name MFT.PresetButton
 * 
 * @desc Media view component for preset Button
 * 
 * @category	view
 * @filesource	app/view/media/PresetButton.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.PresetButton = MFT.Button.extend({
	
	/** Bind class for visual representation of active button */
	classNameBindings: ['pressed','highlight:active'],
	
	classNames: 'preset-item',
	
	/** Computed property for hilighted state of button*/
	highlight: function(){
		if(MFT.MediaController.directTuneSelected){
			return false;
		}else{
			return (this.data.selectedIndex == (this.num -1));
		}
	}.property('this.data.selectedIndex','MFT.MediaController.directTuneSelected'),
	
	/** Form right frequency name according to station type (HD or not HD)*/
	frequency: function(){
		if(this.content.isHd)
			return  this.content.frequency+' HD2';
		else
			return  this.content.frequency;
	}.property('content.frequency'),
	
	/** Define button template */
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'<span>{{num}}</span>'+
			'<p>{{frequency}}</p>'+
		'{{/with}}'
	),
	
	/** Trigger action on mousedown */
	actionDown: function() {
			this._super()
			
			if (MFT.helpMode) {
				MFT.VideoPlayerController.start('ent_Presets');
				return;
			}
			
			var self = this;
			/** init timer for save preset logic*/
			this.timerId = setTimeout(function(){
				if(!MFT.MediaController.directTuneHide){
					MFT.MediaController.offDirectTune();
				}
				MFT.MediaController.onStorePreset(self.content,self.data,(self.num -1));
			},this.timer);				
	},
	
	/** Trigger action on mouseup */
	actionUp: function() {
		this._super();
		if (MFT.helpMode) {
			return;
		}
		MFT.MediaController.set('directTuneSelected', false);
		MFT.MediaController.onPresetButtonActiveStateChange((this.num-1), this.data);
	}

})
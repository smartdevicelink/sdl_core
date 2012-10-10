/**
 * @name MFT.DirectTune
 * 
 * @desc DirectTune visual representation
 * 
 * @category	View
 * @filesource	app/view/media/common/DirectTune.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.DirectTune = Em.ContainerView.create({
	
	classNameBindings: ['MFT.MediaController.directTuneStations.length:visible'],
	
	elementId: 'media_directTune',
	
	childViews: [
		'backspaceButton',
		'enterButton',
		'tuneButtons'
	],
	
	backspaceButton: MFT.Button.extend({
		elementId:			'media_directTune_keyBack',
		
		classNames:			'key',
		icon:				'images/phone/del.png',
		
		target:				'MFT.MediaController',
		action:				'onDirectTuneBack',
		
		onDown:				false,
		
		disabled: function() {
			return !MFT.MediaController.directTune.length;
		}.property('MFT.MediaController.directTune.length'),
	}),
	
	enterButton: MFT.Button.extend({
		elementId:			'media_directTune_keyEnter',
		
		classNames:			'key',
		text:				'Enter',
		
		target:				'MFT.MediaController',
		action:				'onDirectTuneSet',
		
		onDown:				false,
		
		disabled: function() {
			return !MFT.MediaController.directTuneFinished;
		}.property('MFT.MediaController.directTuneFinished')		
	}),
	
	/* Active keys */
	keysBinding:	'MFT.MediaController.directTuneKeys',
	
	tuneButtons: Em.ContainerView.extend({
		
		elementId: 'media_directTune_tuneButtons',
		
		afterRender: function() {
		
			var element, ind;
						
			for ( var i = 1; i < 11; i++ ) {
				
				ind = (i == 10) ? 0 : i;
				
				element = MFT.Button.create({
					
					elementId:			'media_directTune_tuneButtons'+i,
					classNames:			'key',
					index:				ind,
					text:				String(ind),
					disabledBinding:	'isDisabled',
					target:				'MFT.MediaController',
					action:				'onDirectTuneKeyPress',
					
					onDown:				false,
					
					isDisabled:	function() {
						if ( MFT.DirectTune.get('keys').contains( this.index ) ) {
							return false
						} else {
							return true;
						}
					}.property('MFT.DirectTune.keys')	
				});
							
				// Push element to list
				this.get('childViews').pushObject(element);
			}
		}	
	}),
	
	turnOffDirectTuneInHelpMode: function(){
		if(MFT.helpMode){
			MFT.MediaController.offDirectTune();
		}
	}.observes('MFT.helpMode')
});
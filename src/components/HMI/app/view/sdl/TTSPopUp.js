/**
 * @name MFT.TTSPopUp
 * 
 * @desc TTSPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/sdl/TTSPopUp.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.TTSPopUp = Em.ContainerView.create({

	elementId:			'TTSPopUp',

	classNames:			'TTSPopUp',

	classNameBindings:		['active'],

	childViews: [
		'popUp',
		'message'
	],

	content:			'Messaage',

	active: 			false,

	timer:				null,

	popUp : Em.View.extend({

		elementId:			'popUp',

		classNames:			'popUp',
	}),

	message: MFT.Label.extend({

		elementId:			'message',

		classNames:			'message',

		contentBinding:		'parentView.content'
	}),

	ActivateTTS: function(msg, delay){
		var self = this;
				
		// play audio alert
		//MFT.Audio.play('audio/initial.wav');
		
		this.set('content', msg);
		this.set('active', true);

		FFW.UI.OnSystemContext( 'NOT_AUDIBLE' );
		
        clearTimeout(this.timer);
		this.timer = setTimeout(function(){
            self.set('active', false);
            self.onStateChange();
        }, delay ? delay : 10000 );
	},
	
	// send context notification
	onStateChange: function() {
        if ( !MFT.AlertPopUp.active ) {
			FFW.UI.OnSystemContext( 'AUDIBLE' );
        }
	}
});
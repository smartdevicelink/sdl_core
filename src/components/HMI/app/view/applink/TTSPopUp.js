/**
 * @name MFT.TTSPopUp
 * 
 * @desc TTSPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/applink/TTSPopUp.js
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

	ActivateTTS: function(msg){
		var self = this;
				
		// play audio alert
		MFT.Audio.play('audio/initial.wav');
		
		this.set('content', msg);
		this.set('active', true);
		MFT.ApplinkController.onSystemContextChange();
		
        clearTimeout(this.timer);
		this.timer = setTimeout(function(){
            self.set('active', false);
            MFT.ApplinkController.onSystemContextChange();
        }, 10000);
	}
});
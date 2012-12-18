/**
 * @name MFT.AlertPopUp
 * 
 * @desc AlertPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/home/AlertPopUp.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.AlertPopUp = Em.ContainerView.create({

	elementId:			'AlertPopUp',

	classNames:			'AlertPopUp',

	classNameBindings:		['activate:AlertActive'],

	childViews: [
		'applicationName',
		'message1',
		'message2'
	],

	content1:			'Title',

	content2:			'Text',

	activate: 			false,


	applicationName :	MFT.Label.extend({

		elementId:			'applicationName',

		classNames:			'applicationName',

		contentBinding:		'parentView.appName'
	}),

	message1 : MFT.Label.extend({

		elementId:			'message1',

		classNames:			'message1',

		contentBinding:		'parentView.content1'
	}),

	message2 : MFT.Label.extend({

		elementId:			'message2',

		classNames:			'message2',

		contentBinding:		'parentView.content2'
	}),

	AlertActive: function( appId, msg1, msg2, duration, playTone){
		var self = this;
		
		// play audio alert
		if ( playTone ) {
			MFT.Audio.play('audio/alert.wav');
		}
		
		this.set('appName',		MFT.ApplinkController.getApplicationModel(appId).appInfo.appName);
		this.set('content1',	msg1);
		this.set('content2',	msg2);
		this.set('activate',	true);
		
		setTimeout(function(){self.set('activate', false);}, duration);
	}
});
/**
 * @name MFT.TTSPopUp
 * 
 * @desc TTSPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/home/TTSPopUp.js
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

	popUp : Em.View.extend({

		elementId:			'popUp',

		classNames:			'popUp',
	}),

	message : MFT.Label.extend({

		elementId:			'message',

		classNames:			'message',

		contentBinding:		'parentView.content'
	}),

	ActivateTTS: function(msg){
		var self = this;

		this.set('content', msg);
		this.set('active', true);
		setTimeout(function(){self.set('active', false);}, 10000);
	}
});
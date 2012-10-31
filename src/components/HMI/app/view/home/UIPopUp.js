/**
 * @name MFT.UIPopUp
 * 
 * @desc UIPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/home/UIPopUp.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.UIPopUp = Em.ContainerView.create({

	elementId:			'UIPopUp',

	classNames:			'UIPopUp',

	classNameBindings:		['received'],

	childViews: [
		'popUp',
		'message'
	],

	content:			'Messaage',

	received: 			false,

	popUp : Em.View.extend({

		elementId:			'popUp',

		classNames:			'popUp',
	}),

	message : MFT.Label.extend({

		elementId:			'message',

		classNames:			'message',

		contentBinding:		'parentView.content'
	}),

	receiveMessage: function(msg1, msg2, duration, playTone){
		var self = this;

		this.set('content', msg1);
		this.set('received', true);
		setTimeout(function(){self.set('received', false);}, duration);
	}
});
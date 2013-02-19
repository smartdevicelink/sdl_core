/**
 * @name MFT.DriverDistraction
 * 
 * @desc DriverDistractionPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/applink/driverDistraction.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.DriverDistraction = Em.ContainerView.create({

	elementId:			'driverDistraction',

	classNames:			'driverDistractionWindow',

	classNameBindings:		['active'],

	childViews: [
		'driverDistractionPopUp',
		'driverDistractionText'
	],

	content:			'Not accessible while driving',

	active: 			false,

	driverDistractionPopUp: Em.View.create({

		classNames:			'driverDistraction',
	}),

	driverDistractionText : MFT.Label.extend({

		elementId:			'driverDistractionText',

		classNames:			'driverDistractionText',

		contentBinding:		'parentView.content'
	}),

	activate: function(){
		this.set('active', true);
		setTimeout(function(){
			MFT.DriverDistraction.deactivate();
		}, 3000);

        MFT.ApplinkController.onSystemContextChange();
	},

	deactivate: function(){
		this.set('active', false);
		
        MFT.ApplinkController.onSystemContextChange();
	},
});
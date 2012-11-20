/**
 * @name MFT.DriverDistraction
 * 
 * @desc DriverDistractionPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/home/driverDistraction.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.DriverDistraction = Em.ContainerView.create({

	elementId:			'driverDistraction',

	classNames:			'driverDistraction',

	classNameBindings:		['active'],

	childViews: [
		'driverDistractionText'
	],

	content:			'Not accessible while driving',

	active: 			false,

	driverDistractionText : MFT.Label.extend({

		elementId:			'driverDistractionText',

		classNames:			'driverDistractionText',

		contentBinding:		'parentView.content'
	}),

	activate: function(){
		this.set('active', true);
	},

	deactivate: function(){
		this.set('active', false);
	},
});
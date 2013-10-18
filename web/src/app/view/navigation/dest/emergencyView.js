/**
 * @name MFT.NavigationDestinationEmergencyView
 * 
 * @desc Navigation Destination Emergency  module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/EmergencyView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationEmergencyView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode','FLAGS.locale'],
	
	elementId:		'view_navi_destination_emergency',
		
	childViews: [
		'backButton',
		'content',
		'heading'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 			'view_navi_destination_emergency_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'navigation_destination_heading_title',
		classNames: 'block-header-title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_emergency_header')
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled sa">{{MFT.locale.label.view_navi_destination_emergency_sa}}</div>'+
			'<div class="label-20 disabled emerg">{{MFT.locale.label.view_navi_destination_emergency_header}}</div>'+
			'<div class="label-20 disabled nearby">{{MFT.locale.label.view_navi_destination_emergency_nearby}}</div>'+
			'<div class="label-20 disabled police">{{MFT.locale.label.view_navi_destination_emergency_police}}</div>'+
			'<div class="label-20 disabled hospital">{{MFT.locale.label.view_navi_destination_emergency_hospital}}</div>'
		)	
	})
});
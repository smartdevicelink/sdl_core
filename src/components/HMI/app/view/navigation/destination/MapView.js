/**
 * @name MFT.NavigationDestinationMyHomeView
 * 
 * @desc Navigation Destination Map module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/MapView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationMapView = Em.ContainerView.create({
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.navigation.dest.map.active:active_state','MFT.helpMode',],
	
	elementId:		'view_navi_destination_map',
	
	classNames:	   ['hidden'],
	
	childViews: [
		'content',
		'destination'
	],
	
	destination: MFT.Button.extend({
		elementId:		   'view_navi_destination_map_destination_button',
		classNames:		  ['button','label-20'],		
		action:			  'back',
		target:			  'MFT.States',
		textBinding: 'MFT.locale.label.view_navi_destination_streetAddress_dest',	
		disabledBinding:	 Ember.Binding.oneWay('MFT.helpMode'),
	}),
	  
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled view">{{MFT.locale.label.view_navi_destination_streetAddress_view}}</div>'+
			'<div class="label-20 disabled set_as_dest">{{MFT.locale.label.view_navi_destination_streetAddress_set_as_dest}}</div>'+
			'<div class="label-20 disabled saw">{{MFT.locale.label.view_navi_destination_streetAddress_saw}}</div>'+
			'<div class="label-20 disabled saf">{{MFT.locale.label.view_navi_destination_streetAddress_saf}}</div>'+
			'<div class="label-20 disabled poi">{{MFT.locale.label.view_navi_destination_streetAddress_poi}}</div>'+
			'<div class="label-20 disabled tf">{{MFT.locale.label.view_navi_destination_streetAddress_tf}}</div>'+
			'<div class="label-20 disabled menu">{{MFT.locale.label.view_navi_destination_streetAddress_menu}}</div>'+
			'<div class="label-20 disabled chestnut">{{MFT.locale.label.view_navi_destination_streetAddress_chestnut}}</div>'+
			'<div class="label-20 disabled dest">{{MFT.locale.label.view_navi_destination_streetAddress_dest}}</div>'
		)	
	})
});
/**
 * @name MFT.NavigationDestinationStreetAddress
 * 
 * @desc Navigation Destination Street Address  module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/StreetAddressView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationStreetAddress = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_streetAddress',
		
	childViews: [
		'backButton',
		'content'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 			'view_navi_destination_streetAddress_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled number">{{MFT.locale.label.view_navi_destination_streetAddress_number}}</div>'+
			'<div class="label-20 disabled city">{{MFT.locale.label.view_navi_destination_streetAddress_city}}</div>'+
			'<div class="label-20 disabled street">{{MFT.locale.label.view_navi_destination_streetAddress_street}}</div>'+
			'<div class="label-20 disabled de">{{MFT.locale.label.view_navi_destination_streetAddress_de}}</div>'+
			'<div class="label-20 disabled l26">26</div>'+
			'<div class="label-20 disabled sp">{{MFT.locale.label.view_navi_destination_streetAddress_sp}}</div>'+
			'<div class="label-20 disabled mi">{{MFT.locale.label.view_navi_destination_streetAddress_mi}}</div>'+

			'<div class="keyboard">'+
					'<div class="label-20 disabled space">{{MFT.locale.label.view_navi_destination_streetAddress_space}}</div>'+
			'</div>'
		)	
	})
});
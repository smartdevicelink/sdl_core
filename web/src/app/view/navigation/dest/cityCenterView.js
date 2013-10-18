/**
 * @name MFT.NavigationDestinationCityCenterView
 * 
 * @desc Navigation Destination City Center module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/CityCenterView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationCityCenterView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_cityCenter',
		
	childViews: [
		'backButton',
		'heading',
		'content'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 		   'view_navi_destination_cityCenter_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'navigation_destination_favorites_title',
		classNames: 'block-header-title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_cityCenter_header')
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled city">{{MFT.locale.label.view_navi_destination_streetAddress_city}}</div>'+
			'<div class="label-20 disabled state">{{MFT.locale.label.view_navi_destination_Freeway_state}}</div>'+
			'<div class="label-20 disabled mi">{{MFT.locale.label.view_navi_destination_Freeway_mi}}</div>'+

			'<div class="label-20 disabled n26">26</div>'+

			'<div class="label-20 disabled pointer">I</div>'+

			'<div class="keyboard">'+
				'<div class="label-20 disabled space">{{MFT.locale.label.view_navi_destination_streetAddress_space}}</div>'+
			'</div>'
		)
	})
});
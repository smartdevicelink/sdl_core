/**
 * @name MFT.NavigationDestinationIntersectionView
 * 
 * @desc Navigation Destination Intersection module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/IntersectionView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationIntersectionView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_intersection',
		
	childViews: [
		'backButton',
		'content'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 			'navi_destination_intersection_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled street1">{{MFT.locale.label.view_navi_destination_streetAddress_street1}}</div>'+
			'<div class="label-20 disabled city">{{MFT.locale.label.view_navi_destination_streetAddress_city}}</div>'+
			'<div class="label-20 disabled street2">{{MFT.locale.label.view_navi_destination_streetAddress_street2}}</div>'+
			'<div class="label-20 disabled de">{{MFT.locale.label.view_navi_destination_streetAddress_de}}</div>'+
			'<div class="label-20 disabled l26">26</div>'+
			'<div class="label-20 disabled sp">{{MFT.locale.label.view_navi_destination_streetAddress_sp}}</div>'+
			'<div class="label-20 disabled mi">{{MFT.locale.label.view_navi_destination_streetAddress_mi}}</div>'+
			
			'<div class="label-20 disabled space">{{MFT.locale.label.view_navi_destination_streetAddress_space}}</div>'
		)
	})
});
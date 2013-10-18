/**
 * @name MFT.NavigationDestinationFavorites
 * 
 * @desc Navigation Destination Favorites module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/FavoritesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationFavorites = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_favorites',
		
	childViews: [
		'backButton',
		'content',
		'heading'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 		   'view_navi_destination_favorites_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'navigation_destination_favorites_title',
		classNames: 'block-header-title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_favorites_header')
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
				'<div class="label-20 disabled i1">1.</div>'+
				'<div class="label-20 disabled i2">2.</div>'+
				'<div class="label-20 disabled myhome">{{MFT.locale.label.view_navi_destination_favorites_myhome}}</div>'+
				'<div class="label-20 disabled sortby">{{MFT.locale.label.view_navi_destination_favorites_sort}}</div>'+
				'<div class="label-20 disabled az">{{MFT.locale.label.view_navi_destination_favorites_az}}</div>'+
				'<div class="label-20 disabled date">{{MFT.locale.label.view_navi_destination_favorites_date}}</div>'+
				'<div class="label-20 disabled edit">{{MFT.locale.label.view_navi_destination_favorites_edit}}</div>'+
				'<div class="label-20 disabled delete1">{{MFT.locale.label.view_navi_destination_favorites_delete}}</div>'+
				'<div class="label-20 disabled delete2">{{MFT.locale.label.view_navi_destination_favorites_delete}}</div>'+
				'<div class="label-20 disabled add">{{MFT.locale.label.view_navi_destination_favorites_add}}</div>'
		)	
	})
});
/**
 * @name MFT.NavigationDestinationPOIView
 * 
 * @desc Navigation Destination POI module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/POIView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationPOIView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: [
						 'MFT.helpMode',
						 'FLAGS.locale' 
					   ],
	
	elementId:		'view_navi_destination_poi',
		
	childViews: [
		'backButton',
		'content',
		'heading'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 			'view_navi_destination_poi_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'navigation_destination_POI_title',
		classNames: 'block-header-title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_poi_header')
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled sa">{{MFT.locale.label.view_navi_destination_previous_sa}}</div>'+
			'<div class="label-20 disabled sbn">{{MFT.locale.label.view_navi_destination_previous_sbn}}</div>'+
	
			'<div class="label-20 disabled cat">{{MFT.locale.label.view_navi_destination_previous_cat}}</div>'+
			'<div class="label-20 disabled fav">{{MFT.locale.label.view_navi_destination_previous_fav}}</div>'+
			'<div class="label-20 disabled all">{{MFT.locale.label.view_navi_destination_previous_all}}</div>'+
			'<div class="label-20 disabled recent">{{MFT.locale.label.view_navi_destination_previous_recent}}</div>'+
	
			'<div class="label-20 disabled fs">{{MFT.locale.label.view_navi_destination_previous_fs}}</div>'+
			'<div class="label-20 disabled fin">{{MFT.locale.label.view_navi_destination_previous_fin}}</div>'+
			'<div class="label-20 disabled ar">{{MFT.locale.label.view_navi_destination_previous_ar}}</div>'+
			'<div class="label-20 disabled acc">{{MFT.locale.label.view_navi_destination_previous_acc}}</div>'+
			'<div class="label-20 disabled p">{{MFT.locale.label.view_navi_destination_previous_p}}</div>'+
	
			'<div class="label-20 disabled edit1">{{MFT.locale.label.view_navi_destination_previous_edit}}</div>'+
			'<div class="label-20 disabled edit2">{{MFT.locale.label.view_navi_destination_previous_edit}}</div>'+
			'<div class="label-20 disabled edit3">{{MFT.locale.label.view_navi_destination_previous_edit}}</div>'+
			'<div class="label-20 disabled edit4">{{MFT.locale.label.view_navi_destination_previous_edit}}</div>'+
			'<div class="label-20 disabled edit5">{{MFT.locale.label.view_navi_destination_previous_edit}}</div>'
		)	
	})
});
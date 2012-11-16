/**
 * @name MFT.NavigationDestinationMyHomeView
 * 
 * @desc Navigation Destination My Home  module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/NavigationDestinationMyHomeView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationPreviousDestinationsView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_previous',
		
	childViews: [
		'backButton',
		'voiceButton',
		'content',
		'heading'
	],
	  
	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	voiceButton: MFT.Button.extend({
		classNames:		  ['voiceButton','button'],		
		icon:				'images/settings/ico_voice.png',	
		disabled:			true
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'navigation_destination_heading_lable',
		classNames: 'block-header-title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_previous_header')
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled i1">1.</div>'+
			'<div class="label-20 disabled i2">2.</div>'+
			'<div class="label-20 disabled milerd">{{MFT.locale.label.view_navi_destination_previous_milerd}}</div>'+
			'<div class="label-20 disabled southfield">{{MFT.locale.label.view_navi_destination_previous_southfield}}</div>'+
			'<div class="label-20 disabled sortby">{{MFT.locale.label.view_navi_destination_favorites_sort}}</div>'+
			'<div class="label-20 disabled date">{{MFT.locale.label.view_navi_destination_previous_date}}</div>'+
			'<div class="label-20 disabled city">{{MFT.locale.label.view_navi_destination_previous_city}}</div>'+
			'<div class="label-20 disabled delete1">{{MFT.locale.label.view_navi_destination_favorites_delete}}</div>'+
			'<div class="label-20 disabled delete2">{{MFT.locale.label.view_navi_destination_favorites_delete}}</div>'
		)	
	})
});
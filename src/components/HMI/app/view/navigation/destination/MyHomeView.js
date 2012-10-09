/**
 * @name MFT.NavigationDestinationMyHomeView
 * 
 * @desc Navigation Destination My Home  module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/MyHomeView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationMyHomeView = Em.ContainerView.create({
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.navigation.dest.myHome.active:active_state','MFT.helpMode',],
	
	elementId:		'navigation_destination_myHome_view',
	
	classNames:	   ['hidden'],
	
	childViews: [
		'backButton',
		'content'
	],
	  
	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(
			'<div class="label-20 disabled view">{{MFT.locale.label.view_navi_destination_MyHome_view}}</div>'+
			'<div class="label-20 disabled set_a_dest">{{MFT.locale.label.view_navi_destination_MyHome_set_a_dest}}</div>'+
			'<div class="label-20 disabled dest">{{MFT.locale.label.view_navi_destination_MyHome_dest}}</div>'+
			'<div class="label-20 disabled set_as_way">{{MFT.locale.label.view_navi_destination_MyHome_set_as_way}}</div>'+
			'<div class="label-20 disabled menu">{{MFT.locale.label.view_navi_destination_MyHome_menu}}</div>'+
			'<div class="label-20 disabled my_home">{{MFT.locale.label.view_navi_destination_MyHome_my_home}}</div>'+
			'<div class="label-20 disabled rd">{{MFT.locale.label.view_navi_destination_MyHome_rd}}</div>'+
			'<div class="label-20 disabled dial">{{MFT.locale.label.view_navi_destination_MyHome_dial}}</div>'+
			'<div class="label-20 disabled delete">{{MFT.locale.label.view_navi_destination_MyHome_delete}}</div>'+
			'<div class="label-20 disabled edit">{{MFT.locale.label.view_navi_destination_MyHome_edit}}</div>'
		)	
	})
});
/**
 * @name MFT.NavigationDestinationLatitudeLongitudeView
 * 
 * @desc Navigation Destination Latitude Longitude module visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/destination/LatitudeLongitudeView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationLatitudeLongitudeView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'view_navi_destination_latitudeLongitude',
		
	childViews: [
		'backButton',
		'content'
	],
	  
	backButton: MFT.Button.extend({
		elementId: 			'navi_destination_latitudeLongitude_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),

	content: Em.View.create({
		classNames: 'content',
		template: Ember.Handlebars.compile(	
		
			'<div class="label-20 disabled dd">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_dd}}</div>'+
			'<div class="label-20 disabled latitude">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_latitude}}</div>'+
			'<div class="label-20 disabled longitude">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_longitude}}</div>'+
			'<div class="label-20 disabled go">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_go}}</div>'+
			'<div class="label-20 disabled e">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_e}}</div>'+
			'<div class="label-20 disabled n">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_n}}</div>'+
			'<div class="label-20 disabled s">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_s}}</div>'+
			'<div class="label-20 disabled w">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_w}}</div>'+

			'<div class="label-20 disabled n1">1</div>'+
			'<div class="label-20 disabled n2">2</div>'+
			'<div class="label-20 disabled n3">3</div>'+
			'<div class="label-20 disabled n4">4</div>'+
			'<div class="label-20 disabled n5">5</div>'+
			'<div class="label-20 disabled n6">6</div>'+
			'<div class="label-20 disabled n7">7</div>'+
			'<div class="label-20 disabled n8">8</div>'+
			'<div class="label-20 disabled n9">9</div>'+
			'<div class="label-20 disabled n0">0</div>'+

			'<div class="label-20 disabled z1">00</div>'+
			'<div class="label-20 disabled z2">00</div>'+
			'<div class="label-20 disabled z3">00</div>'+
			'<div class="label-20 disabled z4">00</div>'+

			'<div class="label-20 disabled wn1">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_n}}</div>'+
			'<div class="label-20 disabled wn2">{{MFT.locale.label.view_navi_destination_LatitudeLongitude_w}}</div>'+

			'<div class="label-20 disabled d1">.</div>'+
			'<div class="label-20 disabled d2">.</div>'+

			'<div class="label-20 disabled mn1">o</div>'+
			'<div class="label-20 disabled mn2">o</div>'
			
		)
	})
});
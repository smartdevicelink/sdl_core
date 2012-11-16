/**
 * @name MFT.SettingsSettingsWirelessInternetView
 * 
 * @desc Settings.Settings.WirelessInternet module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsWirelessInternetView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSettingsWirelessInternetView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	   ['MFT.helpMode'],
	controller:			  'MFT.SettingsController',
	elementId:			   'settings_settings_wirelessInternet',
	childViews:[
		'backButton',
		'view',
	],	
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'back',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	view: Em.View.extend({
		  
		 classNames:	  	'view',
		 template: Ember.Handlebars.compile(
			'<div class="block-header-title">{{MFT.locale.label.view_settings_settings_WirelessInternet_header}}</div>'+
			
			'<div class="inner-content">'+
				'<div class="label-20 disabled wifi-network">{{MFT.locale.label.view_settings_settings_WirelessInternet_wifi_settings}}</div>'+
				'<div class="label-20 disabled usbmb">{{MFT.locale.label.view_settings_settings_WirelessInternet_usbmb}}</div>'+
				'<div class="label-20 disabled bluetooth_settings">{{MFT.locale.label.view_settings_settings_WirelessInternet_bluetooth_settings}}</div>'+
				'<div class="label-20 disabled bluetooth_pcm">{{MFT.locale.label.view_settings_settings_WirelessInternet_bluetooth_pcm}}</div>'+
				'<div class="label-20 i">{{MFT.locale.label.view_settings_settings_WirelessInternet_bluetooth_i}}</div>'+
				'<div class="label-20 wifi">{{MFT.locale.label.view_settings_settings_WirelessInternet_bluetooth_wifi}}</div>'+
				'<div class="label-20 wdp">{{MFT.locale.label.view_settings_settings_WirelessInternet_bluetooth_wdp}}</div>'+
				'<div class="label-20 cn">{{MFT.locale.label.view_settings_settings_WirelessInternet_bluetooth_cn}}</div>'+
			'</div>'
		  )	
	})
});

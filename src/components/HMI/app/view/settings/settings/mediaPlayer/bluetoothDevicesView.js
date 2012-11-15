/**
 * @name MFT.SettingsSettingsMediaPlayerBluetoothDevicesView
 * 
 * @desc Settings Settings Media Player BluetoothDevicesView module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsMediaPlayerBluetoothDevicesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsMediaPlayerBluetoothDevicesView = Em.ContainerView.create(MFT.LoadableView,{
	elementId:			'settings_settings_mediaPlayer_bluetoothDevices',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],	
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'back',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices_title'),
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_mediaPlayer_bluetoothDevices_list',
			
			itemsOnPage:	5,
			
			disableScrollbar: true,
			
			items:[
			{
								
				type:		 MFT.Button,
				params:		{
					textBinding: 			'MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices_bluetoothDevices',
					templateName:		   'arrow',
					disabled:			   true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					labelContentBinding:   'MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices_bluetooth',
					tButtonValue:		  0,
					tButtonRange: 		  2,
					tButtonLabelsBinding:  'MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices_onOf',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
					textBinding:           'MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices_InternetDataC',
					templateName: 		  'arrow',
					disabled:			  true
				}
			}
		]
	})	

		
});
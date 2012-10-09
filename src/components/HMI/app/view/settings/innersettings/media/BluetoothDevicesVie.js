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

MFT.SettingsSettingsMediaPlayerBluetoothDevicesView = Em.ContainerView.create({
	classNameBindings:	['MFT.States.settings.settings.mediaPlayer.bluetoothDevices.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
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
		content:			 'Bluetooth App'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_mediaPlayer_bluetoothDevices_list',
			
			itemsOnPage:	5,
			
			disableScrollbar: true,
			
			items:[
			{
								
				type:		 MFT.Button,
				params:		{
					text:	'Bluetooth Devices',
					arrow:	   true,
					disabled:	true
				}			
					
			},
			{
								
				type:		 MFT.Label,
				params:		{
					content:	 'Bluetooth',
					icon:		'images/settings/ico_on-off-wide.png',
					disabled:    true
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
					text:    'Internet Data Connections',
					arrow:	   true,
					disabled:	true
				}
			}
		]
	})	

		
});
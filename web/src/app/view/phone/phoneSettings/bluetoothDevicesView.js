/**
 * @name MFT.PhoneSettingsBluetoothDevicesView
 * 
 * @desc Phone Settings Bluetooth Devices module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/options/BluetoothDevicesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.PhoneSettingsBluetoothDevicesView= Em.ContainerView.create( MFT.LoadableView, {
	
	elementId:			'phone_settings_bluetoothDevices',
	
	classNames:			['phone_view'],
	
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'bluetoothLogo',
							  'list',
							  'rightMenu'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'phone_settings_bluetoothDevices_backButton',
		classNames:		   ['backButton','button'],
		stateName:   		'dialpad',	
		action:			   'subState',
		target:			   'MFT.PhoneController',	
		icon:				'images/media/ico_back.png'
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'phone_settings_bluetoothDevices_titile',
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_title'
	}),	
	
	bluetoothLogo: Em.View.extend({
		classNames:		  ['bluetoothLogo']
	}),			  
	
	list:	MFT.List.extend({
			
			elementId:	  'phone_settings_BluetoothDevices_list',
			
			itemsOnPage:	6,
			
			columnsNumber:  1,
						
			items:[
				
				{								
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightIcon',
						classNames: ['active','ffw-button','notpressed','list-item'],
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_PaulPhone',
						righticon:   'images/phone/ico_star.png',
						icon:		'images/phone/ico_phoneMelody.png',
						disabled: true
						
	
					}			
						
				},
				{								
					type:		MFT.Button,
					
					params:		{
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_TonyaCell',
						icon:		'images/phone/ico_cellPhone.png',
						disabled: true
					}			
						
				}
			]
	}),
	
	rightMenu: Em.ContainerView.create({
		/** View ID */
		elementId:	'view_phone_phoneSettings_bluetoothDevices_rightmenu',

		/** Class Names */
		classNames: ['right-menu'],
		
		/** View Components*/
		childViews: [
			'addDeviceButton',
			'connectButton',
			'disconnectButton',
			'infoButton',
			'favoriteButton',
			'deleteButton'
		],
		
		addDeviceButton: MFT.Button.extend({
			classNames:		   'menu-item button',
			textBinding:		  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_addDevice',
			disabled:			 true,
			templateName:	'arrow'
		}),
		
		connectButton: MFT.Button.extend({
			classNames:		   'menu-item button',
			textBinding:		  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_connect',
			disabled:			 true
		}),
		
		disconnectButton: MFT.Button.extend({
			classNames:		   'menu-item button',
			textBinding:		  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_disconnect',
			disabled:			 true
		}),
		
		infoButton: MFT.Button.extend({
			classNames:		   'menu-item button',
			templateName:	'arrow',
			textBinding:		  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_info',
			disabled:			 true
		}),
		
		favoriteButton: MFT.Button.extend({
			classNames:		   'menu-item button',
			textBinding:		  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_favorite',
			disabled:			 true
		}),

		deleteButton: MFT.Button.extend({
			classNames:		   'menu-item button',
			textBinding:		  'MFT.locale.label.view_phone_phoneSettings_bluetoothDevices_delete',
			disabled:			 true
		})
	})
});
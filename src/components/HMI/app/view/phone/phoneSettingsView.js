/**
 * @name MFT.SettingsPhoneView
 * 
 * @desc Phone Settings module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/SettingsView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.SettingsPhoneView = Em.ContainerView.create( MFT.LoadableView, {
		
	elementId:	'phone_settings',
				
	classNames: ['hidden'],
	
	stateObj: MFT.States.phone.phoneSettings,

	childViews: [
		'help',
		'content'
	],
	
	help: Em.ContainerView.extend({
		elementId:			'phone_settings_help',
		
		classNames:			'help',
		
		classNameBindings: ['MFT.helpMode:visible'],
		
		childViews: [
			'dndButton'
		],
		
		dndButton: MFT.Button.extend({
			
			elementId:		'phone_settings_help_dndButton',
			
			action:			'onDND',
			target:			'MFT.PhoneController',
			
			text:			'Do Not Disturb',						
			icon:			'images/common/toggle.png',
			
			onDown: false
		
		})
	}),
	
	content: Em.ContainerView.extend({
		
		elementId:	'phone_settings_content',
		
		childViews: [
			'list'
		],
	
		list: MFT.List.extend({
		
			elementId:		'phone_settings_list',
			
			items:	[
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Bluetooth Devices',
						disabledBinding:	'MFT.helpMode',	
						
						arrow:		true,
						goToState:   'bluetoothDevices',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',

					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Bluetooth',
						disabled:	true,
						
						icon:		'images/common/toggle.png'
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Do Not Disturb',
						disabled:	true,
						
						icon:		'images/common/toggle.png'
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'911 Assist',
						disabled:	true,
						
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Phone Ringer',
						disabledBinding:	'MFT.helpMode',	
						
						arrow:		true,
						goToState:   'phoneRinger',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',

					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Text Message Notification',
						disabledBinding:	'MFT.helpMode',
						
						arrow:		true,
						
						goToState:   'textMessage',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Internet Data Connection',
						disabledBinding:	'MFT.helpMode',
						
						arrow:		true,
						
						goToState:   'internetDataConnection',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',

						text:		'Manage Phonebook',
						
						arrow:		true,
						
						goToState:   'managePhoneBook',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Roaming Warning',
						disabled:	true,
						
						icon:		'images/common/toggle.png'
					}		
				}
			],
			
			itemsOnPage:	6
		})
	})
});
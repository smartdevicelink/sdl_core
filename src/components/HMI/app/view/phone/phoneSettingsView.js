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
		
	childViews: [
		'help',
		'content'
	],
	
	help: Em.ContainerView.extend({
		elementId:			'phone_settings_help',
		
		classNames:			['blured_background','shifted'],

		childViews: [
			'dndButton'
		],
		
		dndButton: MFT.Button.extend({
			
			elementId:		'phone_settings_help_dndButton',
			
			classNames:		['helpmode_box_shadow','active_in_helpmode'],
			
			action:			'onDND',
			target:			'MFT.PhoneController',
			
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_phone_settings_doNotDisturb'),					
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
						textBinding: 'MFT.locale.label.view_phone_settings_bluetoothDevices',
						
						arrow:		true,
						goToState:   'bluetoothDevices',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',

					}				
				},
				{									
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_phone_settings_bluetooth',
						tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_phone_settings_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				},
				{									
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_phone_settings_doNotDisturb',
						tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_phone_settings_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						textBinding: 'MFT.locale.label.view_phone_settings_911Assist',
						disabled:	true,
						
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						textBinding: 'MFT.locale.label.view_phone_settings_phoneRinger',
						
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
						textBinding: 'MFT.locale.label.view_phone_settings_textMessageN',
						
						arrow:		true,
						
						goToState:   'textMessage',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						lableToggleClases: 'lableToggle button',
						templateName:	'arrow',
						textBinding: 'MFT.locale.label.view_phone_settings_internetDataC',
						
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

						textBinding: 'MFT.locale.label.view_phone_settings_managePhonebook',
						
						arrow:		true,
						
						goToState:   'managePhoneBook',
						action:	  'onChildState',
						target:	  'MFT.PhoneController',
					}				
				},
				{									
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_phone_settings_roamingWarning',
						tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_phone_settings_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				}
			],
			
			itemsOnPage:	6
		})
	})
});
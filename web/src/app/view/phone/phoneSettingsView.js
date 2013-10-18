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
		'content'
	],
	
	content: Em.ContainerView.extend({
		
		elementId:	'phone_settings_content',

        classNameBindings: [
            'MFT.States.phone.phoneSettings.bluetoothDevices.active:hidden',
            'MFT.States.phone.phoneSettings.phoneRinger.active:hidden',
            'MFT.States.phone.phoneSettings.textMessage.active:hidden',
            'MFT.States.phone.phoneSettings.internetDataConnection.active:hidden'
        ],
		
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
						tButtonValueBinding:  'MFT.PhoneController.dndValue',
						
						//tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_phone_settings_onOff',
						tButtonDisabledBinding:	'MFT.helpMode',
						classNames:			['ember-view lableToggleButton list-item'],
                        actionUp: function(){
                            if(MFT.helpMode){
                                MFT.VideoPlayerController.start('phone_Do_not_disturb');
                            }
                        },
                        classNameBindings: 'MFT.helpMode:helpmode_box_shadow'
					}
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:'arrow',
						textBinding: 'MFT.locale.label.view_phone_settings_911Assist',
                        classNameBindings: 'MFT.SettingsModel.isSpanish:hidden',
						goToState:   'help.helpAssist',
						action:	  'onState',
						target:	  'MFT.SettingsController',
						
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
						tButtonValue:		 1,
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
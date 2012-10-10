/**
 * @name MFT.SettingsSettingsSystemKeyboardLayoutView
 * 
 * @desc Settings Settings System Keyboard Layout module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsSystemKeyboardLayoutView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsSystemKeyboardLayoutView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.States.settings.settings.system.keyboardLayout.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_system_keyboardLayout',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	stateObj: MFT.States.settings.settings.system.keyboardLayout,
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'back',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout',
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_system_keyboardLayout_list',
			
			itemsOnPage:	5,
			
			disableScrollbarBinding: 'MFT.helpMode',
			
			items:[
			{
								
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABC',
					disabled:	true
				}			
					
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_QWERTY',
					disabled:	true
				}								
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCEurope',
					disabled:	true
				}	
			},
				{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCFrench',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCSpanish',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCPortuguese',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCGerman',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCItalian',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCDutch',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_ABCSwedish',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_chinesePinYin',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_chineseShouzimu',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_russianCyrillic',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_arabic',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_greek',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_polish',
					disabled:	true
				}	
			},
			{
				type:		 MFT.Button,
				params:		{
					textBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout_turkish',
					disabled:	true
				}	
			}
		]
	})	
});
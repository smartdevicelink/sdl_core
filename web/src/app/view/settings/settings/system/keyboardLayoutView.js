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
	classNameBindings:	['MFT.helpMode'],
	elementId:			'settings_settings_system_keyboardLayout',
    classNames: 'settings-list-block',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
    backButton: MFT.Button.extend({
        elementId: 'settings_settings_system_keyboardLayout_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_system_keyboardLayout_title',
		classNames:		  ['block-header-title'],
		contentBinding:	 'MFT.locale.label.view_settings_settings_system_keyboardLayout'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_system_keyboardLayout_list',
			
			itemsOnPage:	5,
			
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
/**
 * @name MFT.SettingsSettingsSystemView
 * 
 * @desc Settings Settings System module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsSystemView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsSystemView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.States.settings.settings.system.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_system',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	stateObj: MFT.States.settings.settings.system,			  
	backButton: MFT.Button.extend({
	classNames:		   ['backButton','button'],		
	action:			   'back',
	target:			   'MFT.States',	
	icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	 'MFT.locale.label.view_settings_settings_system'		
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_system_list',
			
			classNameBindings:	'MFT.helpMode:hidden',
			
			itemsOnPage:	5,
			
			disableScrollbarBinding: 'MFT.helpMode',
			
			items:[
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_settings_system_language',
					icon:		'images/settings/ico_languages.png',
					disabled:    true
				}			
					
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_settings_system_distance',
					icon:		'images/settings/ico_distance.png',
					disabled:    true
				}							
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_settings_system_temperature',
					icon:		'images/settings/ico_temperature.png',
					disabled:    true
				}
			},
				{
				type:		MFT.Label,
				params:		{
					classNames:  ['list-item','big-ico'],
					contentBinding:	 'MFT.locale.label.view_settings_settings_system_systemPrompt',
					icon:	    'images/settings/ico_plus-minus-pannel-disabled.png',
					disabled:    true
				}
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_settings_system_touch',
					icon:	   'images/settings/ico_on-off-wide.png',
					disabled:    true
				}
			},
			{
				type:		MFT.Label,
				params:		{
						content:	'Touch Panel Button Beep',
					icon:	   'images/settings/ico_on-off-wide.png',
					disabled:    true
				}
			},
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_settings_system_keyboardLayout',
					arrow:	   true,
					goToState:   'keyboardLayout',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_settings_system_installApplications',
					arrow:	  true,
					goToState:  'installApplications',
					action:	 'onChildState',
					target:	 'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:   'button',
					text:		'Master Reset',
					disabled:	true
				}
			}
		]
	})
});
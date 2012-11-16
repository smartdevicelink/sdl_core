/**
 * @name MFT.SettingsSettingsView
 * 
 * @desc Settings Settings module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'settings_settings',
	
	classNameBindings: ['MFT.helpMode'],
			
	childViews: [
		'listSettingsSettings'
	],
	
	listSettingsSettings: MFT.List.extend({
		elementId:		   'settings_settings_list',
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'system',
					icon:		'images/settings/ico_settings_white.png',
					textBinding: 'MFT.locale.label.view_settings_settings_system',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
					onDown: false

				}			
					
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'voiceControl',
					icon:		'images/settings/ico_voice.png',
					textBinding: 'MFT.locale.label.view_settings_settings_voiceControl',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
					onDown: false

				}							
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'mediaPlayer',
					icon:		'images/settings/ico_player.png',
					textBinding: 'MFT.locale.label.view_settings_settings_mediaPlayer',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
					onDown: false
				}
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'snavigation',
					icon:		'images/settings/ico_navigation.png',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					stateName:   'phoneSettings',
					icon:		'images/settings/ico_phone.png',
					textBinding: 'MFT.locale.label.view_settings_settings_phone',
					templateName: 'arrow',
					action:		'subState',
					target:		'MFT.PhoneController',
					helpMode: true,
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'wirelessInternet',
					icon:		'images/settings/ico_wireless.png',
					textBinding: 'MFT.locale.label.view_settings_settings_wirelessInternet',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
					onDown: false
				}			
			}
		],
		
		itemsOnPage:	6
	})
});

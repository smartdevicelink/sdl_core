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
	
	classNameBindings: ['MFT.States.settings.settings.active:active_state','MFT.helpMode','MFT.helpMode'],
	
	classNames: 'hidden',
	
	stateObj: MFT.States.settings.settings,
	
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
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false

				}			
					
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'voiceControl',
					icon:		'images/settings/ico_voice.png',
					textBinding: 'MFT.locale.label.view_settings_settings_voiceControl',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false

				}							
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'mediaPlayer',
					icon:		'images/settings/ico_player.png',
					textBinding: 'MFT.locale.label.view_settings_settings_mediaPlayer',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false
				}
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'snavigation',
					icon:		'images/settings/ico_navigation.png',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					stateName:   'phoneSettings',
					icon:		'images/settings/ico_phone.png',
					textBinding: 'MFT.locale.label.view_settings_settings_phone',
					arrow:		true,
					action:		'subState',
					target:		'MFT.PhoneController',
					disabledBinding: 'MFT.helpMode',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'wirelessInternet',
					icon:		'images/settings/ico_wireless.png',
					textBinding: 'MFT.locale.label.view_settings_settings_wirelessInternet',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode',
					onDown: false
				}			
			}
		],
		
		itemsOnPage:	6
	})
});

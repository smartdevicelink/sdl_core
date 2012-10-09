/**
 * @name MFT.SettingsHelpView
 * 
 * @desc Settings Help module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsHelpView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsHelpView = Em.ContainerView.create({
	
	elementId:	'settings_help',
	
	classNameBindings: ['MFT.States.settings.help.active:active_state','MFT.helpMode'],
	
	classNames: 'hidden',
	
	childViews: [
		'listLeft',
		'listRight',
		'bottomLabel'
	],
	
	listLeft: MFT.List.extend({
		elementId:		   'settings_help_list_left',
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'whereAmI',
					icon:		'images/settings/ico_upArrow.png',
					text:		'Where am I?',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'

				}			
					
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'systemInformation',
					icon:		'images/settings/ico_settings_white.png',
					text:		'System Information',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    disabledBinding: 'MFT.helpMode'
				}							
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'softwareLicenses',
					icon:		'images/settings/ico_licenses.png',
					text:		'Software Licenses',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    disabledBinding: 'MFT.helpMode'
				}
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'drivingRestrictions',
					icon:		'images/settings/ico_forbiddance.png',
					text:		'Driving Restrictions',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    disabledBinding: 'MFT.helpMode'
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'assist',
					icon:		'images/settings/ico_assist.png',					
					text:		'911 Assist',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    disabledBinding: 'MFT.helpMode'
				}			
			}
		],
		
		itemsOnPage:	5
	}),
	
	listRight: MFT.List.extend({
		elementId:		   'settings_help_list_right',
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{
				type:		MFT.Button,

				params:		{
					goToState:   'voiceCommandList',
					icon:		'images/settings/ico_voice.png',					
					text:		'Voice Command List',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    disabledBinding: 'MFT.helpMode'
				}			
			}
		]
	}),
	
	bottomLabel: MFT.Label.extend({
		elementId:		   'settings_help_bottom_label',
		content:			 'For help contact SYNC Support 1-877-945-3648'
	})
});
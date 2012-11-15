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

MFT.SettingsHelpView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'settings_help',
		
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
					textBinding: 'MFT.locale.label.view_settings_settings_whereAmI',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true

				}			
					
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'systemInformation',
					icon:		'images/settings/ico_settings_white.png',
					textBinding: 'MFT.locale.label.view_settings_settings_systemInformation',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    helpMode: true
				}							
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'softwareLicenses',
					icon:		'images/settings/ico_licenses.png',
					textBinding: 'MFT.locale.label.view_settings_settings_softwareLicenses',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    helpMode: true
				}
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'drivingRestrictions',
					icon:		'images/settings/ico_forbiddance.png',
					textBinding: 'MFT.locale.label.view_settings_settings_drivingRestrictions',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    helpMode: true
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'helpAssist',
					icon:		'images/settings/ico_assist.png',					
					textBinding: 'MFT.locale.label.view_settings_settings_911Assist',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    helpMode: true
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
					textBinding: 'MFT.locale.label.view_settings_settings_voiceCommandList',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
				    helpMode: true
				}			
			}
		]
	}),
	
	bottomLabel: MFT.Label.extend({
		elementId:		   'settings_help_bottom_label',
		contentBinding: 	  Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_forHelp')
	})
});
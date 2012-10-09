/**
 * @name MFT.SettingsSettingsNavigationView
 * 
 * @desc Settings Settings Navigation module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/innersettings/navigation/SettingsSettingsNavigationView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsNavigationView = Em.ContainerView.create({
	
	elementId:	'settings_settings_navigation',
	
	classNameBindings: ['MFT.States.settings.settings.snavigation.active:active_state','MFT.helpMode'],
	
	classNames: 'hidden',
	
	childViews: [
		'backButton',
		'topTitile',
		'list'
	],
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_title'
	}),	
	
	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	list: MFT.List.extend({
		elementId:		   'settings_settings_navigation_list',
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'mapPreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_mapPreferences',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'routePreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'navigationPreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_navigationPreferences',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'trafficPreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'avoidAreas',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_avoidAreas',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			}
		],
		
		itemsOnPage:	6
	})
});
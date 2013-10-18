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

MFT.SettingsSettingsNavigationView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'settings_settings_navigation',
    classNames: 'settings-list-block',
				
	childViews: [
		'backButton',
		'topTitile',
		'list'
	],
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_navigation_titile',
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_title'
	}),	
	
	backButton: MFT.Button.extend({
		elementId: 			'settings_settings_navigation_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'settingNavigationBack',
		target:			  'MFT.SettingsController',	
		icon:				'images/media/ico_back.png'
	}),
	
	list: MFT.List.extend({
		elementId:		   'settings_settings_navigation_list',
		classNameBindings: ['MFT.helpMode:help-mode'],
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'mapPreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_mapPreferences',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'routePreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'navigationPreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_navigationPreferences',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'trafficPreferences',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}			
					
			},
			{								
				type:		MFT.Button,
				
				params:		{
					goToState:   'avoidAreas',
					textBinding: 'MFT.locale.label.view_settings_settings_navigation_avoidAreas',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}			
					
			}
		],
		
		itemsOnPage:	6
	})
});
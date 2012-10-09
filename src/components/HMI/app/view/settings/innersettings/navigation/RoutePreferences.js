/**
 * @name MFT.SettingsSettingsNavigationRoutePreferencesView
 * 
 * @desc Settings.Settings.Navigation.RoutePreferences module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/innersettings/navigation/SettingsSettingsNavigationRoutePreferencesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSettingsNavigationRoutePreferencesView = Em.ContainerView.create({
	classNameBindings:	['MFT.States.settings.settings.snavigation.routePreferences.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_navigation_routePreferences',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	backButton: MFT.Button.extend({
	classNames:		   ['backButton','button'],		
	action:			   'back',
	target:			   'MFT.States',	
	icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_routePreferences'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_navigation_routePreferences_list',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_routePreferences_preferred',
					tButtonValue:		 1,
					tButtonRange: 		 3,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences_SFE',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_routePreferences_avoidFreeways',
					labelIcon:		    'images/settings/ico_settings_settings_navigation_routePreferences_avoidFreeways.png',
					tButtonValue: 		 1,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}		
								
			},	
			{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_routePreferences_avoidTollroads',
					labelIcon:		    'images/settings/ico_settings_settings_navigation_routePreferences_avoidTollroads.png',
					tButtonValue: 		 1,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}		
								
			},
			{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_routePreferences_avoidFerriesCarTrains',
					labelIcon:		    'images/settings/ico_settings_settings_navigation_routePreferences_avoidFerries.png',
					tButtonValue: 		 1,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}		
								
			},
			{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_routePreferences_useHOVLanes',
					labelIcon:		    'images/settings/ico_settings_settings_navigation_routePreferences_useHOV.png',
					tButtonValue: 		 1,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_routePreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}		
								
			},				
		]
	})
});
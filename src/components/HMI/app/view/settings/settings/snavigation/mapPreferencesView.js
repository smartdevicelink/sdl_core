/**
 * @name MFT.SettingsSettingsNavigationMapPreferencesView
 * 
 * @desc Settings.Settings.Navigation.MapPreferences module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/innersettings/navigation/SettingsSettingsNavigationMapPreferencesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSettingsNavigationMapPreferencesView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.States.settings.settings.snavigation.mapPreferences.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_navigation_mapPreferences',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	stateObj: MFT.States.settings.settings.snavigation.mapPreferences,
	
	backButton: MFT.Button.extend({
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_mapPreferences'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_navigation_mapPreferences_list',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_mapPreferences_breadcrumbs',
					tButtonValue:		 1,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_mapPreferences_OnOFF',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}			
					
			},
			{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_mapPreferences_turnList',
					tButtonValue: 		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_mapPreferences_breadcrumbs_TopBotttom',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}		
								
			},
						{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_mapPreferences_ParkingPOI',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_mapPreferences_OnOFF',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}		
								
			},
		]
	})
});
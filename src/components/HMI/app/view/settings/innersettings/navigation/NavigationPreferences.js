/**
 * @name MFT.SettingsSettingsNavigationNavigationPreferencesView
 * 
 * @desc Settings.Settings.Navigation.NavigationPreferences module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/innersettings/navigation/SettingsSettingsNavigationNavigationPreferencesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSettingsNavigationNavigationPreferencesView = Em.ContainerView.create({
	classNameBindings:	['MFT.States.settings.settings.snavigation.navigationPreferences.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_navigation_navigationPreferences',
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
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_navigationPreferences'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_navigation_navigationPreferences_list',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_guidancePrompts',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_voiceTone',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}			
					
			},
			{
				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_auto',
					tButtonValue: 		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_connectionToggle',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}		
								
			},							
		]
	})
});
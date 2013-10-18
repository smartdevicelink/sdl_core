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
 
MFT.SettingsSettingsNavigationNavigationPreferencesView = Em.ContainerView.create(MFT.LoadableView,{
	elementId:			'settings_settings_navigation_navigationPreferences',
    classNames: 'settings-list-block',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	backButton: MFT.Button.extend({
		elementId: 		   'settings_settings_navigation_navigationPreferences_backButton',
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_navigation_navigationPreferences_titile',
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_title'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_navigation_navigationPreferences_list',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_guidancePrompts',
					tButtonValue:		 1,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_voiceTone',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}			
					
			},
			{
				
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_auto',
					tButtonValue: 		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_navigationPreferences_onOff',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}		
								
			}
		]
	})
});
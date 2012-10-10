/**
 * @name MFT.SettingsSettingsNavigationTrafficPreferencesView
 * 
 * @desc Settings.Settings.Navigation.TrafficPreferences module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/innersettings/navigation/TrafficPreferences.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSettingsNavigationTrafficPreferencesView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.States.settings.settings.snavigation.trafficPreferences.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_navigation_trafficPreferences',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	stateObj: MFT.States.settings.settings.snavigation.trafficPreferences,
	backButton: MFT.Button.extend({
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_navigation_trafficPreferences_list',
			
			itemsOnPage:	5,
			
			disableScrollbarBinding: 'MFT.helpMode',
			
			items:[
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_avoidTrafficProblems',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_automaticManual',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_trafficAlertNotification',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_accidentMapIcon',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_acciden.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_trafficJamMapIcon',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_trafficJam.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_roadClosed',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_roadClosed.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_roadWorks',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_roadWorks.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_incidentMapIcons',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_incident.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_difficultDrivingConditions',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_difficultDriving.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_snowIce',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_snow.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_smogAlerts',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_smog.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_weatherWarning',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_weatherWarning.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_reducesVisibility',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_reducesVisibility.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_turnOnYourRadio',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_turnOnYourRadio.png',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
		]
	})
});
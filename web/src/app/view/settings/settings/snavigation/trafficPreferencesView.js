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
	elementId:			'settings_settings_navigation_trafficPreferences',
    classNames: 'settings-list-block',

	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
						  
	backButton: MFT.Button.extend({
		elementId: 		   'settings_settings_navigation_trafficPreferences_backButton',	
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_navigation_trafficPreferences_titile',
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_title'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_navigation_trafficPreferences_list',
			
			itemsOnPage:	5,
			
			items:[
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_avoidTrafficProblems',
					tButtonValue:		 1,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_automaticManual',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_trafficAlertNotification',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_accidentMapIcon',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_acciden.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_trafficJamMapIcon',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_trafficJam.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_roadClosed',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_roadClosed.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_roadWorks',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_roadWorks.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_incidentMapIcons',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_incident.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_difficultDrivingConditions',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_difficultDriving.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_snowIce',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_snow.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_smogAlerts',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_smog.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_weatherWarning',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_weatherWarning.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_reducesVisibility',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_reducesVisibility.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			},
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_turnOnYourRadio',
					labelIcon:		    'images/settings/trafficPreferences_ico/ico_turnOnYourRadio.png',
					templateName: 'icon',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_navigation_trafficPreferences_onOff',
					labelDisabled: 		true,
					tButtonDisabled:	  true
				}			
					
			}
		]
	})
});
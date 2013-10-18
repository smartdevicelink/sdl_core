/**
 * @name MFT.SettingsSettingsSystemView
 * 
 * @desc Settings Settings System module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsSystemView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsSystemView = Em.ContainerView.create(MFT.LoadableView,{
	elementId:			'settings_settings_system',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list',
							  'MasterResetPopUp'
						  ],
    classNames:		   ['settings-list-block'],

	backButton: MFT.Button.extend({
        elementId: 			'settings_settings_system_backButton',
        classNames:		   ['backButton','button'],
        action:			   'back',
        target:			   'MFT.States',
        icon:				 'images/media/ico_back.png'
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_system_title',
		classNames:		  ['block-header-title'],
		contentBinding:	 'MFT.locale.label.view_settings_settings_system'		
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_system_list',
			
			itemsOnPage:	5,
			
			items:[
			{				
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:			['lableToggleButton', 'list-item', 'ember-view', 'tripleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_system_language',
					tButtonValueBinding:  'MFT.SettingsController.language',
					tButtonRange: 		 3,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_system_language_engEspFr',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}	
			},
			{				
				type:	MFT.LabelPlusToggleButton,
				params:{
					classNames:			['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
    					tButtonValueBinding:	'MFT.SettingsModel.distanceType',
					tButtonRange:			2,
					labelContentBinding:	'MFT.locale.label.view_settings_settings_system_distance',
					tButtonLabelsBinding:	'MFT.locale.label.view_settings_settings_system_distance_kmMi'
				}										
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					classNames:			['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					tButtonValueBinding:	'MFT.SettingsModel.temperatureType',
					tButtonRange:			2,
					labelContentBinding:	'MFT.locale.label.view_settings_settings_system_temperature',
					tButtonLabelsBinding:	'MFT.locale.label.view_settings_settings_system_temperature_frCel'
				}
			},
			{
				type:		MFT.Label,
				params:		{
					classNames:  ['list-item','big-ico'],
					contentBinding:	 'MFT.locale.label.view_settings_settings_system_systemPrompt',
					icon:	    'images/settings/ico_plus-minus-pannel-disabled.png',
					templateName: 'icon',
					disabled:    true
				}
			},
			{				
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:			['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_system_touchScreen',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_system_onOff',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}	
			},
			{				
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:			['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_system_touchPanel',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_system_onOff',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}	
			},
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_settings_system_keyboardLayout',
					templateName: 'arrow',
					goToState:   'keyboardLayout',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
					onDown: false
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_settings_system_installApplications',
					templateName: 'arrow',
					goToState:  'installApplications',
					action:	 'onChildState',
					target:	 'MFT.SettingsController',
					helpMode: true,
					onDown: false
				}
			},
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_settings_system_masterReset',
					action:	 'showResetToFactoryWarning',					
					target:	 'MFT.SettingsController'
				}
			}
		]
	}),
	
	MasterResetPopUp:  MFT.Label.extend({
		elementId: 'settings_settings_system_MasterResetNotifier_popUp',
		classNames: 'optionPopup',		
		contentBinding:	   Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_system_notifier_resetting')
	})
});
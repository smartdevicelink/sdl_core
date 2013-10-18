/**
 * @name MFT.SettingsHelpVoiceCommandListView
 * 
 * @desc Settings Help Voice Command List module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsHelpVoiceCommandListView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsHelpVoiceCommandListView = Em.ContainerView.create(MFT.LoadableView,{
	dataListBinding: 	  'MFT.SettingsData.clocklist',
	elementId:			'settings_help_voiceCommandList',
    classNames: 'settings-list-block',
	childViews:		   [
							  'backButton',
							  'topTitle',
							  'list'
						  ],
	
	backButton: MFT.Button.extend({
		elementId: 			'settings_help_voiceCommandList_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png'
	}),
	
	topTitle: MFT.Label.extend({
		elementId:		   'settings_help_voiceCommandListView_title',
		classNames:		  ['block-header-title'],
		contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandListView_header')
	}),				  
	
	list:	MFT.List.extend( {
			
			elementId:	  			  'settings_help_voiceCommandList_list',

            classNames: 'settings-list',
			
			itemsOnPage:	5,
			
			items:[
			{
								
				type:		 MFT.Button,
				params:		{
                    goToState:   'audio',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_audio',
                    templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'browse',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_browse',
                    templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}							
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'climate',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_climate',
                    templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
				{
				type:		MFT.Button,
				params:		{
                    goToState:   'disc',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_disc',
                    templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'globalCommands',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_globalCommands',
                    templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'media',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_media',
                    templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'navigationCommands',
                    action:	  'onChildState',
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_navigation',
					templateName: 'text',
					disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'otherCommands',
                    action:	  'onChildState',
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_otherCommands',
					templateName: 'text',
					disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'phone',
                    action:	  'onChildState',
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_phone',
					templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'radio',
                    action:	  'onChildState',
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_radio',
					templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'sirius',
                    action:	  'onChildState',
					className:  'button',
					classNameBindings:  ['MFT.SettingsModel.isSpanish:hidden'],
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_sirius',
					templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'travelLink',
                    action:	  'onChildState',
					className:  'button',
                    classNameBindings:  ['MFT.SettingsModel.isSpanish:hidden'],
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_travelLink',
					templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'voiceInstructions',
                    action:	  'onChildState',
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_voiceInstructions',
					templateName: 'text',
                    disabled:	false,
                    target:	  'MFT.SettingsController'
				}
			},
			{
				type:		MFT.Button,
				params:		{
                    goToState:   'voiceSettings',
                    action:	  'onChildState',
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_voiceSettings',
					templateName: 'text',
					disabled:	false,
                    target:	  'MFT.SettingsController'
                }
			}
			
		]
	})

});
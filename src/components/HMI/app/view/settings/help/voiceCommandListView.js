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
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandListView_header'),
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  			  'settings_help_voiceCommandList_list',
			
			classNameBindings:		  'MFT.helpMode:hidden',
			
			itemsOnPage:	5,
			
			disableScrollbarBinding:	'MFT.helpMode',
			
			items:[
			{
								
				type:		 MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_audio',
					templateName: 'text',
					disabled:	true
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_browse',
					templateName: 'text',
					disabled:	true
				}							
			},
			{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_climate',
					templateName: 'text',
					disabled:	true
				}
			},
				{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_disc',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_globalCommands',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_media',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_navigation',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_otherCommands',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_phone',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_radio',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_sirius',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_travelLink',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_voiceInteractions',
					templateName: 'text',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_voiceSettings',
					templateName: 'text',
					disabled:	true
				}
			}
			
		]
	})	

});
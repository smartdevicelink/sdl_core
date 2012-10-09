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

MFT.SettingsHelpVoiceCommandListView = Em.ContainerView.create({
	classNameBindings:	['MFT.States.settings.help.voiceCommandList.active:active_state'],
	classNames:		   ['hidden'],
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
		content:			 'List of Commands'
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
					disabled:	true
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_browse',
					disabled:	true
				}							
			},
			{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_climate',
					disabled:	true
				}
			},
				{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_disc',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_globalCommands',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_media',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_navigation',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_otherCommands',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_phone',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_radio',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_sirius',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_travelLink',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_voiceInteractions',
					disabled:	true
				}
			},
			{
				type:		MFT.Button,
				params:		{

					className:  'button',
					text:		'Global Commands',
					textBinding: 'MFT.locale.label.view_settings_help_voiceCommandListView_voiceSettings',
					disabled:	true
				}
			}
			
		]
	})	

		
});
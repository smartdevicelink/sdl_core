/**
 * @name MFT.SettingsHelpSystemInformationView
 * 
 * @desc Settings Help System Information module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsHelpSystemInformationView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsHelpSystemInformationView = Em.ContainerView.create({
	
	classNameBindings:	['MFT.States.settings.help.systemInformation.active:active_state'],
	classNames:		   ['hidden'],
	dataListBinding: 	  'MFT.SettingsData.clocklist',
	elementId:			'settings_help_systemInformation',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],	
						  						  
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'back',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		content:			 'System Information'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_help_systemInformation_list',
			
			classNameBindings: 'MFT.helpMode:hidden',
			
			itemsOnPage:	5,
								
			items:[
			{
								
				type:		 MFT.Label,
				params:		{

					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_serialNumber',
					disabled:	true
				}			
					
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_vehicleIdentification',
					disabled:	true
				}							
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_CCPU',
					disabled:	true
				}
			},
				{
				type:		MFT.Label,
				params:		{
					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_navigationApplication',
					disabled:	true
				}
			},
			{
				type:		MFT.Label,
				params:		{

					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_mapDatabase',
					disabled:	true
				}
			},
						{
				type:		MFT.Label,
				params:		{

					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_siriusESN',
					disabled:	true
				}
			},
						{
				type:		MFT.Label,
				params:		{

					contentBinding: 'MFT.locale.label.view_settings_help_systemInformation_gracenote',
					disabled:	true
				}
			}
		]
	})		
});
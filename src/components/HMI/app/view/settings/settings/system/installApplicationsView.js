/**
 * @name MFT.SettingsSettingsSystemInstallApplicationsView
 * 
 * @desc Settings Settings System Install Applications visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsSystemInstallApplicationsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsSystemInstallApplicationsView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.States.settings.settings.system.installApplications.active:active_state'],
	classNames:		   ['hidden'],
	elementId:			'settings_settings_system_installApplications',
	stateObj: 	MFT.States.settings.settings.system.installApplications,
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],	
						  
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'onPreviousState',
	target:			  'MFT.SettingsController',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_settings_system_installApplications_header'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_system_installApplications_list',
			
			itemsOnPage:	5,
			
			disableScrollbar: true,
			
			items:[
						{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding:	  'MFT.locale.label.view_settings_settings_system_installApplications_BeginSoftware',
					arrow:	  true,
					goToState:  'beginSoftwareInstallation',
					action:	 'onChildState',
					target:	 'MFT.SettingsController',
					disabled:   true
				}
			},			{
				type:		MFT.Button,
				params:		{
					className:  'button',
					textBinding:	  'MFT.locale.label.view_settings_settings_system_installApplications_ViewSoftware',
					arrow:	  true,
					goToState:  'viewSoftwareLicenses',
					action:	 'onChildState',
					target:	 'MFT.SettingsController',
					disabled:   true
				}
			}
			
		]
	})	
});
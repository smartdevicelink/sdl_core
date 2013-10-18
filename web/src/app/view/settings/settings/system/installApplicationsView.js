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
	elementId:			'settings_settings_system_installApplications',
    classNames: 'settings-list-block',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],	
						  
	backButton: MFT.Button.extend({
	elementId: 		   'settings_settings_system_installApplications_backButton',	
	classNames:		  ['backButton','button'],		
	action:			  'goToPreviousState',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png'
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_system_installApplications_title',
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
					templateName:	  'arrow',
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
					templateName:	  'arrow',
					goToState:  'viewSoftwareLicenses',
					action:	 'onChildState',
					target:	 'MFT.SettingsController',
					disabled:   true
				}
			}
			
		]
	})
});
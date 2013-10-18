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

MFT.SettingsHelpSystemInformationView = Em.ContainerView.create(MFT.LoadableView,{
	
	dataListBinding: 	  'MFT.SettingsData.clocklist',
	elementId:			'settings_help_systemInformation',
    classNames: 'settings-list-block',

    childViews: [
        'backButton',
        'topTitile',
        'list'
    ],
    backButton: MFT.Button.extend({
        elementId: 'settings_help_systemInformation_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_help_systemInformation_title',
		classNames:		  ['block-header-title'],
		contentBinding:      Ember.Binding.oneWay('MFT.locale.label.view_settings_help_systemInformation_title'),
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_help_systemInformation_list',
			
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
                    classNameBindings: 'MFT.SettingsModel.isEnglish::hidden',
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
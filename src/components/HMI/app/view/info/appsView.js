/**
 * @name MFT.InfoAppsview
 * 
 * @desc Info Apps visual representation
 * 
 * @category	View
 * @filesource	app/view/info/AppsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.InfoAppsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'info_apps',
	
	classNameBindings: ['MFT.helpMode',
						'MFT.localization'],
	
	//classNames: ['hidden'],
	
	//stateObj: MFT.States.info.apps,
	
	

	childViews: [
		'vehicleHealthReport',
		'Asist911',
		'installButton',
		'findNewApps', 
		'listOfOptions'
	],
	
	vehicleHealthReport:   MFT.Button.extend({
		goToState:			'vehicle.healthReport',
		classNames:			'button vehicleHealthReport leftButtons',
		icon:				'images/info/ico_vehicle.png',
		textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
		elementId:			'infoAppsVehicleHealthReport',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),
	
	Asist911:   MFT.Button.extend({
		goToState:			'help.helpAssist',
		classNames:			'button Asist911 leftButtons',
		icon:				'images/info/ico_assist.png',
		textBinding:		'MFT.locale.label.view_info_apps_911Assist',
		elementId:			'infoAppsAsist911',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),
	
	installButton:   MFT.Button.extend({
		goToState:  		'settings.system.installApplications',
		icon:				'images/info/ico_info_install.png',
		textBinding:		'MFT.locale.label.view_info_apps_vehicle_InstallApplicationsUp',
		elementId:			'infoAppsInstallButton',
		classNames:			'button installButton leftButtons',
		arrow:				true,
		action:				'onState',
		target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),
	
	findNewApps:   MFT.Button.extend({
		goToState:			'settings.system.installApplications',
		/*icon:				'images/info/ico_info_install.png',*/
		textBinding:		'MFT.locale.label.view_info_apps_vehicle_FindNewApplications',
		elementId:			'infoAppsFindNewApps',
		classNames:			'button findNewApps leftButtons',
		arrow:				true,
		//action:				'onState',
		//target:				'MFT.SettingsController',
		disabledBinding:	'MFT.helpMode',
		onDown:				false
	}),

	listOfOptions: MFT.List.extend({

        elementId:         'info_apps_list',

        itemsOnPage:    5,
                
        /** Items */
        items: [
            {
                type:       MFT.Button,
                params:     {
                    action:				MFT.InfoController.get('applicationList')[0].action,
					target:				MFT.InfoController.get('applicationList')[0].target,
                    text:				'Application',
                    className:			'ffw-button list-item',
                    templateName:       'text',
                    onDown:				false
                }                                   
            },
            {
                type:       MFT.Button,
                params:     {
                    action:             MFT.InfoController.get('applicationList')[0].action,
                    target:             MFT.InfoController.get('applicationList')[0].target,
                    text:               'Application',
                    className:          'ffw-button list-item',
                    templateName:       'text',
                    onDown:             false
                }                                   
            },
            {
                type:       MFT.Button,
                params:     {
                    action:             MFT.InfoController.get('applicationList')[0].action,
                    target:             MFT.InfoController.get('applicationList')[0].target,
                    text:               'Application',
                    className:          'ffw-button list-item',
                    templateName:       'text',
                    onDown:             false
                }                                   
            },
            {
                type:       MFT.Button,
                params:     {
                    action:             MFT.InfoController.get('applicationList')[0].action,
                    target:             MFT.InfoController.get('applicationList')[0].target,
                    text:               'Application',
                    className:          'ffw-button list-item',
                    templateName:       'text',
                    onDown:             false
                }                                   
            },
            {
                type:       MFT.Button,
                params:     {
                    action:             MFT.InfoController.get('applicationList')[0].action,
                    target:             MFT.InfoController.get('applicationList')[0].target,
                    text:               'Application',
                    className:          'ffw-button list-item',
                    templateName:       'text',
                    onDown:             false
                }                                   
            },
            {
                type:       MFT.Button,
                params:     {
                    action:             MFT.InfoController.get('applicationList')[0].action,
                    target:             MFT.InfoController.get('applicationList')[0].target,
                    text:               'Application',
                    className:          'ffw-button list-item',
                    templateName:       'text',
                    onDown:             false
                }                                   
            },
            {
                type:       MFT.Button,
                params:     {
                    action:             MFT.InfoController.get('applicationList')[0].action,
                    target:             MFT.InfoController.get('applicationList')[0].target,
                    text:               'Application',
                    className:          'ffw-button list-item',
                    templateName:       'text',
                    onDown:             false
                }                                   
            }
        ]
    })
});
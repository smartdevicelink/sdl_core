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

    appsActive: function(){
        if(MFT.States.info.apps.active == true){
            FFW.AppLinkCoreClient.getAppList();
        }
    }.observes('MFT.States.info.apps.active'),

	childViews: [
		'vehicleHealthReport',
		'Asist911',
		'installButton',
		'findNewApps',
        'getDeviceList',
		'listOfApplications'
	],
/*
    afterRender: function(){
        /*
         * Request, get List of applications
         */
       /* FFW.AppLinkCoreClient.getAppList();
    },
*/
    /** 
      * Function to add application to application list
      */
    ShowAppList: function( ){

        this.listOfApplications.items = MFT.ApplinkMediaModel.applicationsList.slice();

        this.listOfApplications.list.refresh();

    },
	
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
		onDown:				false,
        templateName:       'text'
	}),

    getDeviceList:   MFT.Button.extend({
        /*icon:             'images/info/ico_info_install.png',*/
        textBinding:        'MFT.locale.label.view_info_apps_vehicle_GetDeviceList',
        elementId:          'infoAppsGetDeviceList',
        classNames:         'button getDeviceList leftButtons',
        arrow:              true,
        action:             'onGetDeviceList',
        target:             'MFT.ApplinkController',
        //disabledBinding:    'MFT.helpMode',
        onDown:             false,
        templateName:       'text'
    }),

	listOfApplications: MFT.List.extend({

        elementId:      'info_apps_list',

        itemsOnPage:    5,
                
        /** Items */
        items:          new Array()
    })
});
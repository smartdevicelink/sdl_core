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
	
	classNameBindings: ['MFT.States.info.apps.active:active_state','MFT.helpMode'],
	
	classNames: ['hidden'],
	
	stateObj: MFT.States.info.apps,
	
	childViews: [
		'list',
		'installButton'
	],
	
	list: MFT.List.extend({
		elementId:		   'info_apps_list',
		
		disableScrollbarBinding:  'MFT.helpMode',
		
		itemsOnPage:		 6,
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
								
				params:		{
					goToState:   		  'vehicle.healthReport',
					classNames:         ['button','ember-view','notpressed','list-item'],
					icon:			   'images/info/ico_vehicle.png',
					textBinding: 	 	'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
					arrow:			  true,
					action:	  		 'onState',
					target:	  		 'MFT.SettingsController',
					disabledBinding: 	'MFT.helpMode',
					onDown: false
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:  		  'help.helpAssist',
					classNames:         ['button','ember-view','notpressed','list-item'],
					icon:			   'images/info/ico_assist.png',
					textBinding: 	    'MFT.locale.label.view_info_apps_911Assist',
					arrow:			  true,
					action:	  	  	 'onState',
					target:	  		 'MFT.SettingsController',
					disabledBinding: 	'MFT.helpMode',
					onDown: false
				}			
					
			}
		]
	}),
	
	installButton:   MFT.Button.extend({
					goToState:  		  'settings.system.installApplications',
					icon:			   'images/info/ico_info_install.png',
					textBinding: 		'MFT.locale.label.view_info_apps_vehicle_InstallApplicationsUp',
					elementId: 	  	  'info_apps_install_Button',
					classNames: 	 	 'button',
					arrow:			  true,
					action:	  	  	 'onState',
					target:	  		 'MFT.SettingsController',
					disabledBinding: 	'MFT.helpMode',
					onDown: false
	})
});
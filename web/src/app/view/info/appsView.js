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
			
	childViews: [
		'list',
		'installButton'
	],
	
	list: MFT.List.extend({
		elementId:		   'info_apps_list',
		
		itemsOnPage:		 6,
		
		disableVehicleHealthMenu: false,
		
		init: function(){
			this._super();
			if(!MFT.SettingsModel.isEnglish){
				this.set('disableVehicleHealthMenu',true);
			}
		},
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
								
				params:		{
					goToState:   		  'vehicle.healthReport',
					classNames:         ['button','ember-view','notpressed','list-item'],
					icon:			   'images/info/ico_vehicle.png',
					textBinding: 	 	'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
					templateName: 'arrow',
					action:	  		 'onState',
					target:	  		 'MFT.SettingsController',
					onDown: false,
					disabledBinding: 'MFT.InfoAppsView.list.disableVehicleHealthMenu'
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:  		  'help.helpAssist',
					classNames:         ['button','ember-view','notpressed','list-item'],
					icon:			   'images/info/ico_assist.png',
					textBinding: 	    'MFT.locale.label.view_info_apps_911Assist',
                    classNameBindings:  'MFT.SettingsModel.isSpanish:hidden',
					templateName:       'arrow',
					action:	  	  	 'onState',
					target:	  		 'MFT.SettingsController',
					onDown: 			 false
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
					templateName: 'arrow',
					action:	  	  	 'onState',
					target:	  		 'MFT.SettingsController',
					onDown: false
	})
});
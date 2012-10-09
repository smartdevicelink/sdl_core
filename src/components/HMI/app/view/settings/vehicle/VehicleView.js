/**
 * @name MFT.SettingsVehicleView
 * 
 * @desc Settings Vehicle module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsVehicleView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsVehicleView = Em.ContainerView.create({
	
	elementId:	'settings_vehicle',
	
	classNameBindings: ['MFT.States.settings.vehicle.active:active_state','MFT.helpMode'],
	
	classNames: 'hidden',
	
	childViews: [
		'listVehicleSettings'
	],
	
	listVehicleSettings: MFT.List.extend({
		elementId:		   'settings_vehicle_list',
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'ambientLighting',
					icon:		'images/settings/ico_ambientLighting.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_ambientLighting',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'vehicleHealthReport',
					icon:		'images/settings/ico_vehicleHealthReport.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_VehicleHealthReport',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}							
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'doorKeypadCode',
					icon:		'images/settings/ico_doorKeypadCode.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_DoorKeypadCode',
					arrow:		true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:	'rearViewCamera',
					icon:		'images/settings/ico_rearViewCamera.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_RearViewCamera',
					arrow:		true,
					action:		'onChildState',
					target:	 	'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					//goToState:	 'enableValetMode',
					icon:		    'images/settings/ico_enableValetMode.png',					
					textBinding:     'MFT.locale.label.view_settings_vehicle_EnableValetMode',
					arrow:		   true,
		/*v2*/	  action:	  	  'onPopUp',
					//action:		 'onChildState',
					target:		  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}			
			},
			{
				type:		MFT.Button,
				
				params:		{
					goToState:   'mcs',
					icon:		'images/settings/ico_enableValetMode.png',					
					textBinding: 'MFT.locale.label.view_settings_vehicle_SeatComfort',
					arrow:	   true,
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					hidden:	  FLAGS.MCS_ENABLED ? false : true
				}			
			}
		],
		
		itemsOnPage:	6
	})
});
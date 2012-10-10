/**
 * @name MFT.SettingsVehicleView
 * 
 * @desc Settings Vehicle module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/VehicleView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsVehicleView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'settings_vehicle',
	
	classNameBindings: ['MFT.States.settings.vehicle.active:active_state','MFT.helpMode'],
	
	classNames: 'hidden',
	
	childViews: [
		'popUp',
		'listVehicleSettings'
	],
	
	stateObj: MFT.States.settings.vehicle,
	
	// Enable valet mode popup
	popUp: Em.ContainerView.extend({
		classNameBindings:	   ['MFT.SettingsController.valetPopUp:visible_display:hidden_display'],
		elementId:			   'settings_vehicle_enableValetMode_popUp',
		
		childViews: [
			'blackFilter',
			'content'
		],
		
		blackFilter: Em.View.extend({
			classNames: 'blackFilter'
		}),
		
		content: Em.ContainerView.extend({
			classNames: 'wrapper',
			
			childViews: [
				'textEnabling',
				'textSamePIN',
				'textContinue',
				'yesButton',
				'noButton'
			],
				
			textEnabling: MFT.Label.extend({
				elementId: 'settings_vehicle_enableValetMode_textEnabling',
				contentBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_enablingValetMode'
			}),
			
			textSamePIN: MFT.Label.extend({
				elementId: 'settings_vehicle_enableValetMode_textSamePIN',
				contentBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_samePin'
			}),
			
			textContinue: MFT.Label.extend({
				elementId: 'settings_vehicle_enableValetMode_textContinue',
				contentBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_continue'
			}),
			
			yesButton: MFT.Button.extend({
				classNames: 'button yesButton',
				action: 'onEnterPin',
				target: MFT.SettingsController,
				textBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_yes'
			}),
			
			noButton: MFT.Button.extend({
				classNames: 'button noButton',
				action: 'hideValetPopUp',
				target: MFT.SettingsController,
				textBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_no'
			})
		})
	}),
	
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
					goToState:   'healthReport',
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
					icon:		    'images/settings/ico_enableValetMode.png',					
					textBinding:     'MFT.locale.label.view_settings_vehicle_EnableValetMode',
					arrow:		   true,
					action:	  	  'showValetPopUp',
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
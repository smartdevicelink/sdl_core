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
		
	childViews: [
		'popUp',
		'listVehicleSettings'
	],
		
	// Enable valet mode popup
	popUp: Em.ContainerView.extend({
		classNameBindings:	   ['MFT.SettingsController.valetPopUp:active_state:inactive_state','MFT.helpMode:help-mode'],
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
				textBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_yes',
				disabled: function(){
					 if(MFT.helpMode){ 
					 	return true
					 }else{
					  	return false;
					 }
				}.property('MFT.helpMode')
			}),
			
			noButton: MFT.Button.extend({
				classNames: 'button noButton',
				action: 'hideValetPopUp',
				target: MFT.SettingsController,
				textBinding: 'MFT.locale.label.view_settings_vehicle_enableValetMode_no',
				disabled: function(){
					 if(MFT.helpMode){ 
					 	return true
					 }else{
					  	return false;
					 }
				}.property('MFT.helpMode')
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
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}			
					
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'healthReport',
					icon:		'images/settings/ico_vehicleHealthReport.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_VehicleHealthReport',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}							
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:   'doorKeypadCode',
					icon:		'images/settings/ico_doorKeypadCode.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_DoorKeypadCode',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}
			},
			{
				type:		MFT.Button,

				params:		{
					goToState:	'rearViewCamera',
					icon:		'images/settings/ico_rearViewCamera.png',
					textBinding: 'MFT.locale.label.view_settings_vehicle_RearViewCamera',
					templateName: 'arrow',
					action:		'onChildState',
					target:	 	'MFT.SettingsController',
					helpMode: true
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					icon:		    'images/settings/ico_enableValetMode.png',					
					textBinding:     'MFT.locale.label.view_settings_vehicle_EnableValetMode',
					templateName: 'arrow',
					action:	  	  'showValetPopUp',
					target:		  'MFT.SettingsController',
					helpMode: true
				}			
			},
			{
				type:		MFT.Button,
				
				params:		{
					goToState:   'mcs',
					icon:		'images/settings/ico_enableValetMode.png',					
					textBinding: 'MFT.locale.label.view_settings_vehicle_SeatComfort',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					hidden:	  FLAGS.MCS_ENABLED ? false : true
				}			
			}
		],
		
		itemsOnPage:	6
	})
});
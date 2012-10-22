/**
 * @name MFT.InfoController
 * 
 * @desc Info	Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/InfoController.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.InfoController = Em.Object.create({
	
	activeState:  'info.services',
	
	hiddenLeftMenu: false,

	onState: function(event){
		MFT.States.goToState('info.'+event.goToState);
	},
	
	onChildState: function(event){
		
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	},
	
	appsScroll: null,
	/*
	*
	*	Refactor
	*	Modify state name!
	*
	*
	*/

	appsParams:{
		0:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		1:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		2:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		3:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		4:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		5:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		6:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		7:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			},
		8:	{
				classNames:			'scrollButtons button notpressed',
				icon:				'images/info/ico_vehicle.png',
				textBinding:		'MFT.locale.label.view_info_apps_vehicle_VehicleHealthReport',
				arrow:				true,
				/*action:				'turnOnApp',*/
				click:				function(){
					MFT.SettingsController.onState();
				},
				disabledBinding:	'MFT.helpMode',
				onDown:				false
			}
	}
});
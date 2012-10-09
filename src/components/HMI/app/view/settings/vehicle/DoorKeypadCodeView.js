/**
 * @name MFT.SettingsVehicleDoorKeypadCodeView
 * 
 * @desc Settings.Vehicle.DoorKeypadCode module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsVehicleDoorKeypadCodeView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsVehicleDoorKeypadCodeView = Em.View.create({
	classNameBindings:	   ['MFT.States.settings.vehicle.doorKeypadCode.active:active_state','MFT.helpMode'],
	classNames:			  'hidden',
	elementId:			   'settings_vehicle_DoorKeypadCode',
	template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		'<div class="block-header-title">{{MFT.locale.label.view_settings_vehicle_DoorKeypadCode}}</div>'+
	
		'<div class="inner-wrapper">'+
			'<div class="label-20 disabled factory">{{MFT.locale.label.view_settings_vehicle_DoorKeypadCode_factory}}</div>'+
			'<div class="label-20 disabled digit">{{MFT.locale.label.view_settings_vehicle_DoorKeypadCode_5digit}}</div>'+
			'<div class="label-20 disabled b1">1/2</div>'+
			'<div class="label-20 disabled b2">3/4</div>'+
			'<div class="label-20 disabled b3">5/6</div>'+
			'<div class="label-20 disabled b4">7/8</div>'+
			'<div class="label-20 disabled b5">9/0</div>'+
	
			'<div class="label-20 disabled enter">{{MFT.locale.label.view_settings_vehicle_DoorKeypadCode_enter}}</div>'+
		'</div>'
	)	
});

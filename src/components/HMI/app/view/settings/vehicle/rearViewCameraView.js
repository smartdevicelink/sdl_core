/**
 * @name MFT.SettingsVehicleRearCameraView
 * 
 * @desc Settings.Vehicle.RearCamera module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsVehicleRearCameraView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsVehicleRearCameraView = Em.View.create(MFT.LoadableView,{
	classNameBindings:	   ['MFT.States.settings.vehicle.rearViewCamera.active:active_state','MFT.helpMode'],
	classNames:			  'hidden',
	elementId:			   'settings_vehicle_RearCamera',
	stateObj: MFT.States.settings.vehicle.rearViewCamera,
	template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		'<div class="block-header-title">{{MFT.locale.label.view_settings_vehicle_RearViewCamera}}</div>'+
		'<div class="inner-wrapper">'+
			'<div class="label-20 disabled rcd">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_rcd}}</div>'+
			'<div class="label-20 disabled visualPark">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_visualPark}}</div>'+
			'<div class="label-20 disabled guidelines">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_guidelines}}</div>'+
			'<div class="label-20 disabled preview">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_preview}}:</div>'+
			'<div class="label-20 disabled on1">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_on}}</div>'+
			'<div class="label-20 disabled off1">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_off}}</div>'+
			'<div class="label-20 disabled on2">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_on}}</div>'+
			'<div class="label-20 disabled off2">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_off}}</div>'+
			'<div class="label-20 disabled on3">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_on}}</div>'+
			'<div class="label-20 disabled off3">{{MFT.locale.label.view_settings_vehicle_RearViewCamera_off}}</div>'+
		'</div>'
	)	
});
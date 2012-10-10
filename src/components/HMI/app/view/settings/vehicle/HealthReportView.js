/**
 * @name MFT.SettingsVehicleHealthReportView
 * 
 * @desc Settings.Vehicle.HealthReport module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsVehicleVehicleHealthReport.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsVehicleHealthReportView = Em.View.create(MFT.LoadableView,{
	classNameBindings:	   ['MFT.States.settings.vehicle.healthReport.active:active_state','MFT.helpMode'],
	classNames:			  'hidden',
	elementId:			   'settings_vehicle_HealthReport',
	stateObj: MFT.States.settings.vehicle.healthReport,
	template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="onPreviousState" '+
			'targetBinding="MFT.SettingsController" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		'<div id="vehicleHealthReport_header" class="block-header-title">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport}}</div>'+
			'<div class="inner-wrapper">'+
				'<div class="label-20 disabled ar">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport_ar}}</div>'+
				'<div class="label-20 disabled mi">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport_mi}}</div>'+
				'<div class="label-20 disabled ar_on">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport_on}}</div>'+
				'<div class="label-20 disabled ar_off">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport_off}}</div>'+
				'<div class="label-20 disabled ar_q">?</div>'+
				'<div class="label-20 disabled mi_5000">5000</div>'+
				'<div class="label-20 disabled mi_7500">7500</div>'+
				'<div class="label-20 disabled mi_10000">10000</div>'+
				'<div class="label-20 disabled mi_q">?</div>'+
				'<div class="label-20 disabled mi_rv">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport_rvhrn}}</div>'+
				'<div class="label-20 disabled mi_tc">{{MFT.locale.label.view_settings_vehicle_VehicleHealthReport_to_active}}</div>'+
			'</div>'+
		'</div>'
	)	
});
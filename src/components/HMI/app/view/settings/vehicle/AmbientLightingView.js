/**
 * @name MFT.SettingsVehicleAmbientLightingView
 * 
 * @desc Settings.Vehicle.AmbientLighting module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsVehicleAmbientLightingView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsVehicleAmbientLightingView = Em.View.create({
	classNameBindings:	   ['MFT.States.settings.vehicle.ambientLighting.active:active_state','MFT.helpMode'],
	classNames:			  'hidden',
	elementId:			   ['settings_vehicle_ambientLighting'],
	template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		
		'{{view MFT.Label '+
			'class="block-header-title" '+
			'contentBinding="MFT.locale.label.view_settings_vehicle_ambientLighting" '+
		'}}'
	)
});
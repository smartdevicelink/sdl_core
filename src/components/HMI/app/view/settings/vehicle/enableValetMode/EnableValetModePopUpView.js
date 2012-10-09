/**
 * @name MFT.SettingsVehicleEnableValetModePopUpView
 * 
 * @desc Settings.Vehicle.EnableValetMode module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsVehicleEnableValetModePopUpView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsVehicleEnableValetModePopUpView = Em.View.create({
	classNameBindings:	   [
							 	'MFT.States.settings.vehicle.enableValetMode.active:active_state',
								'MFT.helpMode',
								'MFT.SettingsVehicleEnableValetModeEnteringPINView.popUpHide:hidden_display'
							],
	elementId:			   'settings_vehicle_enableValetMode_popUp',
	template: Ember.Handlebars.compile(
		'<div class="blackFilter"></div>'+
			'<div class="wrapper">'+
				'{{view MFT.Label '+
					'id="settings_vehicle_enableValetMode_textEnabling" '+
					'contentBinding="MFT.locale.label.view_settings_vehicle_enableValetMode_enablingValetMode" '+
				'}}'+
				
				'{{view MFT.Label '+
					'id="settings_vehicle_enableValetMode_textSamePIN" '+
					'contentBinding="MFT.locale.label.view_settings_vehicle_enableValetMode_samePin" '+
				'}}'+
				
				'{{view MFT.Label '+
					'id="settings_vehicle_enableValetMode_textContinue" '+
					'contentBinding="MFT.locale.label.view_settings_vehicle_enableValetMode_continue" '+
				'}}'+
				
				'{{view MFT.Button '+
					'goToState="enterPIN"'+
					'class="button yesButton" '+
					'action="onChildState" '+
					'target="MFT.SettingsController"'+
					'textBinding="MFT.locale.label.view_settings_vehicle_enableValetMode_yes" '+
				'}}'+
									
				'{{view MFT.Button '+
					'class="button noButton" '+
					'action="hidePopUp" '+
					'target="MFT.SettingsController"'+
					'textBinding="MFT.locale.label.view_settings_vehicle_enableValetMode_no" '+
				'}}'+
			'</div>'			
	)
});
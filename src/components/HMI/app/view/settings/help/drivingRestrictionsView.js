/**
 * @name MFT.DriverRestrictionsView
 * 
 * @desc Settings.Help.DriverRestrictions View module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/DriverRestrictionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsHelpDrivingRestrictionsView = Em.View.create(MFT.LoadableView,{
	controller:			  'MFT.SettingsController',
	elementId:			   ['settings_help_driver_restrictions'],
	template: Ember.Handlebars.compile(
				
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
				
		'<div class="block-header-title">{{MFT.locale.label.view_settings_help_DrivingRestrictions_header}}</div>'+
		'<div class="text">{{MFT.locale.label.view_settings_help_DrivingRestrictions_text}}</div>'
	)	
});
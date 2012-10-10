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
	classNameBindings:	   ['MFT.States.settings.help.drivingRestrictions.active:active_state',],
	controller:			  'MFT.SettingsController',
	classNames:			  'hidden',
	elementId:			   ['settings_help_driver_restrictions'],
	stateObj: MFT.States.settings.help.drivingRestrictions,
	template: Ember.Handlebars.compile(
				
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
				
		'<div id="view_settings_help_DrivingRestrictions">'+
		'<div class="block-header-title">{{MFT.locale.label.view_settings_help_DrivingRestrictions_header}}</div>'+
		'<div class="inner-content">'+
				'<div class="text">{{MFT.locale.label.view_settings_help_DrivingRestrictions_text}}</div>'+
			'</div>'+
		'</div>'
	)	
});
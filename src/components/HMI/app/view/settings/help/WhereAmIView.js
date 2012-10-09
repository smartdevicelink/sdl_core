/**
 * @name MFT.SettingsHelpWhereIAmView
 * 
 * @desc Settings.Help.WhereIAmView module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsHelpWhereIAmView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsHelpWhereAmIView = Em.View.create({
	classNameBindings:	   ['MFT.States.settings.help.whereAmI.active:active_state','MFT.helpMode'],
	controller:			  'MFT.SettingsController',
	classNames:			  'hidden',
	elementId:			   ['settings_help_whereAmI'],
	template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		'<div class="block-header-title">{{MFT.locale.label.view_settings_help_WhereAmI_header}}</div>'+
		'<div class="inner-content">'+
			'<div class="label-20 disabled next_street">{{MFT.locale.label.view_settings_help_WhereAmI_next_street}}</div>'+
			'<div class="label-20 disabled villagerd">{{MFT.locale.label.view_settings_help_WhereAmI_villagerd}}</div>'+
			'<div class="label-20 disabled  mi">{{MFT.locale.label.view_settings_help_WhereAmI_mi}}</div>'+
			'<div class="label-20 disabled current_street">{{MFT.locale.label.view_settings_help_WhereAmI_current_street}}</div>'+
			'<div class="label-20 disabled military_st">{{MFT.locale.label.view_settings_help_WhereAmI_military_st}}</div>'+
			'<div class="label-20 disabled dmi">{{MFT.locale.label.view_settings_help_WhereAmI_dearborn_dmi}}</div>'+
			'<div class="label-20 disabled lat">{{MFT.locale.label.view_settings_help_WhereAmI_lat}}</div>'+
			'<div class="label-20 disabled previous_street">{{MFT.locale.label.view_settings_help_WhereAmI_previous_street}}</div>'+
			'<div class="label-20 disabled rd">{{MFT.locale.label.view_settings_help_WhereAmI_west_rd}}</div>'+
			'<div class="label-20 disabled ft">{{MFT.locale.label.view_settings_help_WhereAmI_west_ft}}</div>'+
		'</div>'
	)	
});
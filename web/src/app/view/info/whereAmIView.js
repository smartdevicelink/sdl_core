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
 
MFT.SettingsHelpWhereAmIView = Em.View.create(MFT.LoadableView,{
	elementId:			   ['info_whereAmI'],
	classNameBindings:	   ['MFT.AppController.invertIsNavigationEnabled:no-sd'],
	controller:			  'MFT.SettingsController',
	template: Ember.Handlebars.compile(
		'<div class="inner-block">'+
			'<div class="label-20 disabled next_street">{{MFT.locale.label.view_info_WhereAmI_next_street}}</div>'+
			'<div class="label-20 disabled villagerd block-border">{{MFT.locale.label.view_info_WhereAmI_villagerd}}</div>'+
			'<div class="label-20 disabled  mi">{{MFT.NavigationController.model.distance_to_next_street.value}}<span class="distance_label">{{MFT.NavigationController.model.distance_to_next_street.shortLable}}</div>'+
			'<div class="label-20 disabled current_street">{{MFT.locale.label.view_info_WhereAmI_current_street}}</div>'+
			'<div class="label-20 disabled military_st block-border">{{MFT.locale.label.view_info_WhereAmI_military_st}}</div>'+
			'<div class="label-20 disabled dmi">{{MFT.locale.label.view_info_WhereAmI_dearborn_dmi}}</div>'+
			'<div class="label-20 disabled lat">{{MFT.locale.label.view_info_WhereAmI_lat}}</div>'+
			'<div class="label-20 disabled previous_street">{{MFT.locale.label.view_info_WhereAmI_previous_street}}</div>'+
			'<div class="label-20 disabled rd block-border">{{MFT.locale.label.view_info_WhereAmI_west_rd}}</div>'+
			'<div class="label-20 disabled ft">{{MFT.NavigationController.model.distance_to_prev_street.value}} {{MFT.NavigationController.model.distance_to_prev_street.shortLable}} </div>'+
		'</div>'
	)	
});
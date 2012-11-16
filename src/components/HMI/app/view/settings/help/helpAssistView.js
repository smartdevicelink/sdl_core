/**
 * @name MFT.SettingsHelp911AssistView
 * 
 * @desc Settings.Help.911Assist module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsHelp911AssistView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsHelp911AssistView = Em.View.create(MFT.LoadableView,{
	
	classNameBindings:	   ['MFT.localization'],
	
	controller:			  'MFT.SettingsController',
	
	activeDisabled: function(){
	if (!MFT.helpMode) {
		return true;
	}
				 
	}.property('MFT.helpMode'),
		
	elementId:			   ['settings_help_911Assist'],
	
	template: Ember.Handlebars.compile(
		
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="onPreviousState" '+
			'targetBinding="MFT.SettingsController" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		
		'<div id="viewSettingsDisplay_911-assist" class="block-header-title">{{MFT.locale.label.view_info_apps_911Assist_header}}</div>'+
		'<div class="inner-content">'+
			'<div class="ico-911"></div>'+
			'<div id="viewSettingsDisplay_vri" class="label-20 disabled btn btn-98 vri"><span>{{MFT.locale.label.view_info_apps_911Assist_vri}}</span></div>'+
			'<div id="viewSettingsDisplay_assist-911" class="label-20 disabled assist-911">{{MFT.locale.label.view_info_apps_911Assist_911_connected}}.</div>'+
			'<div id="viewSettingsDisplay_assist-a911" class="text-block disabled text-block-500 a911">'+
				'{{MFT.locale.label.view_info_apps_911Assist_911}}'+
				'<div id="viewSettingsDisplay_a911-on" class="btn disabled btn-28 disabled btn-left-c a911-on active" {{bindAttr class="view.activeDisabled:active"}}>{{MFT.locale.label.view_info_apps_911Assist_on}}</div>'+
				'<div id="viewSettingsDisplay_a911-off" class="btn disabled btn-28 disabled btn-right-c a911-off">{{MFT.locale.label.view_info_apps_911Assist_off}}</div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_assist-a911r" class="text-block text-block-500 disabled a911r">'+
				'{{MFT.locale.label.view_info_apps_911Assist_reminder}}'+
				'<div id="viewSettingsDisplay_assist-a911-on" class="btn disabled btn-28 disabled btn-left-c a911-on active" {{bindAttr class="view.activeDisabled:active"}}>{{MFT.locale.label.view_info_apps_911Assist_on}}</div>'+
				'<div id="viewSettingsDisplay_assist-a911-off" class="btn disabled btn-28 disabled btn-right-c a911-off">{{MFT.locale.label.view_info_apps_911Assist_off}}</div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_icofesd" class="label-20 disabled icofesd">{{MFT.locale.label.view_info_apps_911Assist_incase}}:</div>'+
			'<div id="viewSettingsDisplay_sarah" class="label-20 btn btn-48 disabled sarah">'+
				'{{MFT.locale.label.view_info_apps_911Assist_sarah}}'+
				'<div class="ico"></div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_john" class="label-20 btn btn-48 disabled john">'+
				'{{MFT.locale.label.view_info_apps_911Assist_john}}'+
				'<div class="ico"></div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_edit" class="label-20 btn btn-48 disabled edit">'+
				'{{MFT.locale.label.view_info_apps_911Assist_edit}}'+
				'<div class="ico"></div>'+
			'</div>'+
		'</div>'
	)	
});
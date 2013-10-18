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
	
	classNameBindings:	   ['FLAGS.locale'],
	
	controller:			  'MFT.SettingsController',
		
	elementId:			   'settings_help_911Assist',
    classNames: 'settings-list-block',
	
	template: Ember.Handlebars.compile(
		
		'{{view MFT.Button '+
			'elementId="settings_help_911Assist_backButton" '+
			'class="button backButton" '+
			'action="goToPreviousState" '+
			'targetBinding="MFT.States" '+
			'icon="images/media/ico_back.png" '+
		'}}'+
		
		'<div id="viewSettingsDisplay_911-assist" class="block-header-title"><span>{{MFT.locale.label.view_info_apps_911Assist_header}}</span></div>'+
		'<div class="inner-content">'+
			'<div class="ico-911"></div>'+
			'<div id="viewSettingsDisplay_vri" class="label-20 disabled btn btn-98 vri"><span>{{MFT.locale.label.view_info_apps_911Assist_vri}}</span></div>'+
			'<div id="viewSettingsDisplay_assist-911" class="label-20 disabled assist-911"><span>{{MFT.locale.label.view_info_apps_911Assist_911_connected}}</div>'+
			'<div id="viewSettingsDisplay_assist-a911" class="text-block disabled text-block-500 a911">'+
				'<span>{{MFT.locale.label.view_info_apps_911Assist_911}}</span>'+
				'<div id="viewSettingsDisplay_a911-on" class="btn disabled btn-28 disabled a911-on" ><span>{{MFT.locale.label.view_info_apps_911Assist_on}}</span></div>'+
				'<div id="viewSettingsDisplay_a911-off" class="btn disabled btn-28 disabled a911-off active"><span>{{MFT.locale.label.view_info_apps_911Assist_off}}</span></div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_assist-a911r" class="text-block text-block-500 disabled a911r">'+
				'<span>{{MFT.locale.label.view_info_apps_911Assist_reminder}}</span>'+
				'<div id="viewSettingsDisplay_assist-a911-on" class="btn disabled btn-28 disabled a911-on"><span>{{MFT.locale.label.view_info_apps_911Assist_on}}</span></div>'+
				'<div id="viewSettingsDisplay_assist-a911-off" class="btn disabled btn-28 disabled a911-off active"><span>{{MFT.locale.label.view_info_apps_911Assist_off}}</span></div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_icofesd" class="label-20 disabled icofesd"><span>{{MFT.locale.label.view_info_apps_911Assist_incase}}:</div>'+
			'<div id="viewSettingsDisplay_sarah" class="label-20 btn btn-48 disabled sarah">'+
				'<span>{{MFT.locale.label.view_info_apps_911Assist_sarah}}</span>'+
				'<div class="ico"></div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_john" class="label-20 btn btn-48 disabled john">'+
				'<span>{{MFT.locale.label.view_info_apps_911Assist_john}}</span>'+
				'<div class="ico"></div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_edit" class="label-20 btn btn-48 disabled edit">'+
				'<span>{{MFT.locale.label.view_info_apps_911Assist_edit}}</span>'+
				'<div class="ico"></div>'+
			'</div>'+
		'</div>'
	)	
});
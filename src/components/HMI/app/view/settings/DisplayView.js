/**
 * @name MFT.SettingsDisplayView
 * 
 * @desc Settings Display module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsDisplayView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsDisplayView = Em.View.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
	
	elementId:		'settings_display',
	
	classNames:	   'hidden',
	
	stateObj: MFT.States.settings.display,
	
	activeDisabled: function(){
		if (!MFT.helpMode) {
			return true;
		}
				 
	}.property('MFT.helpMode'),
	
	template: Ember.Handlebars.compile(
		'<div class="inner-wrapper">'+
			'<div id="viewSettingsDisplay_brightness" class="label-20 disabled brightness">{{MFT.locale.label.view_settings_display_brightness}}</div>'+
			'<div class="label-20 disabled auto-dim">'+
				'{{MFT.locale.label.view_settings_display_auto_dim}}'+
				'<div id="viewSettingsDisplay_on" class="btn disabled btn-28 disabled btn-left-c auto-dim-on active" {{bindAttr class="view.activeDisabled:active"}} >{{MFT.locale.label.view_settings_display_on}}</div>'+
				'<div id="viewSettingsDisplay_off" class="btn disabled btn-28 disabled btn-right-c auto-dim-off">{{MFT.locale.label.view_settings_display_off}}</div>'+
			'</div>'+
			
			'<div class="label-20 disabled mode">'+
				'{{MFT.locale.label.view_settings_display_mode}}'+
				'<div id="viewSettingsDisplay_auto" class="btn disabled btn-28 disabled btn-left-c auto active" {{bindAttr class="view.activeDisabled:active"}}">{{MFT.locale.label.view_settings_display_auto}}</div>'+
				'<div id="viewSettingsDisplay_day" class="btn disabled btn-28 disabled btn-center day">{{MFT.locale.label.view_settings_display_day}}</div>'+
				'<div id="viewSettingsDisplay_night" class="btn disabled btn-28 disabled btn-right-c night">{{MFT.locale.label.view_settings_display_night}}</div>'+
			'</div>'+
			'<div id="viewSettingsDisplay_editWallpaper" class="label-20 disabled edit-wallpaper">{{MFT.locale.label.view_settings_display_edit_wallpaper}}</div>'+
			'<div id="viewSettingsDisplay_autoDimManualOffset" class="label-20 disabled auto-dim-manual-offset">{{MFT.locale.label.view_settings_display_auto_offset}}</div>'+
		'</div>'
	)	

});

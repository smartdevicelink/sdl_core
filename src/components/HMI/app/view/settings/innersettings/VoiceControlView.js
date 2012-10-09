/**
 * @name MFT.SettingsSettingsVoiceControlView
 * 
 * @desc Settings.Sound.SettingsSettingsVoiceControl module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsVoiceControlView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSettingsVoiceControlView = Em.View.create({
	classNameBindings:	   ['MFT.States.settings.settings.voiceControl.active:active_state','MFT.helpMode'],
	controller:			  'MFT.SettingsController',
	classNames:			  'hidden',
	elementId:			   'settings_settings_VoiceControl',
    template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'target="MFT.States"'+
			'icon="images/media/ico_back.png" '+
		'}}'+
		'<div class="block-header-title">{{MFT.locale.label.view_settings_settings_VoiceControl_header}}</div>'+
		'<div class="inner-content">'+
			'<div class="label-20 disabled im">{{MFT.locale.label.view_settings_settings_VoiceControl_im}}</div>'+
			'<div class="label-20 disabled novice">{{MFT.locale.label.view_settings_settings_VoiceControl_novice}}</div>'+
			'<div class="label-20 disabled advance">{{MFT.locale.label.view_settings_settings_VoiceControl_advance}}</div>'+
			'<div class="label-20 disabled cp">{{MFT.locale.label.view_settings_settings_VoiceControl_cp}}</div>'+
			'<div class="label-20 disabled mcl">{{MFT.locale.label.view_settings_settings_VoiceControl_mcl}}</div>'+
			'<div class="label-20 disabled pcl">{{MFT.locale.label.view_settings_settings_VoiceControl_pcl}}</div>'+
			'<div class="label-20 disabled vcv">{{MFT.locale.label.view_settings_settings_VoiceControl_vcv}}</div>'+

			'<div class="label-20 disabled on1">{{MFT.locale.label.view_settings_settings_VoiceControl_on}}</div>'+
			'<div class="label-20 disabled on2">{{MFT.locale.label.view_settings_settings_VoiceControl_on}}</div>'+
			'<div class="label-20 disabled on3">{{MFT.locale.label.view_settings_settings_VoiceControl_on}}</div>'+

			'<div class="label-20 disabled off1">{{MFT.locale.label.view_settings_settings_VoiceControl_off}}</div>'+
			'<div class="label-20 disabled off2">{{MFT.locale.label.view_settings_settings_VoiceControl_off}}</div>'+
			'<div class="label-20 disabled off3">{{MFT.locale.label.view_settings_settings_VoiceControl_off}}</div>'+
		'</div>'
	  )	
});

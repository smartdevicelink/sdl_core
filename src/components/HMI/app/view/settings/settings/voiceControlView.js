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
 
MFT.SettingsSettingsVoiceControlView = Em.View.create(MFT.LoadableView,{
	controller:			  'MFT.SettingsController',
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
			'<div class="label-20 disabled novice toggle0">{{MFT.locale.label.view_settings_settings_VoiceControl_novice}}</div>'+
			'<div class="label-20 disabled advance toggle1">{{MFT.locale.label.view_settings_settings_VoiceControl_advance}}</div>'+
			'<div class="label-20 disabled cp">{{MFT.locale.label.view_settings_settings_VoiceControl_confirmationPrompts}}</div>'+
			'<div class="label-20 disabled mcl">{{MFT.locale.label.view_settings_settings_VoiceControl_mediaCandidateLists}}</div>'+
			'<div class="label-20 disabled pcl">{{MFT.locale.label.view_settings_settings_VoiceControl_phoneCandidateLists}}</div>'+
			'<div class="label-20 disabled vcv">{{MFT.locale.label.view_settings_settings_VoiceControl_voiceControlVolume}}</div>'+

			'<div class="label-20 disabled on1 toggle0">{{MFT.locale.label.view_settings_settings_VoiceControl_on}}</div>'+
			'<div class="label-20 disabled on2 toggle0">{{MFT.locale.label.view_settings_settings_VoiceControl_on}}</div>'+
			'<div class="label-20 disabled on3 toggle0">{{MFT.locale.label.view_settings_settings_VoiceControl_on}}</div>'+

			'<div class="label-20 disabled off1 toggle1">{{MFT.locale.label.view_settings_settings_VoiceControl_off}}</div>'+
			'<div class="label-20 disabled off2 toggle1">{{MFT.locale.label.view_settings_settings_VoiceControl_off}}</div>'+
			'<div class="label-20 disabled off3 toggle1">{{MFT.locale.label.view_settings_settings_VoiceControl_off}}</div>'+
		'</div>'
	  )	
});

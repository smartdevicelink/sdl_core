/**
 * @name MFT.SettingsSoundSetBalanceAndFadeView
 * 
 * @desc Settings.Sound.SetBalanceAndFade module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSoundSetBalanceAndFadeView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsSoundSetBalanceAndFadeView = Em.View.create(MFT.LoadableView,{
	classNameBindings:	   ['MFT.helpMode','MFT.localization'],
	elementId:			   'settings_sound_soundBalanceFade',
		
	template: Ember.Handlebars.compile(
		'{{view MFT.Button '+
			'class="button backButton" '+
			'action="back" '+
			'target="MFT.States"'+
			'icon="images/media/ico_back.png" '+
		'}}'+
		'<div class="block-header-title">{{MFT.locale.label.view_settings_Sound_BalanceFader_header}}</div>'+
		'<div class="inner-wrapper">'+
				'<div class="label-20 disabled fade">{{MFT.locale.label.view_settings_Sound_BalanceFader_fade}}</div>'+
				'<div class="label-20 disabled balance">{{MFT.locale.label.view_settings_Sound_BalanceFader_balance}}</div>'+
				'<div class="label-20 disabled z1">0</div>'+
				'<div class="label-20 disabled z2">0</div>'+
				'<div class="label-20 disabled f">{{MFT.locale.label.view_settings_Sound_BalanceFader_f}}</div>'+
				'<div class="label-20 disabled b">{{MFT.locale.label.view_settings_Sound_BalanceFader_b}}</div>'+
				'<div class="label-20 disabled l">{{MFT.locale.label.view_settings_Sound_BalanceFader_l}}</div>'+
				'<div class="label-20 disabled r">{{MFT.locale.label.view_settings_Sound_BalanceFader_r}}</div>'+
			'</div>'+
		'</div>'
	)	
});

/**
 * @name MFT.SettingsSoundView
 * 
 * @desc Settings Sound module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSoundView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSoundView = Em.ContainerView.create({
	
	classNameBindings: 	['MFT.States.settings.sound.active:active_state','MFT.helpMode'],
	
	classNames:		   ['hidden'],
	
	elementId:			'settings_sound_view',
	
	childViews:		   [
							  'list'
						  ],

	list:	MFT.List.extend({
			
			elementId:	  'settings_sound_list',
			
			itemsOnPage:	5,
			
			disableScrollbarBinding: 'MFT.helpMode',
			
			items:[
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_sound_bass',
					icon:	    	   'images/settings/ico_plus-minus-pannel_disabled.png',
					disabled:    	   true
				}								
			},
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_sound_midrage',
					icon:	    	   'images/settings/ico_plus-minus-pannel_disabled.png',
					disabled:    	   true
				}								
			},
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_sound_treble',
					icon:	    	   'images/settings/ico_plus-minus-pannel_disabled.png',
					disabled:    	   true
				}								
			},
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_sound_setBalance',
					arrow:	   true,
					goToState:   'setSoundBalanceFader',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabledBinding: 'MFT.helpMode'
				}
			},
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_sound_playTHX',
					arrow:	   true,
					goToState:   'setSoundBalanceFader',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabled: 	true
				}
			},
			{				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_sound_dsp',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_sound_stereoSurround',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}					
			},
			{				
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_sound_speed',
					tButtonValue:		 0,
					tButtonRange: 		 8,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_sound_offtoSeven',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}					
			},
		]
	})
});
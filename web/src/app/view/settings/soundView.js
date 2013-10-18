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

MFT.SettingsSoundView = Em.ContainerView.create(MFT.LoadableView,{
			
	elementId:			'settings_sound_view',

    childViews: [
        'list'
    ],
						  
	list:	MFT.List.extend({
			
			elementId:	  'settings_sound_list',
			
			itemsOnPage:	5,
			
			items:[
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_sound_bass',
					icon:	    	   'images/settings/ico_plus-minus-pannel_disabled.png',
					templateName: 'icon',
					disabled:    	   true
				}								
			},
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_sound_midrange',
					icon:	    	   'images/settings/ico_plus-minus-pannel_disabled.png',
					templateName: 'icon',
					disabled:    	   true
				}								
			},
			{
								
				type:		 MFT.Label,
				params:		{
					contentBinding:	 'MFT.locale.label.view_settings_sound_treble',
					icon:	    	   'images/settings/ico_plus-minus-pannel_disabled.png',
					templateName: 'icon',
					disabled:    	   true
				}								
			},
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_sound_setBalance',
					templateName: 'arrow',
					goToState:   'setSoundBalanceFader',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true
				}
			},
            {
                type:		MFT.LabelPlusToggleButton,
                params:		{
                    classNameBindings: 'MFT.AppController.activeVehicleModel.isClimateDisabled::hidden',
                    labelContentBinding:  'MFT.locale.label.view_settings_sound_occupancyMode',
                    tButtonValue:		 2,
                    tButtonRange: 		 3,
                    tButtonLabelsBinding: 'MFT.locale.label.view_settings_sound_allSeatsDriverRear',
                    labelDisabled: 		 true,
                    tButtonDisabled:	 true
                }
            },
			{
				type:		MFT.Button,
				params:		{
					className:   'button',
					textBinding: 'MFT.locale.label.view_settings_sound_playTHX',
					templateName: 'arrow',
					goToState:   'setSoundBalanceFader',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					disabled: 	true
				}
			},
			{				
				type:		MFT.LabelPlusToggleButton,
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
				type:		MFT.LabelPlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_sound_speed',
					tButtonValue:		 0,
					tButtonRange: 		 8,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_sound_offtoSeven',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}					
			}
		]
	})
});

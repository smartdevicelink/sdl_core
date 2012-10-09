/**
 * @name MFT.SettingsClockView
 * 
 * @desc Settings Clock module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsClockView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsClockView = Em.ContainerView.create({
	classNameBindings:	['MFT.States.settings.clock.active:active_state','MFT.helpMode'],
	classNames:		['hidden'],
	elementId:		 'settings_clock',
	childViews:		[
					   		'clockPanel',
							'listClockSettings'
					   ],
						  
	clockPanel:		   Em.ContainerView.extend({
						  classNames:			'clockPanel',
						  childViews: 		 [
											  	'leftPlusButton',
											  	'leftMinusButton',
											  	'timeNum',
											  	'rightPlusButton',
											  	'rightMinusButton',
											  	'am',
											  	'pm'
											  ],
						  leftPlusButton:	 MFT.Button.extend({
							 classNames:		['leftPlusButton', 'button'],
							 action:			'',
							 target:			'MFT.SettingsController',
							 timer:			 200,
							 icon:			  'images/settings/ico_clock_plus_comtrol.png',
							 /*disabledBinding: 'MFT.helpMode',*/
							 disabled:		  true						 
							 }),
						  
						  leftMinusButton:	MFT.Button.extend({
							 classNames:		['leftMinusButton', 'button'],
							 action:			'',
							 target:			'MFT.SettingsController',
							 timer:			 200,
							 icon:			  'images/settings/ico_minus_control.png',
							 disabled:		  true								 
							 }),
							 
						  timeNum:			Em.ContainerView.extend({
												classNames:	['timeNum'],	
												childViews: 	['time', 'timeZone'],
													time:		   MFT.Label.extend({
																	classNames: ['time'],
																	contentBinding: Em.Binding.oneWay('MFT.locale.label.view_settings_timeNum'),
																	disabled:    true
																}),
												timeZone:		MFT.Label.extend({
																	classNames: ['timeZone'],
																	contentBinding: Em.Binding.oneWay('MFT.locale.label.view_settings_timeZone'),
																	disabled:    true
																})
					
											  }),
											 
						  rightPlusButton:	MFT.Button.extend({
							 classNames:		['rightPlusButton', 'button'],
							 action:			'',
							 target:			'MFT.SettingsController',
							 timer:			 200,
							 icon:			  'images/settings/ico_clock_plus_comtrol.png',
							 disabled:		  true
							 }),					  
											  
						  rightMinusButton:   MFT.Button.extend({
							 classNames:		['rightMinusButton', 'button'],
							 action:			'',
							 target:			'MFT.SettingsController',
							 timer:			 200,
							 icon:			  'images/settings/ico_minus_control.png',
							 disabled:		  true								 
							 }),
						  
						  
						  am:				 MFT.Button.extend({
							 classNames:		['am', 'button'],
							 action:			'',
							 target:			'MFT.SettingsController',
							 timer:			 200,
							 textBinding:		Em.Binding.oneWay('MFT.locale.label.view_settings_am'),
							 disabled:		  true
															 
											  }),
						 
						  pm:				 MFT.Button.extend({
							 classNames:		 ['pm', 'button'],
							 action:			 '',
							 target:			 'MFT.SettingsController',
							 timer:			  200,
							 textBinding:		Em.Binding.oneWay('MFT.locale.label.view_settings_pm'),
							 disabled:		  true								 
											  }),
						  }),
	
	listClockSettings: MFT.List.extend({
		elementId:		   'settings_clock_list',
		
		classNameBindings: ['MFT.helpMode: hidden'],
		
		disableScrollbarBinding:	Em.Binding.oneWay('MFT.helpMode'),
		
		/** Items */
		items: [
			{
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_clokDisplay',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_timeDate',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer doubletoggle']
				}									
			},
			{
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_outsideAirTemperature',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_OnOFF',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer doubletoggle']
				}									
			},
			{
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_clockFormat',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_hours',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer doubletoggle']
				}									
			},
			{
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_GPSTimeSynchronization',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_OnOFF',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer doubletoggle']
				}									
			},
			{
				type:		MFT.LablePlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_autoTimeZoneUpdate',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_OnOFF',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer doubletoggle']
				}									
			}
		],
		
		itemsOnPage:	4
	})	

		
});
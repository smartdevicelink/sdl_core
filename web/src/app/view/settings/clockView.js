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

MFT.SettingsClockView = Em.ContainerView.create(MFT.LoadableView,{
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
												childViews: 	['time'],
												time:		   MFT.Label.extend({
																classNames: ['time'],
																contentBinding: 'MFT.SettingsController.time'
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
											  })
						  }),
	
	listClockSettings: MFT.List.extend({
		elementId:		   'settings_clock_list',
		
		/** Items */
		items: [
			{
				type:		MFT.LabelPlusToggleButton,
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
				type:		MFT.LabelPlusToggleButton,
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
				type:		MFT.LabelPlusToggleButton,
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
				type:		MFT.LabelPlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_GPSTimeSynchronization',
					tButtonValue:		 1,
					tButtonRange:		 1,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_reset',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer resetButton'],
					hiddenBinding: 		'MFT.AppController.invertIsNavigationEnabled'
				}									
			},
			{
				type:		MFT.LabelPlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_clock_autoTimeZoneUpdate',
					tButtonValue:		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_clock_OnOFF',
					labelDisabled: 		true,
					tButtonDisabled:	  true,
					classContainer:	   ['toogleButtonContainer doubletoggle'],
					hiddenBinding: 		'MFT.AppController.invertIsNavigationEnabled'
				}									
			}
		],
		
		itemsOnPage:	4
	})
		
});
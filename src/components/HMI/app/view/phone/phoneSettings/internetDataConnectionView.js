/**
 * @name MFT.PhoneSettingsInternetDataConnectionView
 * 
 * @desc Phone Settings Internet Data Connection module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/options/InternetDataConnectionView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.PhoneSettingsInternetDataConnectionView = Em.ContainerView.create( MFT.LoadableView, {
	stateObj:	MFT.States.phone.phoneSettings.internetDataConnection,
	
	classNameBindings:	['MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'phone_settings_internetDataConnection_view',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'content'
						  ],

	backButton: MFT.Button.extend({
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_phone_phoneSettings_internetDataConnection_title'
	}),				  
	
	content: Em.ContainerView.extend({
		classNames:		   ['content'],
		childViews:		   [
								  'phone',
								  'connection',
								  'connectionHelp',
								  'DUNSettings',
								  'queryOnConnect',
								  'queryOnConnectHelp',
								  'dataUsage'
						  	  ],
		phone: MFT.Label.extend({
			elementId:		  ['phone_settings_phoneRinger_phone_label'],
			icon:			   'images/phone/ico_phoneMelody.png',
			contentBinding:	 'MFT.locale.label.view_phone_phoneSettings_internetDataConnection_PaulPhone',
			disabled:		   true
				
		}),
		
		connection: MFT.LablePlusToggleButton.extend({
			elementId:		    'phone_settings_phoneRinger_connection_LablePlusToggleButton',
			labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_connectionProfile',
			tButtonValue:		 2,
			tButtonRange: 		 3,
			tButtonLabelsBinding: 'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_connectionToggle',
			labelDisabled: 		true,
			tButtonDisabled:	  true
		}),
		
		connectionHelp: MFT.Button.extend({
			elementId:		  'phone_settings_phoneRinger_connectionHelp_button',
			classNames:  		 ['button','helpButton'],
			icon:			   'images/phone/ico_questionMark.png',
			disabled:		   true
		}),
		
		DUNSettings: MFT.Button.extend({
			elementId:		 'phone_settings_phoneRinger_DUNSettings_button',
			classNames:  		'button',
			textBinding:       'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_DUNSettings',
			arrow: 			 true,
			disabled:   		  true
			
		}),
		
		queryOnConnect: MFT.LablePlusToggleButton.extend({
			elementId:		  	['phone_settings_phoneRinger_OnConnect_Lable'],
			labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_queryOnConnect',
			tButtonValue:		 null,
			tButtonRange: 		 3,
			tButtonLabelsBinding: 'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_queryToggle',
			labelDisabled: 		true,
			tButtonDisabled:	  true
		}),
		
		queryOnConnectHelp: MFT.Button.extend({
			elementId:		  'phone_settings_phoneRinger_queryOnConnectHelp_button',
			classNames:  		 ['button','helpButton'],
			icon:			   'images/phone/ico_questionMark.png',
			disabled:		   true
		}),
		
		dataUsage: Em.ContainerView.extend({
			classNames:	'wraper',
			childViews: [ 
				MFT.Label.extend({
					elementId:		  ['phone_settings_phoneRinger_dataUsage_label'],
					contentBinding:	 'MFT.locale.label.view_phone_phoneSettings_internetDataConncetion_dataUsage',
					disabled:		   true	
				})
			]
		})
					
	})
	
});
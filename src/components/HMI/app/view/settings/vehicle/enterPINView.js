/**
 * @name MFT.SettingsHelpVoiceCommandListEnteringPINView
 * 
 * @desc Settings Help Voice Command List Entering Pin module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/EnteringPinView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsVehicleEnableValetModeEnteringPINView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.helpMode','MFT.localization'],
	classNames:		   ['hidden'],
	elementId:			'settings_vehicle_enableValetMode_enteringPin',
		
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'enterPadLable',
							  'enterPad',
							  'backspaceButton',
							  'enterButton'
						  ],
								  	
	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode'
	}),
	
	enterPadLable: MFT.Label.extend({
		elementId:		  'settings_vehicle_enableValetMode_enteringPin_enterPadLabel',
		contentBinding:	 'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_enter4Digit',
		disabled: 		   true
	}),
	
	enterPad:  Em.ContainerView.extend({
		elementId:		   'settings_vehicle_enableValetMode_enteringPin_enterPad',
		childViews: [
			'PIN',
			'key7',
			'key8',
			'key9',
			'key4',
			'key5',
			'key6',
			'key1',
			'key2',
			'key3',
			'key0',
		],
		
		/** Number area */
		PIN: MFT.Label.extend({
			elementId:		'settings_vehicle_enableValetMode_enteringPin_PIN',
			disabled: 		 true
		}),
		
		key7: MFT.Button.extend({
			classNames:	   ['digitButton','button'],
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit7',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_7',
			disabled: 		 true
		}), 
		
		key8: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit8',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_8',
			disabled: 		 true
		}), 		
		key9: MFT.Button.extend({
			classNames:	   ['digitButton','button'],
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit9',		
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_9',
			disabled: 		 true
		}), 
		key4: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit4',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_4',
			disabled: 		 true
		}), 
		
		key5: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit5',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_5',
			disabled: 		 true
		}), 
		
		key6: MFT.Button.extend({
			classNames:	   ['digitButton','button'],
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit6',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_6',
			disabled: 		 true
		}), 
		key1: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit1',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_1',
			disabled: 		 true
		}), 
		
		key2: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit2',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_2',
			disabled: 		 true
		}), 
		
		key3: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit3',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_3',
			disabled: 		 true
		}),  
		key0: MFT.Button.extend({
			classNames:	   ['digitButton','button'],	
			elementId:		'settings_vehicle_enableValetMode_enteringPin_digit0',	
			textBinding:	  'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_0',
			disabled: 		 true
		}), 
	}),
	
	backspaceButton: MFT.Button.extend({
		elementId:		   'settings_vehicle_enableValetMode_enteringPin_backspaceButton',
		classNames:		  ['backspace','button'],		
		icon:				'images/settings/ico_backspace.png',
		disabled: 		    true	
	}),
	
	enterButton: MFT.Button.extend({
		elementId:		   'settings_vehicle_enableValetMode_enteringPin_enterButton',
		classNames:		  ['button'],		
		textBinding:	  	 'MFT.locale.label.view_settings_vehicle_EnableValetMode_enterPIN_enter',
		disabled: 		    true
	})			 
});
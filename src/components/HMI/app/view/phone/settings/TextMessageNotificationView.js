/**
 * @name MFT.PhoneSettingsTextMessageNotificationView
 * 
 * @desc Phone Settings Text Message NotificationView module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/options/ManagePhoneBookView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.PhoneSettingsTextMessageNotificationView = Em.ContainerView.create({
	classNameBindings:	['MFT.States.phone.phoneSettings.textMessage.active:active_state','MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'phone_settings_textMessageNotification_view',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		classNames:		   ['backButton','button'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_phone_phoneSettings_textMessageNotification_title'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'phone_settings_textMessageNotification_list',
			
			itemsOnPage:	5,
			
			disableScrollbar: true,
			
			items:[
				
				{								
					type:		MFT.Button,
					
					params:		{
						classNames: ['active','ffw-button','notpressed','list-item'],
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_textMessageNotification_alert1',
						disabled: true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						classNames:  [ 'list-item' , 'soundButton' , 'button'],
						icon:		'images/phone/ico_sound_active.png',
						disabled:    true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_textMessageNotification_alert2',
						disabled: true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						classNames:  [ 'list-item' , 'soundButton' , 'button'],
						icon:		'images/phone/ico_sound_active.png',
						disabled:    true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_textMessageNotification_alert3',
						disabled: true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						classNames:  [ 'list-item' , 'soundButton' , 'button'],
						icon:		'images/phone/ico_sound_active.png',
						disabled:    true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_textMessageNotification_textToSpeech',
						disabled: true
	
					}			
						
				},
				
				{								
					type:		MFT.Button,
					
					params:		{
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_textMessageNotification_silent',
						disabled: true
	
					}			
						
				}
				
			]
	})
});
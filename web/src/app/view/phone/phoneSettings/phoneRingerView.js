/**
 * @name MFT.PhoneSettingsPhoneRingerView
 * 
 * @desc Phone Settings Phone Ringer module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/options/PhoneRingerView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.PhoneSettingsPhoneRingerView = Em.ContainerView.create( MFT.LoadableView, {
		
	elementId:			'phone_settings_phoneRinger',
	
	classNames:			['phone_view'],
	
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'phone_settings_phoneRinger_backButton',
		classNames:		   ['backButton','button'],		
		action:			   'onBackState',
		target:			   'MFT.PhoneController',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'phone_phoneSettings_phoneRinger_title',
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_phone_phoneSettings_phoneRinger_title'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'phone_settings_phoneRinger_list',
			
			itemsOnPage:	5,
			
			columnsNumber:  2,
						
			items:[
				
				{								
					type:		MFT.Button,
					
					params:		{
						classNames: ['active','ffw-button','notpressed','list-item'],
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_phoneRinger_ringer1',
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
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_phoneRinger_ringer2',
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
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_phoneRinger_ringer3',
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
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_phoneRinger_beep',
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
						classNames: ['ffw-button','notpressed','list-item'],
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_phoneRinger_textToSpeech',
						disabled: true
	
					}			
						
				}
				
			]
	})
});
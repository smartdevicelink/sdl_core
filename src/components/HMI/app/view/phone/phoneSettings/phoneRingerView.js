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
	
	stateObj:	MFT.States.phone.phoneSettings.phoneRinger,
	
	classNameBindings:	['MFT.helpMode'],
	classNames:		   ['hidden'],
	elementId:			'phone_settings_phoneRinger_view',
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
		contentBinding:	  'MFT.locale.label.view_phone_phoneSettings_phoneRinger_title'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'phone_settings_phoneRinger_list',
			
			itemsOnPage:	5,
			
			columnsNumber:  2,
			
			disableScrollbarBinding: 'MFT.helpMode',
			
			items:[
				
				{								
					type:		MFT.Button,
					
					params:		{
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
						classNames: ['active','ffw-button','notpressed','list-item'],
						textBinding: 'MFT.locale.label.view_phone_phoneSettings_phoneRinger_connectedPhone',
						disabled: true
	
					}			
						
				}
				
			]
	})
});
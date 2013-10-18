/**
 * @name MFT.PhoneView
 * 
 * @desc Phone module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/PhoneView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.PhoneView = Em.ContainerView.create(MFT.LoadableView,{
		
	elementId:	'phone',
 	
 	classNames:	['phone_view'],
 	
	/** Initial phone components */		
	childViews: [
		'info',
		'menu',
		'endCallMessage'
	],
	
	/** Info block */
	info: Em.View.extend({
		//classNameBindings: ['MFT.helpMode:hide'],
		
		elementId:	'phone_info',
		
		template: Ember.Handlebars.compile(
			'{{#if MFT.PhoneController.model.readyForCall}}<div id="phone_info_name"><span>{{MFT.locale.label.view_wigetPhone_phone}}</div></span>{{/if}}'+
			'{{#if MFT.PhoneController.model.onCall}}'+
				'<div id="phone_info_name"><span>Sarah</span> <span id="phone_info_number">{{MFT.PhoneController.model.phoneStatusNumber}}</span><p><span>3:47</span></p></div>'+
			'{{/if}}'
		)
	}),
	
	/** Left menu */
	menu: Em.ContainerView.extend({
		elementId:	'phone_menu',
		
		classNameBindings: [
            'MFT.States.phone.phoneSettings.bluetoothDevices.active:hidden',
            'MFT.States.phone.phoneSettings.phoneRinger.active:hidden',
            'MFT.States.phone.phoneSettings.textMessage.active:hidden',
            'MFT.States.phone.phoneSettings.internetDataConnection.active:hidden',
            'MFT.States.phone.phoneSettings.managePhoneBook.active:hidden'
        ],
		
		childViews: [
			'border',
			'items'
		],
		
		border: Em.View.extend({
			classNames:	'ls_border',
			
			template: Ember.Handlebars.compile('<img class="bg" src="images/common/ls_border.png">')
		}),
		
		items: Em.ContainerView.extend({
			classNames:	'ls-items',
			
			childViews: [
				'dialpadButton',
				'quickdialButton',
				'phonebookButton',
				'historyButton',
				'messagingButton',
				'settingsButton'
			],
			
			dialpadButton: MFT.Button.extend({
				elementId:			'phone_menu_dialpadButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.dialpad.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.view_phone_phone'),
				icon:				'images/phone/ico_phone.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'dialpad'
			}),
			
			quickdialButton: MFT.Button.extend({
				elementId:			'phone_menu_quickdialButton',
				
				classNames:			['ls-item lsp1_p','helpmode_box_shadow'],
				classNameBindings:	['MFT.States.phone.quickdial.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.view_phone_quickdial'),
				icon:				'images/phone/ico_quickdial.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'quickdial',
				
				videoPath:			'phone_Quick_dial',
				
				onHelpMode: function(){
					(MFT.helpMode) ? this.set('onDown',false) : this.set('onDown',true); 
				}.observes('MFT.helpMode')
			}),
			
			phonebookButton: MFT.Button.extend({
				elementId:			'phone_menu_phonebookButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.phonebook.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.view_phone_phonebook'),
				icon:				'images/phone/ico_phonebook.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'phonebook'
			}),
			
			historyButton: MFT.Button.extend({
				elementId:			'phone_menu_historyButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.history.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.view_phone_history'),
				icon:				'images/phone/ico_history.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'history'
			}),
			
			messagingButton: MFT.Button.extend({
				elementId:			'phone_menu_messagingButton',
				
				classNames:		   ['ls-item lsp1_p','helpmode_box_shadow'],
				classNameBindings:	['MFT.States.phone.messaging.active:phone_active'],
				textBinding:		  Em.Binding.oneWay('MFT.locale.label.view_phone_messaging'),
				icon:				 'images/phone/ico_messaging.png',
				action:			   'subState',
				target:			   'MFT.PhoneController',
				
				stateName:			'messaging',
				
				videoPath:			'phone_Messaging',
				
				onHelpMode: function(){
					(MFT.helpMode) ? this.set('onDown',false) : this.set('onDown',true); 
				}.observes('MFT.helpMode')
			}),
			
			settingsButton: MFT.Button.extend({
				elementId:			'phone_menu_settingsButton',
				
				classNames:		   ['ls-item lsp1_p','helpmode_box_shadow'],
				classNameBindings:	['MFT.States.phone.phoneSettings.active:phone_active'],
				textBinding:		  Em.Binding.oneWay('MFT.locale.label.view_phone_settings'),
				icon:				 'images/phone/ico_settings.png',
				action:			   'subState',
				target:			   'MFT.PhoneController',
				
				stateName:			'phoneSettings',
				
				videoPath:			'phone_Settings',
				
				onHelpMode: function(){
					(MFT.helpMode) ? this.set('onDown',false) : this.set('onDown',true); 
				}.observes('MFT.helpMode')
			})
		})
	}),
	
	/** End call message window */
	endCallMessage: Em.ContainerView.extend({

		elementId:	'phone_endCallMessage',
				
		childViews: [
            'text',
            'okButton'
		],
		
		text: MFT.Label.extend({
            elementId:	'phone_endCallMessage_text',
            
            contentBinding:	Em.Binding.oneWay('MFT.locale.label.view_phone_popUp_callEnded')
        }),
        
        okButton: MFT.Button.extend({
            elementId:	'phone_endCallMessage_okButton',
            
            text: 'OK',
            
            target: 'MFT.PhoneController',
            action: 'hideEndCallMessage',
            
            onDown: false
        })
	})
});
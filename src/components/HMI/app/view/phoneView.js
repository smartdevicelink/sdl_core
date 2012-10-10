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

MFT.PhoneView = Em.ContainerView.create({
	
	stateId:	'phone',
	
	elementId:	'phone',
 	
 	classNames:	'hidden',
	
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.phone.active:active_state'],
	
	
	/** Initial phone components */		
	childViews: [
		'info',
		'menu',
		'endCallMessage'
	],
	
	/** Info block */
	info: Em.View.extend({
		classNameBindings: ['MFT.helpMode:hide'],
		
		elementId:	'phone_info',
		
		template: Ember.Handlebars.compile(
			'{{#if MFT.PhoneController.model.readyForCall}}<div id="phone_info_name">Paul&rsquo;s Phone</div>{{/if}}'+
			'{{#if MFT.PhoneController.model.onCall}}'+
				'<div id="phone_info_name">Sarah <span id="phone_info_number">{{MFT.PhoneController.model.phoneStatusNumber}}</span><p>3:47</p></div>'+
			'{{/if}}'
		)
	}),
	
	/** Left menu */
	menu: Em.ContainerView.extend({
		elementId:	'phone_menu',
		
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
				'help',
				'helpDivider',
				'dialpadButton',
				'quickdialButton',
				'phonebookButton',
				'historyButton',
				'messagingButton',
				'settingsButton'
			],
			
			help: MFT.HelpBlock.extend({
				elementId:	'phone_menu_help'
			}),
		
			helpDivider: MFT.HelpDivider.extend({
				elementId:	'phone_help_divider'
			}),
			
			dialpadButton: MFT.Button.extend({
				elementId:			'phone_menu_dialpadButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.dialpad.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.phone'),
				icon:				'images/phone/ico_phone.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'dialpad'
			}),
			
			quickdialButton: MFT.Button.extend({
				elementId:			'phone_menu_quickdialButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.quickdial.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.quickdial'),
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
				textBinding:		Em.Binding.oneWay('MFT.locale.label.phonebook'),
				icon:				'images/phone/ico_phonebook.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'phonebook'
			}),
			
			historyButton: MFT.Button.extend({
				elementId:			'phone_menu_historyButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.history.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.history'),
				icon:				'images/phone/ico_history.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'history'
			}),
			
			messagingButton: MFT.Button.extend({
				elementId:			'phone_menu_messagingButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.messaging.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.messaging'),
				icon:				'images/phone/ico_messaging.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'messaging',
				
				videoPath:			'phone_Messaging',
				
				onHelpMode: function(){
					(MFT.helpMode) ? this.set('onDown',false) : this.set('onDown',true); 
				}.observes('MFT.helpMode')
			}),
			
			settingsButton: MFT.Button.extend({
				elementId:			'phone_menu_settingsButton',
				
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.phone.phoneSettings.active:phone_active'],
				textBinding:		Em.Binding.oneWay('MFT.locale.label.settings'),
				icon:				'images/phone/ico_settings.png',
				action:				'subState',
				target:				'MFT.PhoneController',
				
				stateName:			'phoneSettings',
				
				videoPath:			'phone_Settings',
				
				onHelpMode: function(){
					(MFT.helpMode) ? this.set('onDown',false) : this.set('onDown',true); 
				}.observes('MFT.helpMode')
			})
		})
	}),
	
	/** End call message window */
	endCallMessage: MFT.Label.extend({
		elementId:	'phone_endCallMessage',
		
		content:	'Call Ended'
	})
});
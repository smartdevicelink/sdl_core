/**
 * @name MFT.DialpadPhoneView
 * 
 * @desc Phone Dialpad module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/DialpadView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.DialpadPhoneView = Em.ContainerView.create({
	
	elementId:	'phone_dialpad',
	
	classNameBindings: ['MFT.States.phone.phone.active:active_state'],
	
	classNames: ['hidden'],
	
	childViews: [
		'help',
		'number',
		'keypad',
		'controls',
		'deleteButton',
		'dialButton',
		'endButton',
		'contactImage'
	],
	
	help: Em.View.extend({
		elementId:			'phone_dialpad_help',
		
		classNames:			'help',
		
		classNameBindings: ['MFT.helpMode:visible']	
	}),
	
	/** Number area */
	number: MFT.Label.extend({
		elementId:		'phone_dialpad_number',
		
		contentBinding:	Em.Binding.oneWay('MFT.PhoneController.model.dialpadNumber')
	}),
	
	/** Keypad block */
	keypad: Em.ContainerView.extend({
		elementId:	'phone_dialpad_keypad',
		
		childViews: [
			'key1',
			'key2',
			'key3',
			'key4',
			'key5',
			'key6',
			'key7',
			'key8',
			'key9',
			'key0',
			'keyStar',
			'keyDiez'
		],
		
		key1: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key1',
			
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'1',
			
			template:	Ember.Handlebars.compile(
				'1'
			)
		}),
		
		key2: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key2',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'2',
			
			template:	Ember.Handlebars.compile(
				'2</br><span class="words">ABC</span>'
			)
		}),
		
		key3: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key3',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'3',
			
			template:	Ember.Handlebars.compile(
				'3</br><span class="words">DEF</span>'
			)
		}),
		
		key4: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key4',
			
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'4',
			
			template:	Ember.Handlebars.compile(
				'4</br><span class="words">GHI</span>'
			)	
		}),
		
		key5: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key5',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'5',
			
			template:	Ember.Handlebars.compile(
				'5</br><span class="words">JKL</span>'
			)	
		}),
		
		key6: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key6',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'6',
			
			template:	Ember.Handlebars.compile(
				'6</br><span class="words">MNO</span>'
			)	
		}),
		
		key7: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key7',
			
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'7',
			
			template:	Ember.Handlebars.compile(
				'7</br><span class="words">PQRS</span>'
			)	
		}),
		
		key8: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key8',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'8',
			
			template:	Ember.Handlebars.compile(
				'8</br><span class="words">TUV</span>'
			)	
		}),
		
		key9: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key9',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'9',
			
			template:	Ember.Handlebars.compile(
				'9</br><span class="words">WXYZ</span>'
			)	
		}),
		
		key0: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key0',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'0',
			
			template:	Ember.Handlebars.compile(
				'0</br><span class="words">+</span>'
			)	
		}),
		
		keyStar: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_keyStar',
							
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'*',
			
			template:	Ember.Handlebars.compile(
				'*'
			)	
		}),
		
		keyDiez: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_keyDiez',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'#',
			
			template:	Ember.Handlebars.compile(
				'#'
			)	
		})
	}),
	
	controls: Em.ContainerView.extend({
		elementId:	'phone_dialpad_controls',
		
		childViews: [
			'privacyButton',
			'holdButton',
			'muteButton',
			'joinButton'
		],
		
		privacyButton: MFT.IndButton.extend({
			elementId:			'phone_dialpad_controls_privacyButton',
			text:				'Privacy',
			
			action:				'onPrivacy',
			target:				'MFT.PhoneController',
			contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dialpadPrivacy'),
			
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			
			disabledBinding:	Em.Binding.oneWay('disabledValue'),
			
			// Custom disable binding value
			disabledValue: function() {
								
				return ( MFT.PhoneController.model.readyForCall && !MFT.helpMode );
				
			}.property('MFT.PhoneController.model.readyForCall','MFT.helpMode'),
			
			helpMode:			true	
		}),
		
		holdButton: MFT.IndButton.extend({
			elementId:	'phone_dialpad_controls_holdButton',
			text:		'Hold',
			
			action:				'onHold',
			target:				'MFT.PhoneController',
			contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dialpadHold'),
			
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			
			disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.readyForCall'),			
		}),
		
		muteButton: MFT.IndButton.extend({
			elementId:	'phone_dialpad_controls_muteButton',
			text:		'Mute',
			
			action:				'onMute',
			target:				'MFT.PhoneController',
			contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dialpadMute'),
			
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			
			disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.readyForCall')
		}),
		
		joinButton: MFT.Button.create({
			elementId:			'phone_dialpad_controls_joinButton',
			text:				'Join',
			
			action:				'onJoin',
			target:				'MFT.PhoneController',
			
			disabled: function() {
				return !MFT.helpMode;
			}.property('MFT.helpMode'),

			helpMode:			true
		})
	}),
	
	deleteButton: MFT.Button.extend({
		elementId:		'phone_dialpad_deleteButton',
		
		classNameBindings: ['MFT.PhoneController.model.dialpadNumber.length:show'],
		
		icon:			'images/phone/del.png',
		
		action:			'onDelete',
		target:			'MFT.PhoneController',
		
		disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.onCall')
	}),
	
	dialButton: MFT.Button.extend({
		elementId:			'phone_dialpad_dialButton',
		
		text:				'Dial',
		icon:				'images/phone/dial.png',
		
		action:				'onDialCall',
		target:				'MFT.PhoneController',
		
		disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.onCall')			
	}),
	
	endButton: MFT.Button.extend({
		elementId:			'phone_dialpad_endButton',
		
		text:				'End',
		icon:				'images/phone/end.png',
		
		action:				'onEndCall',
		target:				'MFT.PhoneController',
		
		disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.readyForCall')
	}),
	
	contactImage: Em.View.extend({
		classNameBindings: ['MFT.PhoneController.model.onCall:show'],
		
		elementId:		'phone_dialpad_contactImage'
	})
})
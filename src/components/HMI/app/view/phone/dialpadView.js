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

MFT.DialpadPhoneView = Em.ContainerView.create(  MFT.LoadableView, {
		
	elementId:	'phone_dialpad',
		
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
		
		classNames:			['blured_background','shifted']
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
				'2<p class="words">{{MFT.locale.label.view_phone_abc}}</p>'
			)
		}),
		
		key3: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key3',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'3',
			
			template:	Ember.Handlebars.compile(
				'3<p class="words">{{MFT.locale.label.view_phone_def}}</p>'
			)
		}),
		
		key4: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key4',
			
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'4',
			
			template:	Ember.Handlebars.compile(
				'4<p class="words">{{MFT.locale.label.view_phone_ghi}}</p>'
			)	
		}),
		
		key5: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key5',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'5',
			
			template:	Ember.Handlebars.compile(
				'5<p class="words">{{MFT.locale.label.view_phone_jkl}}</p>'
			)	
		}),
		
		key6: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key6',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'6',
			
			template:	Ember.Handlebars.compile(
				'6<p class="words">{{MFT.locale.label.view_phone_mno}}</p>'
			)	
		}),
		
		key7: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key7',
			
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'7',
			
			template:	Ember.Handlebars.compile(
				'7<p class="words">{{MFT.locale.label.view_phone_pqrs}}</p>'
			)	
		}),
		
		key8: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key8',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'8',
			
			template:	Ember.Handlebars.compile(
				'8<p class="words">{{MFT.locale.label.view_phone_tuv}}</p>'
			)	
		}),
		
		key9: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key9',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'9',
			
			template:	Ember.Handlebars.compile(
				'9<p class="words">{{MFT.locale.label.view_phone_wxyz}}</p>'
			)	
		}),
		
		key0: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_key0',
			
			classNames:	'center',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'0',
			
			template:	Ember.Handlebars.compile(
				'0<p class="words">{{MFT.locale.label.view_phone_plus}}</p>'
			)	
		}),
		
		keyStar: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_keyStar',
							
			classNames:	'left',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'*',
			
			template:	Ember.Handlebars.compile(
				'{{MFT.locale.label.view_phone_mtpl}}'
			)	
		}),
		
		keyDiez: MFT.Button.extend({
			elementId:	'phone_dialpad_keypad_keyDiez',
			
			classNames:	'right',
			
			action:		'onKeyPress',
			target:		'MFT.PhoneController',
			
			keyData:	'#',
			
			template:	Ember.Handlebars.compile(
				'{{MFT.locale.label.view_phone_dies}}'
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
			
			classNames:			['helpmode_box_shadow','small','active_in_helpmode'],
			
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_phone_privacy'),
			
			action:				'onPrivacy',
			target:				'MFT.PhoneController',
			contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dialpadPrivacy'),
			
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			
			disabledBinding:	Em.Binding.oneWay('disabledValue'),
			
			// Custom disable binding value
			disabledValue: function() {
								
				return ( MFT.PhoneController.model.readyForCall && !MFT.helpMode );
				
			}.property('MFT.PhoneController.model.readyForCall','MFT.helpMode')
		}),
		
		holdButton: MFT.IndButton.extend({
			elementId:	'phone_dialpad_controls_holdButton',
			
			classNames:			['small'],
			
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_phone_hold'),
			
			action:				'onHold',
			target:				'MFT.PhoneController',
			contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dialpadHold'),
			
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			
			disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.readyForCall'),			
		}),
		
		muteButton: MFT.IndButton.extend({
			elementId:	'phone_dialpad_controls_muteButton',
			
			classNames:			['small'],
			
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_phone_mute'),
			
			action:				'onMute',
			target:				'MFT.PhoneController',
			contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dialpadMute'),
			
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			
			disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.readyForCall')
		}),
		
		joinButton: MFT.Button.create({
			elementId:			'phone_dialpad_controls_joinButton',
						
			classNames:			['helpmode_box_shadow','small','active_in_helpmode'],
			
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_phone_join'),
			
			action:				'onJoin',
			target:				'MFT.PhoneController',
			
			disabled: function() {
				return !MFT.helpMode;
			}.property('MFT.helpMode')
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
		
		textBinding:		  Em.Binding.oneWay('MFT.locale.label.view_phone_dial'),
		icon:				 'images/phone/dial.png',
		
		action:			   'onDialCall',
		target:			   'MFT.PhoneController',
		
		disabledBinding:	Em.Binding.oneWay('MFT.PhoneController.model.onCall')			
	}),
	
	endButton: MFT.Button.extend({
		elementId:			'phone_dialpad_endButton',
		
		textBinding:		  Em.Binding.oneWay('MFT.locale.label.view_phone_end'),
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
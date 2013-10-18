/**
 * @name MFT.WidgetPhoneView
 * 
 * @desc Phone home screen widget visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/WidgetPhoneView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.WidgetPhoneView = Em.ContainerView.extend({
	elementId:	'home_widgetPhone',
		
	childViews: [
		'icons',
		'label',
		'dndButton',
		'onCall'
	],
	
	icons: Em.View.extend({
		classNameBindings: [
			'MFT.PhoneController.model.onCall:hidden'
		],
		
		elementId:			'home_widgetPhone_icons'
	}),
	
	label: MFT.Label.extend({
		classNameBindings: [
			'MFT.PhoneController.model.onCall:hidden'
		],
		
		elementId:			'home_widgetPhone_label',
		contentBinding:		Em.Binding.oneWay('MFT.locale.label.view_wigetPhone_phone_connected')
	}),
	
	dndButton: MFT.IndButton.extend({
		
		classNameBindings: [
			'MFT.PhoneController.model.onCall:hidden',
		],
		
		classNames: ['helpmode_box_shadow'],
					
		elementId:			'home_widgetPhone_dndButton',
		textBinding:		  Em.Binding.oneWay('MFT.locale.label.view_wigetPhone_label'),
		
		contentBinding:		Em.Binding.oneWay('MFT.PhoneController.model.dnd'),
		action:				'onDND',
		target:				'MFT.PhoneController',
        stopPropagation:    true,
		indActiveClass:		'ind_act',
		indDefaultClass:	'ind_inact',
		
		onDown:				false
	}),
	
	onCall: Em.ContainerView.extend({
		
		elementId:			'home_widgetPhone_onCall',
		
		classNameBindings:	['MFT.PhoneController.model.readyForCall:hidden'],
		
		childViews: [
			'nameInfo',
			'callInfo',
			'endCallButton'
		],
		
		nameInfo: Em.View.extend({
			
			elementId:			'home_widgetPhone_nameInfo',
			
			template:			Ember.Handlebars.compile('<img src="images/phone/phone-ico.png"><span>Sarah</span>')
		}),
		
		callInfo: Em.View.extend({
			
			elementId:			'home_widgetPhone_callInfo',
			
			template:			Ember.Handlebars.compile(
				'<img src="images/phone/signal-ico.png"><span>3:47</span>'
			)
		}),
		
		endCallButton: MFT.Button.extend({
			
			elementId:			'home_widgetPhone_endCallButton',
			
			textBinding: 		  Ember.Binding.oneWay('MFT.locale.label.view_wigetPhone_end'),
			
			icon:				'images/phone/end.png',
			
			action:				'onEndCall',
			target:				'MFT.PhoneController',
            stopPropagation:    true,
			
			onDown:				false
		})
	}),

    /**
     * Select phone state
     * should be called when user press home phone block
     */
    actionUp: function(){
        if (MFT.helpMode) return false;

        MFT.States.goToState( MFT.PhoneController.activeState );
    }
});
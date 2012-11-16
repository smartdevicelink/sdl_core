/**
 * @name MFT.StatusPhoneView
 * 
 * @desc Phone statusbar visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/StatusPhoneView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.StatusPhoneView = Em.ContainerView.extend({
	
	elementId:	'status_phone',
	
	classNames: ['status_bar'],
	
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.phone.active:selected'],
	
	/** Climate components */		
	childViews: [
		'statusHome',
		'statusCall',
		'statusAntena'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.extend({
		elementId:			'status_phone_label',
		
		classNameBindings:	['MFT.States.home.active:visible'],
		
		contentBinding:	    Ember.Binding.oneWay('MFT.locale.label.view_statusPhone_phone')
	}),
	
	/** Phone status of call */
	statusCall: MFT.Label.extend({
		elementId:			'status_phone_call',
		
		classNameBindings:	['MFT.States.home.active:hidden_display'],			
		
		contentBinding:		Em.Binding.oneWay('MFT.PhoneController.phoneStatus')
	}),
	
	/** Antena icon */
	statusAntena: Em.View.extend({
		elementId:			'status_phone_antena',
		
		classNameBindings:	['MFT.States.home.active:hidden_display']
		
	}),
	
	/**
	 * Select phone state
	 * should be called when user press phone statusbar
	 */
	actionUp: function(event){
		MFT.States.goToState( MFT.PhoneController.activeState );
	}
});
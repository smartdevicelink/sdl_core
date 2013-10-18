/**
 * @name MFT.WOInfoView
 * 
 * @desc Warning representation
 * 
 * @category	View
 * @filesource	app/view/WOInfoView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.WOInfoView = Em.ContainerView.extend({
	
	elementId: 'wo-info-view',
	
	classNameBindings: ['MFT.StartUpController.isInfoViewHidden:hidden:active_state'],
	
	childViews: ['content','button'],
	
	content: MFT.Label.extend({	
		classNames: 'message',
		contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_woInfo_toWatch'), 

	}),
	
	button: MFT.Button.create({
		/** Element Id*/
		elementId: 'configueApp-button',
		textBinding: Ember.Binding.oneWay('MFT.locale.label.view_woInfo_configureApp'),
		target: 'MFT.StartUpController',
		action: 'onAppConfigPress',
		onDown: false
 	})
	
});
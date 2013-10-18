/**
 * @name MFT.woSkipButtonView
 * 
 * @desc Welcome orientation skipp button visual representation
 * 
 * @category	View
 * @filesource	app/view/woSkipButtonView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
 MFT.woSkipButtonView = MFT.Button.create({
	/** Element Id*/
	elementId: 'woSkipButton',
	/** Button appeares after 3 sec delay after Wo video started*/
	classNameBindings: ['MFT.StartUpController.isSkipButtonActive:visible_display','MFT.StartUpController.isSkipButtonHidden:hiddenButton'],
	textBinding: 'MFT.locale.label.view_skip_button',
	target: 'MFT.StartUpController',
	action: 'onSkipPress'
 });
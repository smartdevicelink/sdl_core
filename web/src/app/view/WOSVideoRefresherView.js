/**
 * @name MFT.woSkipButtonView
 * 
 * @desc Welcome orientation refresher view, that appears before the first start of Welcome orientation video.
 * (For Android only)
 * 
 * @category	View
 * @filesource	app/view/WOSVideoRefresherView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
 MFT.WOSVideoRefresherView = Em.View.extend({
	/** Element Id*/
	elementId: 'WOSVideoRefresher_view',
	/** Button appeares before the first start of Welcome orientation video.*/
	classNameBindings: ['MFT.StartUpController.isWOSVideoRefresherActive:visible_display']
 });
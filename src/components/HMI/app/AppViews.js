/**
 * @name MFT
 * 
 * @desc Setup application container for all views 
 * 
 * @category	Application
 * @filesource	app/AppViews.js
 * @version	2.0
 *
 * @author	Artem Petrosyan
 */

/** Appending views */
MFT.AppViews = Em.ContainerView.extend({
	
	elementId:	'app',
	
	classNameBindings: [
		'MFT.localization',
		'MFT.helpMode:helpmode'
	],
	
	childViews: [
		//MFT.WelcomeOrientationView,
		MFT.InteractionChoicesView,
		MFT.SelectSystempopup,
		//MFT.WOSkippButtonView,
		MFT.StatusClimateView,
		MFT.StatusMediaView,
		MFT.StatusNavigationView,
		MFT.StatusInfoView,
		MFT.StatusPhoneView,
		MFT.TopControls,
		MFT.BottomControls,
		MFT.TTSPopUp,
		MFT.AlertPopUp,
		MFT.VRPopUp,
		MFT.DriverDistraction,
		//MFT.PlayerControllsView,
		//MFT.VideoErrorPopupView,
		MFT.ControlButtons,
		'activeAview'
	],
	
	// Active module view container
	activeAview: Em.ContainerView.extend({
		elementId:	'active_view'
	}),

	/*
	 * This method is called when the app is fully rendered
	 * and ready to be displayed.
	 * We notify the backend to hide the splash
	 * and load internal view modules
	 */
	didInsertElement: function() {
		this._super();
		
		//Em.Logger.log('MFT Rendered!');
		//if ( FLAGS.MCS_ENABLED ) {
		//	WarningOkButtonView.appLoaded();
		//} else {
			FFW.Backend.notifyAppLoaded();
			MFT.set('appReady',true);
		//}
		
		// preload other views
		//FFW.AppendScript.pushScript('phone');
		FFW.AppendScript.pushScript('media');
		FFW.AppendScript.pushScript('media.applink');
		FFW.AppendScript.pushScript('media.applink.applinkoptions');
		FFW.AppendScript.pushScript('media.applink.applinkslider');
		FFW.AppendScript.pushScript('media.applink.applinkscrollablemessage');
		
		FFW.AppendScript.pushScript('info');
		FFW.AppendScript.pushScript('info.nonMedia');
		FFW.AppendScript.pushScript('info.nonMedia.options');

		// preload big images
		// to resolve blinking issue
		FFW.PreloadImages.preload();
	}
});

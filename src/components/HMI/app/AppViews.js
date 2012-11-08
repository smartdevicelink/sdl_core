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
	
	childViews: [
//		MFT.WelcomeOrientationView,
//		MFT.WOSkippButtonView,
		MFT.StatusClimateView,
		MFT.StatusMediaView,
		MFT.StatusNavigationView,
		MFT.StatusPhoneView,
		MFT.TopControls,
		MFT.BottomControls,
		MFT.TTSPopUp,
		MFT.UIPopUp,
		MFT.VRPopUp,
//		MFT.PlayerControllsView,
//		MFT.VideoErrorPopupView,
		MFT.ControlButtons,
		MFT.AppOptionsView,
		MFT.AppSubMenuView,
		MFT.AppSubMenuView,
		MFT.AppPerformInteractionChoise,
		'activeAview'
	],
	
	// Active module view container
	activeAview: Em.ContainerView.extend({
		elementId:	'active_view',
		
		childViews: [
			MFT.HomeView,
			MFT.ClimateView,
			MFT.NavigationView,
			MFT.PhoneView,
			MFT.MediaView,
			MFT.SettingsView,
			MFT.InfoView
		]
	}),
	
	init: function() {
		this._super();
	
		// for testing
		if (FLAGS.EXTENDED_LOG) {			
			Em.Logger.log( '* EXTENDED LOG IS ENABLED *' );
			Em.Logger.log( 'Analyze render time of each view' );
			Em.Logger.log( 'To disable this feature set FLAGS.EXTENDED_LOG to false' );
			
			MFT.startTime = Date.now();
			
			//var endTime = 0;
			Em.View.reopen({
				afterRender: function(){
					this._super();
										
					//endTime = Date.now() - startTime;
					
					// for debug only, to be removed from release code
					//this.sleep(20);
					
					Em.Logger.log(Date.now() - MFT.startTime + 'ms: ' +this.elementId );
					MFT.startTime = Date.now();
				},
				
				sleep: function(ms) {
					ms += new Date().getTime();
					while (new Date() < ms){}
				} 
			})
		}
		
		//IF IOS set warning view height - 1 px;(FIX AVA2-BUG_IOS-12 - (Brown line under app is shown after app is launched)) 
		if(FFW.isIOS){
			$('#warning_view').css('height','479px') ;
		}
		
	},

	/*
	 * This method is called when the app is fully rendered
	 * and ready to be displayed. We notify the backend to hide
	 * the splash.
	 */
	afterRender: function() {
		Em.Logger.log('MFT Rendered!');
		WarningOkButtonView.appLoaded();
//		FFW.Backend.notifyAppLoaded();
		
		FFW.PreloadImages.preload();
		
	}
});


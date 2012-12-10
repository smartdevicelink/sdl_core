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
		//FFW.AppendScript.pushScript('info');
		//FFW.AppendScript.pushScript('settings');
		
		// preload climate view
		//FFW.AppendScript.pushScript( 'climate' );
		// depends on vechicle type
		//FFW.AppendScript.pushScript( MFT.ClimateController.get('climateVechicleMode') );
		
		// preload navigation view
		// depends on sd card settings
		/*if ( FFW.Backend.isNavigationEnabled ) {
			FFW.AppendScript.pushScript('navigation');
		}
		*/
		// preload big images
		// to resolve blinking issue
		FFW.PreloadImages.preload();
		
		//MFT.TransitionsTest.init();
	}
});
/*
MFT.TransitionsTest = {
	
	startId: 0,
	
	querry: [],
	
	init: function() {
		this.loadChildStates(MFT.States.states);
	},
	
	start: function() {
		var self = this;
		
		this.startId = setInterval(function(){
			if ( self.querry.length ) {
				MFT.States.goToState( self.querry.shift() );
			}
		},100);
	},
	
	loadChildStates: function( states ) {
		for (var state in states ) {
		
			this.querry.push( states[state].get('path') );
			
			//console.log( states[state].get('path') );
						
			if ( !states[state].get('isLeaf') ) {
				this.loadChildStates(states[state].states);
			}
			
		}
	}
};
*/
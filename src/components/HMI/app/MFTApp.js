/**
 * @name MFT
 * 
 * @desc Setup application namespace and settings
 * 
 * @category	Application
 * @filesource	app/App.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT = Em.Application.create({
  	
	/** Override for mouse/touch events */
  	customEvents: ('ontouchstart' in document.documentElement) 
  					? { touchstart:	'actionDown', touchend:	'actionUp' , touchmove: 'actionMove'}
  					: { mousedown:	'actionDown', mouseup:	'actionUp', mousemove: 'actionMove' },
  					
  	
  	/** Help mode flag {boolean} */
  	helpMode: false,
 	
 	// determine home view {string}
 	homeView: 'home',
	
	/** Set language for localization */
	localization: 'eng',

	//Application ready flag
	appReady: false,
	
	// Debug text
	debugText: 'debug&nbsp;mode',
 	
	init: function() {
	        Em.Logger.log('MFT: Application init!');
        
        	FFW.Buttons.connect();
	        FFW.TTS.connect();
	        FFW.VR.connect();
	        FFW.AppLinkCoreClient.connect();
        	FFW.UI.connect();

	        this._super();
	},
	 	  	
  	/** Application ready event */
	ready: function() {
		Em.Logger.log('MFT: Application ready!');
		
		/** State manager init */
		MFT.States = StateManager.create();
		
		/** container for all views */
		MFT.views 	= MFT.AppViews.create().appendTo('body');
		
		if (FLAGS.APP_HIDE_MOUSE_CURSOR) {
			var style = document.createElement('style');
            style.setAttribute('type', 'text/css');
            style.innerHTML = '*{cursor:none !important}';
			
			document.getElementsByTagName("head")[0].appendChild(style);			
		}

		/** connect to video player engine */
		MFT.VideoPlayerController.connect();
	}
});
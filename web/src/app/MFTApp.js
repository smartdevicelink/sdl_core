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
  					? { touchstart:	'actionDown', touchend:	'actionUp' }
  					: { mousedown:	'actionDown', mouseup:	'actionUp', mousemove: 'actionMove' },
  					
  	
  	/** Help mode flag {boolean} */
  	helpMode: false,
 	
 	// determine home view {string}
 	homeView: 'home',

	//Application ready flag
	appReady: false,
	
	// Debug text
	debugText: 'debug&nbsp;mode',
 	
	init: function() {
		Em.Logger.log('MFT: Application init!');
		
		/* connect to backend*/
		if ( !FLAGS.MCS_ENABLED) {
			FFW.Backend.connect();
            FFW.PhoneBookRPC.connect();
		}

		this._super();
	},
	
	//Helper for Help Mode Variable reverse boolean State (Help mode should return false)
	reversHelpModeBoolean: function(){
		return (MFT.helpMode)? false: true;
	}.property('this.helpMode'),
	
	/**
	 * Property  to check franch localization
     *
     * @return {Boolean}
	 */
	isFr: function() {
	   return ( FLAGS.locale === 'fr' );
	}.property('FLAGS.locale'),
	  	
  	/** Application ready event */
	ready: function() {
		Em.Logger.log('MFT: Application ready!');
		
		/** State manager init */
		MFT.States = StateManager.create();
		
		/** container for all views */
		MFT.views 	= MFT.AppViews.create().appendTo('#wrapper');
		
		if (FLAGS.APP_HIDE_MOUSE_CURSOR) {
			var style = document.createElement('style');
            style.setAttribute('type', 'text/css');
            style.innerHTML = '*{cursor:none !important}';
			
			document.getElementsByTagName("head")[0].appendChild(style);			
		}

		/** connect to video player engine */
		MFT.VideoPlayerController.connect();
		
		/** connect to native rate component*/
		MFT.RateController.connect();
		
		/** set measurement systems to current localization*/
		MFT.SettingsController.setMeasurementSystems();
	}
});

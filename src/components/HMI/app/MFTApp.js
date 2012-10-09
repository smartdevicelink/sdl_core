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
  					? { touchstart:	'actionDown', touchend:	'actionUp' , touchleave: 'touchLeave'}
  					: { mousedown:	'actionDown', mouseup:	'actionUp' },
  					
  	
  	/** Help mode flag {boolean} */
  	helpMode: 	false,
 	
	init: function() {
		Em.Logger.log('MFT: Application init!');
		
		/* connect to backend*/
		if ( !FLAGS.MCS_ENABLED) {
			FFW.Backend.connect();
		}

		this._super();
	},
	 	  	
  	/** Application ready event */
	ready: function() {
		Em.Logger.log('MFT: Application ready!');
		
		/** Set localization  */
		MFT.locale.setLang(MFT.eng);
		
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
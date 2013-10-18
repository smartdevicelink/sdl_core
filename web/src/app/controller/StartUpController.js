/**
 * @name MFT.StartUpController
 * 
 * @desc App First Start Up logic
 * 
 * @category	Controller
 * @filesource	app/controller/StartUpController.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.StartUpController = Em.Object.create({
	
	/* Hidden State For Warning View*/
	isWarningHidden: false,
	
	/* Info View For further Welcome Orientation Video playing */
	isInfoViewHidden: true,
	
	/*
	 * Skip Button Visibility flag
	*/	
	isSkipButtonActive: false,
	
	/*
	 * Skip Button Hidden flag
	*/	
	isSkipButtonHidden: true,
	
	/*
	 * Flag to set all Statuses hidden while Welcome Orientation Video
 	 * is playing	
	 */
	isAllStatusHidden: false,
	
	/*
	 * Welcome orientation video refresher view visibility flag
	 */
	isWOSVideoRefresherActive: false,
	
	/*
	 * On Warning OK Button Press Logic 
	*/	
	onWarningSubmit: function(view){
		//Start Welcome Orientation Video Playing
		if(FFW.Backend.isFirstStart){
			
			// Play FAQ_Welcome_orientation video
			MFT.VideoPlayerController.start('FAQ_Welcome_orientation');
			
			// init observer only for First Application launch
			this.addObserver('MFT.VideoPlayerController.model.isReady', function(){
				if(MFT.VideoPlayerController.model.isReady){
					
					//disable player controlls
					MFT.VideoPlayerController.set('disableControlls',true);
					//hide Warrning View
					this.set('isWarningHidden', true);
					// hide all Statuses
					this.set('isAllStatusHidden', true);
					
					/** Show WOSVideoRefresher to refresh app view */
					this.set('isWOSVideoRefresherActive', true);
										
					/** Show Skip Button after 3 second delay*/
					var self = this;
					
					/**
					 * This function is used only for 
					 * fixing of Android 2.x vertion video player bug.
					 */
					this.skipButtonTimer = setTimeout(function(){
						self.set('isSkipButtonActive', true);
					},1);
					
					this.skipButtonTimer = setTimeout(function(){
						self.set('isSkipButtonHidden', false);
					},3000);	
									
				} else {
					
					//enable player controlls
					MFT.VideoPlayerController.set('disableControlls',false);

                    this.hideWarningWindow();
					
					//remove warning view (for Android 4.2.* video controls)
					view._parentView.remove();		
		
					// show info view
					this.set('isInfoViewHidden', false);
					//hide Skip button
					this.set('isSkipButtonActive', false);
					this.set('isSkipButtonHidden', true);
					// show all Statuses
					this.set('isAllStatusHidden', false);
					// remove this observer 
					this.removeObserver('MFT.VideoPlayerController.model.isReady', this );
				}
			});

		} else {			
			this.hideWarningWindow();
		}
		
	},

    /** Hide warning window */
    hideWarningWindow: function(){
        this.set('isWarningHidden', true);
    },

	/*
	 * On Skip Button Press Logic 
	*/
	onSkipPress: function(){
		/**  Stop Video */
		MFT.VideoPlayerController.stop();
		// show info view
		this.set('isInfoViewHidden', false);
		//hide Skip button
		this.set('isSkipButtonActive', false);
		this.set('isSkipButtonHidden', true);
		// show all Statuses
		this.set('isAllStatusHidden', false);
	},
	/*
	 * On App Config button press 
	*/
	onAppConfigPress: function(){
		// hide info view
		this.set('isInfoViewHidden', true);
		//hide Skip button
		this.set('isSkipButtonActive', false);
		this.set('isSkipButtonHidden', true);
		/** Show System Select Menu*/
		MFT.AppController.set('active',true);
		/** Clear Skipp Button timer (in case the video is closed before 3 seconds ) */
		clearTimeout(this.skipButtonTimer);
	}
	
	
});
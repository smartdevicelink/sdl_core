/**
 * @name MFT.VideoPlayerController
 * 
 * @desc VideoPlayer model logic
 * 
 * @category	Controller
 * @filesource	app/controller/VideoPlayerController.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
MFT.VideoPlayerController = Ember.Object.create({
	
	/** Video Player Model Binding*/
	modelBinding: (FFW.isAndroid || FLAGS.EMULATE_ANDROID) ?  'FFW.VideoPlayerRPC' : 'MFT.HTMLVideoPlayer',
	
	/** Player Controlls Events
	 *	0 - Normal Video PlayBack
	 *  1 - Drag
	 *	2 - FF
	 *	3 - REW
	*/
	actionInProgress: 			0,
	/** Switch to selected state on video resume in order to avoid unnecessary state switch on video start */
	exitState: 					null,
	
	/** FF or REW video steps in seconds*/
	const_seekVideoStep: 		0.5,
	
	/** Width of progress bar in pixels*/
	progreebarWidth: 		507,
	
	/** Value in percents*/
	const_maxProgressBarWidth: 	90.7,
	
	/** Value in percents*/
	const_minProgressBarWidth: 	0,

	/** Value in pixels*/
	progressBarLeftOffsetValue : 93,
	
	/** init aniamtion interval variable for progressbar*/
	animationInterval: 			null,
	
	/**
	 * X Coordinate of Cursor or Finger on the screen 
	*/
	dragXCoordinate:			0,
	
	/**
	 * position on progress bar in percents (%)
	 */
	positionOnProgressBar: 		0,
	
	/**Video Ended event flag */
	isEnded: false,
	/** Disable VIdeo Controls*/
	disableControlls: false,
	
	/**
	 * Fix for Player controlls hide effect in Androis version 4.2.0
	 * Hide class is provided with css3 frame animation that fixes refresh of player controlls above 
	 * android video layer 
	*/
	hideAnimation : false,
	
	checkForAndroidVersion: function(){
		if(FFW.isAndroid && (FFW.Backend.OSInfo.OSVersion > '4.1.0' || FFW.Backend.OSInfo.OSVersion === 'unknown')){
			this.set('hideAnimation',true);	
		}
	}.observes('FFW.Backend.OSInfo.OSVersion'),
	
	/**
	 * for Android only
	 * indicates if new position was set on progress video bar
	 * it needed to avoid wrong notificztion with current video time after setting new position
	 */
	isTaped: false,
	
	/**
	 * Calculate progressbar width only for ANDROID
	 */
	
	calculateProgressBarOffsets: function(){
		if(this.model.isReady){
			if(FFW.isAndroid){
				this.set('progreebarWidth', $('#player_controlls_progressbar').width() * FFW.Scale.scalePoint);
			}
		}
	}.observes('FFW.Scale.scalePoint','this.model.isReady'),
	
	/**
	 * start video playback
	 * 
	 * videoPath	name of video
	 * exitState 	state to be activate when video is stopped 
	 */
	start: function(videoPath, exitState){
		this.setDragXCoordinate();
		//reset for Android video time variables because of incoming msgs after video stop
		if(FFW.isAndroid){
			/** reset progressbar position */
			this.set('positionOnProgressBar',0);
			/** reset buffer time */	
			this.set('bufferVideoCurrentTime',0);
			/** reset model time */
			this.model.set('currentVideoTime',0);
		}
		this.model.start(videoPath);
		
		if (exitState && exitState != MFT.States.currentState.getPath('path')) {
			this.set('exitState', exitState);
		}
		
		this.initListenerMouseUpEvent(); // mouseup or touch  event listener initialization
	},
	
	onVideoStartStateTransition: function(){
		if(this.model.isReady){
			if(this.exitState != null){
				MFT.States.goToState(this.exitState);
			}
		}else{
			this.set('exitState', null);
		}
		
	}.observes('this.model.isReady'),
	/** 
	 * Proceed on State change if video is on error 	
	*/
	onErrorVideoStateProceed: function(){
		if ( this.exitState ) {
			MFT.States.goToState( this.exitState );
			this.set( 'exitState', null );
		}
	}.observes('this.model.isError'),
	
	/** Calculate currentVideoTime according to drag event (DragXCorrdinate) */
	currentDragCoordinateToTime:function(){
		if(typeof this.model.totalDuration != 'undefined'){	
			if (this.dragXCoordinate > this.progreebarWidth)
				return this.model.totalDuration;
			else if (this.dragXCoordinate < 0)
				return 0;
			else 
				return (this.dragXCoordinate / this.progreebarWidth)  *  this.model.totalDuration;
		}
	},
	
	 /** Current playback time in String representation*/
	 currentPlayBackTime: function() {
		 if(this.actionInProgress == 0){
		 	return this.formatTime(this.model.currentVideoTime);
		 }else{
	 		return this.formatTime(this.bufferVideoCurrentTime);
		 }
	 }.property('this.model.currentVideoTime','this.bufferVideoCurrentTime'),
	 
	 /** Current playback time in String representation*/
	 totalDuration: function() {
		 return this.formatTime(this.model.totalDuration);
	 }.property('this.model.totalDuration'),
	 
	 /** Format to String Total video duration*/
	 formatTime: function(value) {
		  var minute =  Math.floor(value/60);
		  var second = (value % 60).toFixed(0);
	      return minute+":"+ (second < 10 ? '0' : '' ) + second; 
	 },
	 
	/** 
	 *	Observable variable 
	 *
	 *  @return CSS Style String with current position on progress bar
	 */
	progressBarStyle: function() {
		 if(this.positionOnProgressBar >= this.const_maxProgressBarWidth){
		 	return 'width:'+this.const_maxProgressBarWidth+'%';
		 }else if (this.positionOnProgressBar <= this.const_minProgressBarWidth){
		 	return 'width:'+this.const_minProgressBarWidth+'%';
		 }else {
		 	return 'width:'+this.positionOnProgressBar+'%';
		 }
	}.property('positionOnProgressBar'),
	
	/** Show Video player controlls*/
	showControlls: function() {
		if(MFT.PlayerControllsView)
			MFT.PlayerControllsView.set("isHidden", !this.model.isReady);
	}.observes('this.model.isReady'),
	
	/** 
	 *  Connect to video player engine
	 *  Android 	- over RPC 
	 *  other OSes 	- actually does nothing  
	 */
	connect: function(){
		this.model.connect();
	},
	/**
	 * Set X coordinate of Cursor or Finger on the screen
	 *
	 */
	setDragXCoordinate: function(){
				
			var self = this;
			
			// Check for Event type (Mouse or Touch)
			if('ontouchstart' in document.documentElement){
				var type = 'touch';
			}else{
				var type = 'mouse';
			}
			
			// Event hadler 
			document['on' + type + 'move'] = document['ontouchstart'] = function(e){
				//Check for event
				if(!e){
					e = window.event;
				}
				e.preventDefault();
				//MouseMove (Desktop/Browser Support)
				if(e.type == 'mousemove'){
					self.set('dragXCoordinate', e.pageX - $('#player_controlls_progressbar').offset().left);
				// TouchMove (Mobile (Android/IOS) Support)
				}else if(e.touches[0]){
					self.set('dragXCoordinate', e.touches[0].pageX - $('#player_controlls_progressbar').offset().left);
				} 
			}
	},
	
	/**
	 * Start Drag events (Drag handler event or Tap on progress bar event)
	*/	
	startDragging: function(){
		this.set('actionInProgress', 1); // Drag
		this.startUpdateTimer(); // progress bar animation timer
	},
	
	/**
	 * Fast Forward event
	*/
	FF: function(){
		this.set('actionInProgress', 2); // FF
		this.startUpdateTimer(); // progress bar animation timer
	},
	
	/**
	 * REW event
	*/
	REW: function(){
		this.set('actionInProgress', 3); // REW
		this.startUpdateTimer(); // progress bar animation timer
	},
	
	/**
	 * Start video playback if paused = false; pause if paused = true;
	 *
	*/
	play: function(){
		if(this.model.isPaused){
			this.model.play();
		}else{
			this.model.pause();
		}
	},
	
	/**
	 * Manual video player stop from State Manager
	 */
	close: function() {
		this.stop();
	},
	
	/*
	 * Start video playback if paused = false; pause if paused = true;
	 *
	 */
	pause: function(){
		this.model.pause();
	},
	
	/**
	 * Stop video playback (video and controlls destroyed, video data is removed from device memory)
	 *
	 */
	stop: function(){
		if (this.model.isReady)
		{
			
			this.model.stop();
			/** reset isPaused flag*/
			this.model.set('isPaused', null);
			/** reset progressbar position */
			this.set('positionOnProgressBar',0);
			/** reset buffer time */	
			this.set('bufferVideoCurrentTime',0);
			/** reset model time */
			this.model.set('currentVideoTime',0);
			/** reset model ended event flag */
			this.set('isEnded',false);
			
			// reset event listener
			document.onmouseup = document.ontouchend = null;
			
			// reset event listener
			document.onmousemove = document.ontouchstart = null;
			
			// reset event listener
			document.ontouchmove = document.ontouchstart = null;		
		}
	},
	/**
	 * Close Player On State Transition ready event
	 *
	*/
	onStateTransition: function() {	
		if ( this.model.isReady ){
			if( MFT.TransitionIterator.ready ) {
				 if(this.exitState == null ) {
					this.close();
				}
				this.set('exitState', null);
			}
		}
	}.observes('MFT.TransitionIterator.ready'),
	
	/** 
	 *Tap on progressbar event
	 */
	tap: function(){
		if (FFW.isAndroid)
			this.isTaped = true;
		this.startDragging();
	},
	/** On Ended Event handler, stops the video if current time is equal to total video duration*/
	onEnded: function(){
		if(this.model.currentVideoTime >=this.model.totalDuration || 
			this.bufferVideoCurrentTime >= this.model.totalDuration){
			
			this.set('isEnded',true);
			/** Stop video in case of normal video playback */
			if(this.actionInProgress == 0){
				this.stop()
			}
		}else{
			this.set('isEnded',false);
		}
	}.observes('this.model.currentVideoTime','this.bufferVideoCurrentTime'),
	/**
	 * Hide controlls while video is Playing
	 *
	 * Reset timer for hidding controls in such cases
	 * 1- Video paused
	 * 2- Drag, FF, REW or Tap actions  	
	 * 3- Moseup or touchend event on screen
	*/
	initHideControlls: function(){
			if(this.model.isReady){
				if(this.actionInProgress == 0 && !this.model.isPaused){
					var self = this;
					/** Fire Ember run loop*/
					this.hideTimeout = Ember.run.later(function(){
						MFT.PlayerControllsView.set("isHidden", true); // set true to  hidden class to controls
					},3000);
				}else{
					Ember.run.cancel(this.hideTimeout); // cancel timer
				}
			}
	}.observes("this.actionInProgress","this.model.isPaused","this.model.isReady"),
	
	/**  Listen to document onmoueup or touchend event to
	  *  stop player control events and
	  *  set current seeked position to  Video Player   
	  */
	initListenerMouseUpEvent: function(){
		var self = this;
		
		if (FFW.isAndroid){
			this.isTaped = true;
		}

		document.onmouseup = document.ontouchend  = function(e){	
			
			clearInterval(self.animationInterval);
			if(self.model.isReady){
				MFT.PlayerControllsView.set("isHidden", false);	 // set false to  hidden class to controls
			}
			Ember.run.cancel(self.hideTimeout); // cancel timer
			
			self.initHideControlls(); // init
			
			if(self.actionInProgress != 0){
				// Set current seeked time position to video player
				if(self.bufferVideoCurrentTime.toFixed(1) != self.model.totalDuration.toFixed(1)){
					self.model.setPosition(self.bufferVideoCurrentTime);
				}
				
				// resume video if it was paused
				if (self.model.wasPaused) {
					self.play();
				}
				// If video was seeked to end 
				if(self.isEnded){
					self.stop();
				}
				//Take off mark higherlighting on mouseup or touch end event
				MFT.PlayerControllsView.progressbar.dragArea.mark.set('active',false);
				//deactivate drag in Android
				if (FFW.isAndroid && parseFloat(FFW.Backend.OSInfo.OSVersion) < 4){
					
					FFW.VideoPlayerRPC.sendDragState(false);
				}
				
				self.set('actionInProgress',0);
			}
			
		}
	},
	
	/**Timer for Update progress bar handler position*/
	startUpdateTimer: function(){
		// save the current video player state (playing/paused) in another variable
		// in order to know whether to resume the video after the video was seeked to new position
		if(!this.model.isPaused){
			this.model.set('wasPaused', true);
			this.pause();	
		}else{
			this.model.set('wasPaused', false);
		}
		/** Set current video time to buffer time*/
		this.set('bufferVideoCurrentTime', this.model.currentVideoTime);
		// clear timer
		if(this.animationInterval){
			clearInterval(this.animationInterval);
		}
		// start animation interval
		this.animationInterval = setInterval(function(){ 
			MFT.VideoPlayerController.updateProgressBar();
		},25);
	},
	
	/**
	 * ProgressBar width is represented in precents 100% of progressBar Width = 100 % video duration
	 * Video duration and progressBar width are synchronized
	*/
	getPositionOnProgressBarInPercent: function(){
		var current = (this.actionInProgress == 1) ? this.currentDragCoordinateToTime() : this.bufferVideoCurrentTime;
		return (current / this.model.totalDuration) * this.const_maxProgressBarWidth;
	},
	
	/** Update progressbar position on video current time normal playback change*/
	onCurrentVideoTimeUpdate: function(){
		if(this.actionInProgress == 0){
			/** Set Current Video Time to Controllers Buffer Time*/
			this.set('bufferVideoCurrentTime', this.model.currentVideoTime);
			/** Update Progressbar position*/
			this.updateProgressBar();
		}
	}.observes('this.model.currentVideoTime'),
	/** 
	 *Set progress Bar Position  acoording to seeked time
	 *
	 */
	updateProgressBar: function(){			  
		if(this.model.isReady){
			switch (this.actionInProgress){	
			  case 1: { // is Dragging
				 this.set('bufferVideoCurrentTime', this.currentDragCoordinateToTime());
				 break;
			  }
			  case 2: { // is FF
				   this.set('bufferVideoCurrentTime', 
							(this.bufferVideoCurrentTime +  this.const_seekVideoStep) <= this.model.totalDuration ? 
							this.bufferVideoCurrentTime + this.const_seekVideoStep: this.model.totalDuration);
				  break;
			  }
			  
			  case 3:{ //is REW
					 this.set('bufferVideoCurrentTime', 
							(this.bufferVideoCurrentTime -  this.const_seekVideoStep) >= this.const_minProgressBarWidth ? 
							this.bufferVideoCurrentTime - this.const_seekVideoStep: this.const_minProgressBarWidth);
					 break;
			  }
			}
			this.set('positionOnProgressBar',this.getPositionOnProgressBarInPercent());
		}
	},
	
	/** 
	 * Resume video if application was playing before going to background
	 * Pause video if application goes to background and video is playing
	 */
	 handleApplicationActivity: function () {
		if (FFW.Backend.isInForeground) {
			// resume video if it was paused
			if (!this.model.wasPaused) {
				this.play();
			}
		} else {
			// save the current video player state (playing/paused) in another variable
			// in order to know whether to resume the video after the app has become active
			this.model.set('wasPaused', this.model.isPaused);
			this.pause();
		}
	}.observes('FFW.Backend.isInForeground'),
	
	/** 
	 * Close Video Error PopUp
	 * Reset video error Code
	 */
	
	closeErrorPopup: function(){
		this.model.set('errorCode',0);
		this.model.set('isError',false);
	},
	
	/**
	  * Error Code == 10 (No network connection)
	  * Error Code == 11 (If video file is missing when network is available but video still downloading in the background)
	  *	Error Code == 12 (Video File is corrupted)
	  * Error Code == 13 (If network is available, but video is corrupted and maximum download attempt value has been reached)
	 */	
			
	errorMessage: function(){
		
		switch(this.model.errorCode){
			
			case 10: {
				return {'line0': MFT.locale.label.videoerrorpopupview_no_video_code_10_message}; 
				break;
			}
			
			case 11: {
				return {'line0': MFT.locale.label.videoerrorpopupview_no_video_code_11_message_p0,
						'line1': '(' + this.getErrorData() + MFT.locale.label.videoerrorpopupview_no_video_code_11_message_p1,
						'line2': MFT.locale.label.videoerrorpopupview_no_video_code_11_message_p2
					   }
				break;
			}
			
			case 12: {
				return {'line0': MFT.locale.label.videoerrorpopupview_no_video_code_12_message}; 
				break;
			}
			
			case 13: {
				return {'line0': MFT.locale.label.videoerrorpopupview_no_video_code_13_message}; 
				break;
			}
			
			case 14: {
				return {'line0': MFT.locale.label.videoerrorpopupview_no_video_code_14_message}; 
				break;
			}
			
		}
	}.property('this.model.errorCode', 'FLAGS.locale'),
	
	// Always returns data, length of 2 chars
	getErrorData: function(){
		var data = this.model.errorData;
		if ( this.model.errorData < 10 ){
			var data = ' ' + this.model.errorData
		}
		
		return data
	}
});
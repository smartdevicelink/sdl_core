/**
 * @name MFT.HTMLVideoPlayer
 * 
 * @desc VideoPlayer model 
 * 
 * @category	model
 * @filesource	app/model/HTMLVideoPlayerModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.HTMLVideoPlayer =  Ember.Object.create({
	 /** Video object refference*/
	 video: 			null,
	 
	 /** Overal video duration in Seconds*/
	 totalDuration: 	0,

	 /** Ready State of video content to start playing*/
	 isReady: 			false,
	 
	 /** Video Paused state*/
	 isPaused: 			false,
	 
	 /** Current video time*/
	 currentVideoTime: 	0,
	 /** Video Error flag*/
	 isError: false,
	 /**
	  * Error Code == 10 (No network connection)
	  * Error Code == 11 (If video file is missing when network is available but video still downloading in the background)
	  *	Error Code == 12 (Video File is corrupted)
	  * Error Code == 13 (If network is available, but video is corrupted and maximum download attempt value has been reached)
 	 */	
	 errorCode: 0,
	 /**
	  *	Error Data == Persents of downloading video files
	  * !=0 only for errorCode = 11
 	 */	
	 errorData: 0,
	 /**
	  * to support seamless interface with VideoPlayerRPC
	  */
	 connect:function(){
		// do nothing 
	 },

	 /**
	  * Start video palying (Current clicked element passed to function)
	 */
	 start:function(name){
		 
		 //Init new DOM HTML5 VIdeo Object
		 // Due to memory leak , unstable work on IOS we should init video object every time on video start event
		 if(!$('#html5Player')[0]){
			 $('#app').prepend('<video'+
								' id="html5Player"'+
								' ontimeupdate="MFT.HTMLVideoPlayer.timeupdate()"'+ 
								' onerror="MFT.HTMLVideoPlayer.onError()"'+ 
								' webkit-playsinline>'+
							'</video>');  // new instance of HTML5 video inited
				
		 }
		 this.set('video',$('video').get(0)); // new video object inited 
		 
		 this.checkForOSsupportedFormat(name) // check for Platform 
		 
		 this.video.load(); // video start precaching
		 
		 // Set onloaded metada of video event listener
		 $(this.video).bind('loadedmetadata', function(e) {
			e.preventDefault();
			MFT.HTMLVideoPlayer.set('totalDuration', this.duration); // set video duration in Seconds
			$('video').addClass('active_state');
			this.play(); // play video event
			MFT.HTMLVideoPlayer.set('isReady', true);
		 });
		 
	 },
	 
	 checkForOSsupportedFormat: function(videoTitle){
		if(FFW.isIOS){
			// IOS
			$(this.video).attr('src',"video/" + videoTitle+ ".mp4");										
		}else{
			//DESKTOP
			$(this.video).attr('src',"video/" + videoTitle+ ".webm");									
		}
	 },
	 
	 /**Abort play back*/
	 stop:function(){
		this.set('isReady', false); // hide player
		this.cleanUpMemory();
	 },
	 
	cleanUpMemory: function(){
		$(this.video).attr('src',''); // pass empty src to player, force to reset previous played video
		this.video = null; // remove video object from memory
		$('video').removeClass('active_state'); // remove active class from player
		this.set('totalDuration' ,0); // refresh total duration to 0

		// remove video tag reference, clear all player listeners
		var removeVideoTag = function() {
			$('video').remove();
		};
		// fix QuickTime icon on iOS 5 when stopping video from paused state
		if (this.isPaused) {
			setTimeout(removeVideoTag, 1);
		} else {
			removeVideoTag();
		}
	},

 	 /**Native HTML5 time update event */
	 timeupdate:function(e){
		if(this.video)
		if(!this.video.seeking){
		 	this.set('currentVideoTime', this.video.currentTime);
		}
	 },
	 
	 /**Play video*/
	 play: function(){
		if (this.video == null) return;
		this.video.play();
		this.set('isPaused', false);
	 },
	 
	 /**Pause Video*/
	 pause: function(){
		if (this.video == null) return;
		this.video.pause();
		this.set('isPaused', true); 
	 },
	 
	 /** Set current seeked time to video*/
	 setPosition: function(seekedTime){
		 if (this.video == null) return;
		 this.set('currentVideoTime', seekedTime);
		 this.video.currentTime = seekedTime;
	 },
	 
	 /*On load video event*/
	 onLoad: function() {
		var self = this;
		setTimeout(function(){
			self.set('totalDuration', self.video.duration); // set video duration in Seconds
			self.video.play(); // play video event
			self.set('isReady', true);
		},200)
		
	 },
	 
	 /** Errors*/
	 onError: function(){
		/**
		* 	Error states
		*
		*	MEDIA_ERR_ABORTED (1) 				User aborted video playback
		*	MEDIA_ERR_DECODE (2) 				Decoding error, video is broken or the codec makes problems
		*	MEDIA_ERR_NETWORK(3) 				Network error (could not read the stream)
		*	MEDIA_ERR_SRC_NOT_SUPPORTED (4) 	The format is not supported
	    */
		if(this.video != null) 
			switch (this.video.error.code) {
			   case 1:
				  //console.log("Play back aborted.");
				  break;
			   case 2:
				  //console.log("Network error - please try again later.");
				  break;
			   case 3:
				  //console.log("Video is broken..");
				  break;
			   case 4:
			   		 $('video').remove(); // remove video tag reference, clear all player listeners
				   	 // Set Error Code
					 this.set('errorCode' , 12);
					 this.set('isError', true);
				  	 break;
			}
		 
	  }

});
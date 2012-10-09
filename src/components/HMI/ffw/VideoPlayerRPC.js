/*
 * Reference implementation of VideoPlayer component.
 * 
 * This class is activate on Android OS only
 * since embedded video playback is not supported in Android 2.x and 3.x
 * So Anrdoid native video player is used to play video
 *
 * @author Sergey Solovey
 */

FFW.VideoPlayerRPC = FFW.RPCObserver.create({
	 
	 /** Overal video duration in Seconds*/
	 totalDuration: 	0,

	 /** Ready State of video content to start playing*/
	 isReady: 			false,
	 
	 /** Video Paused state*/
	 isPaused: 			false,
	 
	 /** Video Error state*/
	 isError: 			false,
	 
	 /** Current video time*/
	 currentVideoTime: 	0,
	 
	 /** New position for tapping*/
	 newPosition: 	-1,
	 
	 /*
	 * identifiers for requests
 	 */		
	 startRequestId: 				-1,
	 stopRequestId: 				-1,
	 pauseRequestId: 				-1,
	 resumeRequestId: 				-1,
	 setPositionRequestId:			-1,
	 getPositionRequestId: 			-1,
	 
	 onAppStateChangedSubscribeRequestId:	-1,
	 onPositionChangedSubscribeRequestId:	-1,
	 
	 /**
	  * Start video palying (Current clicked element passed to function)
	 */
	 start:	function(name){
		 
		var videoName = name + ".mp4";
		 
		if (FLAGS.VIDEO_PLAYER_JS) {
			Android.videoStart(videoName, 
				FFW.Scale.scalePoint,
				$('body').offset().top, 
				$('body').offset().left);
		} else {
			this.startRequestId = this.client.generateId();
	
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.startRequestId,
				"method":	"VideoPlayer.start",
				"params": 	{
					"videoName": 	videoName,
					"scale": 		FFW.Scale.scalePoint,
					"position":  {
						"x": $('body').offset().left,
						"y": $('body').offset().top
					}
				}
			};
			this.client.send(JSONMessage);
		}
		
		//this.set('isReady', true);
	 },
	 
	 
	 /**
	  * Change backround color to transparent for Android native player visibility 
	 */
	 hideBackground: function(){
		 if(this.isReady){
		 	$('body').css('background-color', 'transparent');
			$('#active_view').addClass('hidden');
		 }else{
		 	$('body').css('background-color', '#000000');
			$('#active_view').removeClass('hidden');
		 }
	 }.observes('isReady'),
	 
	 /**Abort play back*/
	 stop:	function(){
		 
		if (FLAGS.VIDEO_PLAYER_JS) {
			Android.videoStop();
		} else {
			this.stopRequestId = this.client.generateId();
	
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.stopRequestId,
				"method":	"VideoPlayer.stop"
			};
			this.client.send(JSONMessage);
		}
		
		this.set('isReady', false);
	 },
	 
 	 /**Play video*/
	 play: function(){
		if (FLAGS.VIDEO_PLAYER_JS) {
			Android.videoPlay();
		} else {
			this.resumeRequestId = this.client.generateId();
	
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.resumeRequestId,
				"method":	"VideoPlayer.resume"
			};
			this.client.send(JSONMessage);
		}
		
		this.set('isPaused', false);
	 },
	 
	 /**Pause Video*/
	 pause: function(){
		if (FLAGS.VIDEO_PLAYER_JS) {
			Android.videoPause();
		} else {
			this.pauseRequestId = this.client.generateId();
	
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.pauseRequestId,
				"method":	"VideoPlayer.pause"
			};
			this.client.send(JSONMessage);
		}
		
		this.set('isPaused', true); 
	 },
	 
	 /** Set current seeked time to video*/
	 setPosition: function(seekedTime){
		Em.Logger.log("FFW.VideoPlayer.set position = " + seekedTime); 
		 
		if (FLAGS.VIDEO_PLAYER_JS) {
			 Android.videoSetPosition(seekedTime);
		} else {
			this.setPositionRequestId = this.client.generateId();
	
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.setPositionRequestId,
				"method":	"VideoPlayer.setPosition",
				"params": 	{
					"position": seekedTime,
				}
			};
			
			this.client.send(JSONMessage);
			this.newPosition = seekedTime;
		}

        this.set('currentVideoTime', seekedTime);
	 },
	
	 /** Get current seeked time to video*/
	 getPosition: function(seekedTime){
		if (FLAGS.VIDEO_PLAYER_JS) {
			 Android.videoSetPosition(seekedTime);
		} else {
			this.getPositionRequestId = this.client.generateId();
	
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.getPositionRequestId,
				"method":	"VideoPlayer.getPosition"
			};
			this.client.send(JSONMessage);
		}

        this.set('currentVideoTime', seekedTime);
	 },
	 		
	/*
     *	access to basic RPC functionality
 	 */		
	client:		FFW.RPCClient.create({ componentName: "VideoPlayerClient" }),
	
	connect: function() {
		if (!FLAGS.VIDEO_PLAYER_JS) {
			this.client.connect(this);
		}
	},

	disconnect: function() {
		this.client.disconnect();
	},

	onRPCRegistered: function () {
		this._super();
		Em.Logger.log("FFW.VideoPlayer.onRPCRegistered");
		
		//subscribe on position changed
		this.onPositionChangedSubscribeRequestId = this.client.generateId();
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.onPositionChangedSubscribeRequestId,
			"method":	"MB.subscribeTo",
			"params":	{
				"propertyName": "VideoPlayer.positionChanged"
			}
		};
		this.client.send(JSONMessage);
	},
	
	onRPCUnregistered: function () {
		this._super();
		Em.Logger.log("FFW.VideoPlayer.onRPCUnregistered");
	},
    
	/*
     * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onRPCResult: function(response) {
		this._super();
		
		if (response.id == this.startRequestId)
		{
			this.set('totalDuration', response.result.totalDuration / 1000);
			this.set('isReady', true);
		} else if (response.id == this.stopRequestId)
		{
		} else if (response.id == this.pauseRequestId)
		{
		} else if (response.id == this.resumeRequestId)
		{
		} else if (response.id == this.setPositionRequestId)
		{
		} else if (response.id == this.getPositionRequestId)
		{
			this.set("currentVideoTime", response.result.position);
		} else if ( response.id == this.onAppStateChangedSubscribeRequestId ||
					response.id == this.onPositionChangedSubscribeRequestId)
		{
			this.checkSubscriptionResponse(response);
		}
	},
	
	/*
 	 */	
	onRPCError: function(error) {
		this._super();

		this.set('isError',true);
	},

	/*
 	 */	
	onRPCNotification: function(notification) {
		this._super();

		if (notification.method == "VideoPlayer.stateChanged") {
			// TODO
		} else if (notification.method == "VideoPlayer.positionChanged") {
			var currTime = notification.params.currentPosition / 1000;
			// Only fo Android
			// check if after tapping on video scroll bar and setting new position
			// right current time is received.
			// Set 1 second for Android to change and send new current position
			if (FFW.isAndroid && MFT.VideoPlayerController.isTaped)
			{
				Em.Logger.log("===============================");
				Em.Logger.log("newPosition = " + this.newPosition);
				Em.Logger.log("currTime = " + currTime);
				Em.Logger.log("isTaped = " + MFT.VideoPlayerController.isTaped);
				Em.Logger.log("===============================");
				if(this.newPosition >=0 && 
					currTime >= this.newPosition && 
					currTime <= this.newPosition + 1)
				{
					this.newPosition = -1;
					MFT.VideoPlayerController.isTaped = false;
					this.set("currentVideoTime", currTime);
				}
			}
			else
			{
				this.set("currentVideoTime", currTime);
			}
		}
	},
	
	/*
	 * Checks that the response to (un)subscription messages is normal.
	 * Otherwise, prints a warning.
	 * Returns whether the response is normal.
	 */
	checkSubscriptionResponse: function(response) {
		if (response.result != "OK") {
			Em.Logger.log("FFW.VideoPlayer: subscription response is not OK: " + response.result);
			return false;
		}
		return true;
	},
})
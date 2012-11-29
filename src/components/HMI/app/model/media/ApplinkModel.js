/**
 * @name MFT.SDModel
 * 
 * @desc SD Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/SDModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.ApplinkModel = Em.Object.create({
	
	active: false,

	applinkConnectionStatus: true,
	
	/** App Player*/
	init: function(){
		this._super();
	},
	
	/**
	  * Array of Interaction Choises
	  */
	interactionChoises:			new Array(),

	/**
	  * Array of commands in ApplinkOptionsView
	  */
    optionsCommands:			new Array(),

	/**
	  * Array of menus in ApplinkOptionsSubMenuView
	  */
    subMenuCommands:			new Array(),

    /**
	  * Array of commands for VR
	  */
    voiceRecognitionCommands:	new Array(),

    /**
	  * Array of active applications
	  */
    applicationsList:			new Array(),

	/**
	  * Timer for Media Clock
	  */
	timer:			null,

	countUp:		true,
	pause:			false,
	maxTimeValue:	68400, // 19 hours
	duration:		0,
	currTime:		0,

 	/**
	  * Array of Interaction Choises
	  */
	showInfo: Em.Object.create({
		field1:			'<field1>',
		field2:			'<field2>',
		field3:			'<field3>',
		mediaClock:		'<mediaClock>',
		statusBar:		'<statusBar',
		appName:		'<App name>',
		deviceName:		'<Device name>'
	}),
	

	onGetAppList: function( params ){
 		
		this.applicationsList.splice(0, this.applicationsList.length);
		for(var i = 0; i < params.appList.length; i++){
            this.applicationsList.push({
                type:       MFT.Button,
                params:     {
                    action:         'turnOnApplink',
                    target:         'MFT.MediaController',
                    text:           params.appList[i].appName,
                    className:      'scrollButtons button notpressed',
                    icon:           params.icon,
                    templateName:   'rightIcon'
                }                                   
            });
        }
        MFT.InfoAppsView.ShowAppList();

    },

    /**
	  * Calls when websocket connection is closed
      */
	onApplicationDisconected: function(){
		this.set('applinkConnectionStatus', false);
		this.interactionChoises 		= [];
		this.subMenuCommands 			= [];
		this.voiceRecognitionCommands	= [];
		this.applicationsList			= [];

	},

	startTimer: function(){
		if(!this.pause){
			this.timer = setInterval(function(){
				MFT.ApplinkModel.set('currTime', MFT.ApplinkModel.currTime+1);
			}, 1000);
		}else{
			clearInterval(this.timer);
		}
	}.observes('this.pause'),

	setDuration: function() {
		if(this.countUp){
			this.showInfo.set('mediaClock', Math.ceil((this.duration + this.currTime+1)/60)-1 + ":" + (this.duration + this.currTime) % 60 );
		}else{
			this.showInfo.set('mediaClock', Math.ceil((this.duration - this.currTime+1)/60)-1 + ":" + (this.duration - this.currTime) % 60 );
		}
	}.observes('this.currTime'),

	changeDuration: function() {
		clearInterval(this.timer);
		this.currTime = 0;
		this.startTimer();
	}.observes('this.duration')
});
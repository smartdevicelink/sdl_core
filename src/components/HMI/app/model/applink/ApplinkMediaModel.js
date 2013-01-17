/**
 * @name MFT.ApplinkMediaModel
 * 
 * @desc Applink data model
 * 
 * @category	Model
 * @filesource	app/model/media/ApplinkMediaModel.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.ApplinkMediaModel = MFT.ApplinkAppModel.create({
	
	active: false,

	applinkConnectionStatus: true,

	/**
	  * RangedValue for Slider
	  */
	applinkSliderContent:		MFT.RangedValue.create({range: 10, value:3, cycle: false, minValue: 0}),

	/**
	  * Array of commands in ApplinkOptionsView
	  */
    optionsCommands:			new Array(),

    /**
	  * Array of commands for VR
	  */
    voiceRecognitionCommands:	new Array(),

    /**
      * Set of params of Slider View
      */
    sliderParams: {
        headerLabel: "headerLabel",
        footerLabel: "footerLabel"
    },

	/**
	  * Timer for Media Clock
	  */
	timer:			null,

    newCommandId:   null, 

    newParams:  null,

    /** Current applink Sub Menu identificator*/
    currentApplinkSubMenuid: null,

    /** Current applink Perform Interaction Choise identificator*/
    currentApplinkPerformInteractionChoiseId: null,

    /**
      * Text for label on Perform Interaction screen
      */
    performInteractionInitialText: '',

	countUp:		true,
	pause:			false,
	maxTimeValue:	68400, // 19 hours
	duration:		0,
	currTime:		0,

 	/**
	  * Info data
	  */
	appInfo: Em.Object.create({
		field1:			'<field1>',
		field2:			'<field2>',
		field3:			'<field3>',
		mediaClock:		'<mediaClock>',
		statusBar:		'<statusBar',
		appName:		'<App name>',
		deviceName:		'<Device name>',
        customPresets:[
            '<no definition>',
            '<no definition>',
            '<no definition>',
            '<no definition>',
            '<no definition>',
            '<no definition>'
        ]
	}),

    /**
      * Info data
      */
    alertInfo: Em.Object.create({
        text1:          '<text1>',
        text2:          '<text2>',
        text3:          '<text3>',
        ttsChunks:      null,
        duration:       0,
        playTone:       null,
        softButtons:    null,
        tryAgainTime:   0
    }),

    /**
      *  Function that calls from VR to activate application
      */
    turnOnApplink: function(appName, appId){
        var params = {
            "appName":  appName,
            "appId":    appId
        };
        MFT.MediaController.turnOnApplink(params);
        MFT.MediaView.leftMenu.applinkButton.set('icon', 'sdfsdf');
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
				MFT.ApplinkMediaModel.set('currTime', MFT.ApplinkMediaModel.currTime+1);
			}, 1000);
		}else{
			clearInterval(this.timer);
		}
	}.observes('this.pause'),
	
	stopTimer: function() {
		clearInterval(this.timer);
		this.appInfo.set('mediaClock','');
	},

	setDuration: function() {
        var number;
		if(this.countUp){
			number = this.duration + this.currTime;
			//this.appInfo.set('mediaClock', Math.ceil((this.duration + this.currTime+1)/60)-1 + ":" + (number < 10 ? '0' : '') + number );
		}else{
			number = this.duration - this.currTime;
			//this.appInfo.set('mediaClock', Math.ceil((this.duration - this.currTime+1)/60)-1 + ":" + (number < 10 ? '0' : '') + number );
		}
		this.appInfo.set('mediaClock', Math.ceil((number+1)/60)-1 + ":" + ((number % 60) < 10 ? '0' : '') + (number % 60) );
	}.observes('this.currTime'),

	changeDuration: function() {
		clearInterval(this.timer);
		this.currTime = 0;
		this.startTimer();
	}.observes('this.duration'),

    /** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params, icon, appId ){

       MFT.ApplinkOptionsView.AddCommand( commandId, params, appId );

    },

    /** 
     * Delete command to Options list
     */
    onApplinkOptionsDeleteCommand: function(commandId){

        MFT.ApplinkOptionsView.DeleteCommand( commandId );

        var  count = this.subMenuCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(this.subMenuCommands[i].cmdId == commandId){
               this.subMenuCommands.splice(i, 1);
            }
        }
    },

    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){

        MFT.ApplinkOptionsView.AddSubMenu( menuId, params.menuName, params.position );

    },

    /** Delete subMenu button from Options list */
    onApplinkDeleteSubMenu: function( menuId ){

        if( MFT.ApplinkMediaController.currentApplinkSubMenuid == menuId ){
            MFT.States.back();
        }

        MFT.ApplinkOptionsView.DeleteSubMenu( menuId );

        return "SUCCESS";
    },

    /** Add command to VRPopUp list */
    onApplinkVRAddCommand: function( commandId, vrCommands ){
        MFT.VRPopUp.AddCommand( commandId, vrCommands );
    },

    /** Delete command from VRPopUp list */
    onApplinkVRDeleteCommand: function( commandId ){

        MFT.VRPopUp.DeleteCommand( commandId );

    },

    /** Applink AddCommand handler */
    onApplinkAddCommand: function(params){
        if( params.menuParams.parentID == 0 || params.menuParams.parentID == null ){
            this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams, params.cmdIcon, params.appId);
        }else{
            this.subMenuCommands.push(params);
            if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
                MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.ApplinkMediaController.currentApplinkSubMenuid);
            }

        }
    },

    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){
		if(params.updateMode == "CLEAR" ) {
			this.stopTimer();
			return;
		}
        if(params.updateMode == "COUNTUP"){
            this.set('countUp', true);
        }else if(params.updateMode == "COUNTDOWN"){
            this.set('countUp', false);
        }

        if(params.updateMode == "PAUSE"){
            this.set('pause', true);
        }else if(params.updateMode == "RESUME"){
            this.set('pause', false);
        }else{
            this.set('duration', 0);
            this.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
            this.set('pause', false);
        }
        
    },

    /** Applin UI Show handler */
    onApplinkUIShow: function(params){
        clearInterval(this.timer);
        this.appInfo.set('field1',        params.mainField1);
        this.appInfo.set('field2',        params.mainField2);
        this.appInfo.set('field3',        params.mainField3);
        this.appInfo.set('field4',        params.mainField4);
        this.appInfo.set('alignment',     params.alignment);
        this.appInfo.set('statusBar',     params.statusBar);
        this.appInfo.set('mediaClock',    params.mediaClock);
        this.appInfo.set('mediaTrack',    params.mediaTrack);
        this.appInfo.set('image',         params.graphic);
        this.appInfo.set('softButtons',   params.softButtons);
        if(params.customPresets){
            var i=0;
            for(i=0; i<params.customPresets.length; i++){
                if(params.customPresets[i] != '' || params.customPresets[i] != null){
                    this.appInfo.set('customPresets.' + i, params.customPresets[i]);
                }
            }
        }

        MFT.applinkView.innerMenu.content.AddSoftButton(params.softButtons);
    },

});
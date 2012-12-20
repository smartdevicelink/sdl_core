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
      * Array of connected devices
      */
    devicesList:                new Array(),

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
    },

	onGetAppList: function( appList ){

		this.applicationsList.splice(0, this.applicationsList.length);
		for(var i = 0; i < appList.length; i++){
            if( appList[i].isMediaApplication ){    
                this.applicationsList.push({
                    type:       MFT.Button,
                    params:     {
                        action:         'turnOnApplink',
                        target:         'MFT.ApplinkMediaController',
                        text:           appList[i].appName,
                        appName:        appList[i].appName,
                        appId:          appList[i].appId,
                        className:      'scrollButtons button notpressed',
                        icon:           appList[i].icon,
                        templateName:   'rightIcon'
                    }                                   
                });
            }else{
                this.applicationsList.push({
                    type:       MFT.Button,
                    params:     {
                        action:         'turnOnApplinkNonMedia',
                        target:         'MFT.MediaController',
                        text:           appList[i].appName,
                        appName:        appList[i].appName,
                        appId:          appList[i].appId,
                        className:      'scrollButtons button notpressed',
                        icon:           appList[i].icon,
                        templateName:   'rightIcon'
                    }                                   
                });
            }
        }
        MFT.InfoAppsView.ShowAppList();

    },

    onGetDeviceList: function( params ){
        if (null == params.resultCode || (null != params.resultCode && "SUCCESS" == params.resultCode)) {
            this.devicesList.splice(0, this.devicesList.length);
            for(var i = 0; i < params.deviceList.length; i++){
                this.devicesList.push({
                    type:       MFT.Button,
                    params:     {
                        action:         'onDeviceChoosed',
                        target:         'MFT.ApplinkController',
                        text:           params.deviceList[i],
                        deviceName:     params.deviceList[i],
                        className:      'scrollButtons button notpressed',
                        icon:           params.icon,
                        templateName:   'rightIcon'
                    }                                   
                });
            }
            
            if( MFT.States.info.devicelist.active ){
                MFT.DeviceLilstView.ShowDeviceList();
            }
        }
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

	setDuration: function() {
        var number = (this.duration + this.currTime) % 60;
		if(this.countUp){
			this.appInfo.set('mediaClock', Math.ceil((this.duration + this.currTime+1)/60)-1 + ":" + (number < 10 ? '0' : '') + number );
		}else{
			this.appInfo.set('mediaClock', Math.ceil((this.duration - this.currTime+1)/60)-1 + ":" + (number < 10 ? '0' : '') + number );
		}
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

    /** Delete command to Options list */
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

    /** Delete all commands in sub menu from VR */
    onApplinkCreateInteractionChoise: function(params){

        this.interactionChoises.push(params);

        MFT.VRPopUp.CreateInteractionChoise(params);

    },

    /** Delete all commands in sub menu from VR */
    onApplinkDeleteInteractionChoise: function(choiseSetID){

        if( (choiseSetID == this.currentApplinkPerformInteractionChoiseId) && MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.States.back();
        }

        for(var val in this.interactionChoises){
            if(this.interactionChoises[val].interactionChoiceSetID == choiseSetID){
                this.interactionChoises.splice(val, 1);
                break;
            }
        }

        MFT.VRPopUp.DeleteInteractionChoise( choiseSetID );

    },

    /** Applink AddCommand handler */
    onApplinkAddCommand: function(params){
        if( params.menuParams.parentID == 0 ){
            this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams, params.cmdIcon, params.appId);
        }else{
            this.subMenuCommands.push(params);
            if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
                MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
            }

        }

        // appId
    },

    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){

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

    /** Switching on Applink Perform Interaction Choise */
    turnOnApplinkPerform: function(params){
        this.set('performInteractionInitialText', params.initialText);

/*
    TTSChunk initialPrompt[1:100], //data type from AppLink protocol specification
    InteractionMode interactionMode, // data type from AppLink protocol specification
    unsigned int (2000000000)  interactionChoiceSetIDList[1:100],// List of interaction choice set IDs to use with an interaction
    TTSChunk * helpPrompt[1:100], //data type from AppLink protocol specification
    TTSChunk * timeoutPrompt[1:100], // data type from AppLink protocol specification
    unsigned int (5000:100000) * timeout, //in milliseconds
    VrHelpItem * vrHelp[1:100], // Ability to send suggested VR Help Items to display on-screen during Perform Interaction
    int appId
    -> 
    unsigned int (2000000000) * choiceID, // ID of the choice that was selected in response to PerformInteraction
    TriggerSource * triggerSource //data type from AppLink protocol specification

*/



        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.AppPerformInteractionChoise.PerformInteraction(params.interactionChoiceSetIDList);
        }else{
            this.set('currentApplinkPerformInteractionChoiseId', params.interactionChoiceSetIDList);
            MFT.States.goToState('media.applink.applinkperforminteractionchoise');
        }

        this.onApplinkTTSSpeak(params.initialPrompt);

        setTimeout(function(){
            MFT.ApplinkMediaModel.onApplinkTTSSpeak(params.timeoutPrompt);
        }, params.timeout);

    },

    /** Create list of lapplications on info view */
    /*onShowAppList: function(params){
        MFT.InfoAppsView.ShowAppList(params);
    },*/

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
        this.appInfo.set('customPresets', params.customPresets);

        // appId
    },

    /** Applink Slider activation */
    onApplinkSlider: function(params){

        this.applinkSliderContent.set('range', params.numTicks);
        this.applinkSliderContent.set('value', params.position);
        this.set('sliderParams.headerLabel', params.sliderHeader);
        this.set('sliderParams.footerLabel', params.sliderFooter);
        MFT.ApplinkMediaController.turnOnApplinkSlider();
        //MFT.ApplinkSliderView.activate();
        setTimeout(function(){
            if(MFT.States.media.applink.applinkslider.active){
                MFT.States.back();
            }
        },
        params.timeout);
    },

    /** Applink TTS Speak handler */
    onApplinkTTSSpeak: function(ttsChunks){
        var message = '';
        if(ttsChunks){
            for(var i = 0; i < ttsChunks.length; i++){
                message += ttsChunks[i].text + '\n';
            }
            MFT.TTSPopUp.ActivateTTS(message);
        }
    },

    /** Applink UI Alert handler */
    onApplinkUIAlert: function(params){

        this.alertInfo.set('text1', params.AlertText1);
        this.alertInfo.set('text2', params.AlertText2);
        this.alertInfo.set('text3', params.alertText3);
        this.alertInfo.set('ttsChunks', params.ttsChunks );
        this.alertInfo.set('duration', params.duration );
        this.alertInfo.set('playTone', params.playTone );
        this.alertInfo.set('softButtons', params.softButtons );
        this.alertInfo.set('tryAgainTime', params.tryAgainTime );

        // appId

        MFT.AlertPopUp.AlertActive();

        this.onApplinkTTSSpeak(params.ttsChunks);

    }
});
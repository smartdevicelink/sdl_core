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

MFT.ApplinkMediaModel = Em.Object.create({
	
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
				MFT.ApplinkMediaModel.set('currTime', MFT.ApplinkMediaModel.currTime+1);
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
	}.observes('this.duration'),

    /** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params ){

       MFT.ApplinkOptionsView.AddCommand( commandId, params );

    },

    /** Delete command to Options list */
    onApplinkOptionsDeleteCommand: function(commandId){

        MFT.ApplinkOptionsView.DeleteCommand( commandId );

        var  count = MFT.ApplinkMediaModel.subMenuCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkMediaModel.subMenuCommands[i].cmdId == commandId){
               MFT.ApplinkMediaModel.subMenuCommands.splice(i, 1);
            }
        }
    },

    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){

        MFT.ApplinkOptionsView.AddSubMenu( menuId, params );

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

        MFT.ApplinkMediaModel.interactionChoises.push(params);

        MFT.VRPopUp.CreateInteractionChoise(params);

    },

    /** Delete all commands in sub menu from VR */
    onApplinkDeleteInteractionChoise: function(choiseSetID){

        if( (choiseSetID == this.currentApplinkPerformInteractionChoiseId) && MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.States.back();
        }

        for(var val in MFT.ApplinkMediaModel.interactionChoises){
            if(MFT.ApplinkMediaModel.interactionChoises[val].interactionChoiceSetID == choiseSetID){
                MFT.ApplinkMediaModel.interactionChoises.splice(val, 1);
                break;
            }
        }

        MFT.VRPopUp.DeleteInteractionChoise( choiseSetID );

    },

    /** Applink AddCommand handler */
    onApplinkAddCommand: function(params){
        if( params.menuParams.parentID == 0 ){
            this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams);
        }else{
            MFT.ApplinkMediaModel.subMenuCommands.push(params);
            if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
                MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
            }

        }
    },

    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){

        if(params.updateMode == "COUNTUP"){
            MFT.ApplinkMediaModel.set('countUp', true);
        }else if(params.updateMode == "COUNTDOWN"){
            MFT.ApplinkMediaModel.set('countUp', false);
        }

        if(params.updateMode == "PAUSE"){
            MFT.ApplinkMediaModel.set('pause', true);
        }else if(params.updateMode == "RESUME"){
            MFT.ApplinkMediaModel.set('pause', false);
        }else{
            MFT.ApplinkMediaModel.set('duration', 0);
            MFT.ApplinkMediaModel.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
            MFT.ApplinkMediaModel.set('pause', false);
        }
        
    },

    /** Switching on Applink Perform Interaction Choise */
    turnOnApplinkPerform: function(params){
        this.set('performInteractionInitialText', params.initialText);
        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.AppPerformInteractionChoise.PerformInteraction(params.interactionChoiceSetIDList);
        }else{
            this.set('currentApplinkPerformInteractionChoiseId', params.interactionChoiceSetIDList);
            MFT.States.goToState('media.applink.applinkperforminteractionchoise');
        }
    },

    /** Create list of lapplications on info view */
    /*onShowAppList: function(params){
        MFT.InfoAppsView.ShowAppList(params);
    },*/

    /** Applin UI Show handler */
    onApplinkUIShow: function(params){
        MFT.ApplinkMediaModel.showInfo.set('field1', params.mainField1);
        MFT.ApplinkMediaModel.showInfo.set('field2', params.mainField2);
        MFT.ApplinkMediaModel.showInfo.set('mediaClock', params.mediaClock);
        MFT.ApplinkMediaModel.showInfo.set('field3', params.mediaTrack);
        MFT.ApplinkMediaModel.showInfo.set('statusBar', params.statusBar);
    },

    /** Applink Slider activation */
    onApplinkSlider: function(params){
        this.turnOnApplinkSlider();
    },

    /** Applink TTS Speak handler */
    onApplinkTTSSpeak: function(params){
        var message = '';
        for(var i = 0; i < params.ttsChunks.length; i++){
            message += params.ttsChunks[i].text + '\n';
        }
        MFT.TTSPopUp.ActivateTTS(message);
    },

    /** Applin UI Alert handler */
    onApplinkUIAlert: function(params){
        MFT.AlertPopUp.AlertActive(params.AlertText1, params.AlertText2, params.duration, params.playTone);
    }
});
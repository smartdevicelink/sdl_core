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

MFT.ApplinkMediaModel = MFT.ApplinkAppModel.extend({
    
    init: function() {
	   
	   this._super();
	   
	   // init properties here
	   this.set('appInfo', Em.Object.create({
            field1:			'<field1>',
            field2:			'<field2>',
            field3:			'<field3>',
            mediaClock:		'<mediaClock>',
            statusBar:		'<statusBar',
            appName:		'<App name>',
            customPresets:[
                '<no definition>',
                '<no definition>',
                '<no definition>',
                '<no definition>',
                '<no definition>',
                '<no definition>'
                ]
            })
        );
	},
    
	active: false,

	applinkConnectionStatus: true,

	/**
	 * Array of commands in ApplinkOptionsView
	 */
    optionsCommands:			new Array(),

    /**
	 * Array of commands for VR
	 */
    voiceRecognitionCommands:	new Array(),

	/**
	 * Timer for Media Clock
	 */
	timer:			null,

    /** Current applink Sub Menu identificator*/
    currentApplinkSubMenuid: null,

    /** Current applink Perform Interaction Choise identificator*/
    currentApplinkPerformInteractionChoiseId: null,

	countUp:		true,
	pause:			false,
	maxTimeValue:	68400, // 19 hours
	duration:		0,
	currTime:		0,

    /**
	 * Activate current application model
	 */
    turnOnApplink: function(){
        MFT.ApplinkMediaController.turnOnApplink( this );
    },

	startTimer: function(){
	
	   var self = this;
	   
		if(!this.pause){
			this.timer = setInterval(function(){
				self.set('currTime', self.currTime+1);
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
        var number,
            str;
		if(this.countUp){
			number = this.duration + this.currTime;
			//this.appInfo.set('mediaClock', Math.ceil((this.duration + this.currTime+1)/60)-1 + ":" + (number < 10 ? '0' : '') + number );
		}else{
			number = this.duration - this.currTime;
			//this.appInfo.set('mediaClock', Math.ceil((this.duration - this.currTime+1)/60)-1 + ":" + (number < 10 ? '0' : '') + number );
		}
        str = (Math.ceil(number/3600) -1) < 10 ? '0' : '';
        str += (Math.ceil(number/3600) -1) + ':';
        str += Math.ceil( (number+1)/60%60)-1 + ":" + ((number % 60) < 10 ? '0' : '') + (number % 60);
		this.appInfo.set('mediaClock', str );
	}.observes('this.currTime'),

	changeDuration: function() {
		clearInterval(this.timer);
		this.currTime = 0;
		this.startTimer();
	}.observes('this.duration'),

    /** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params, icon, appId ){

       MFT.ApplinkOptionsView.AddCommand( commandId, params, icon, appId );

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
        if( params.menuParams.parentID == 0 || !params.menuParams.parentID ){
            this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams, params.cmdIcon ? params.cmdIcon.value : null , params.appId);
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
    }
});
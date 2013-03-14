/**
 * @name MFT.SDLMediaModel
 * 
 * @desc SDL data model
 * 
 * @category	Model
 * @filesource	app/model/media/SDLMediaModel.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.SDLMediaModel = MFT.SDLAppModel.extend({

    init: function() {

	   this._super();

	   // init properties here
	   this.set('appInfo', Em.Object.create({
            field1:			'<field1>',
            field2:			'<field2>',
            field3:			'<field3>',
            mediaClock:		'<mediaClock>',
            trackIcon:      'images/sdl/audio_icon.jpg',
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

        this.set('isPlaying', true);

        this.set('commandsList',[]);
        this.set('softButtons',[]);
	},

    /**
     * Flag for media playing state
     * @param {Bool}
     */
    isPlaying:  false,

    /**
     * Flag for model active state
     * currently used for status bar
     * @param {Bool}
     */
	active: false,

	/**
	 * Timer for Media Clock
	 */
	timer:			null,

    /**
     * Current sdl Sub Menu identificator
     */
    currentSDLSubMenuid: null,

    /**
     * Current sdl Perform Interaction Choise identificator
     */
    currentSDLPerformInteractionChoiseId: null,

	countUp:		true,
	pause:			false,
	maxTimeValue:	68400, // 19 hours
	duration:		0,
	currTime:		0,

    /**
     * Notification of deactivation of current application model
     * initiated in StateManager
     */
    deactivateApp: function(){

        MFT.SDLModel.onDeactivateApp( MFT.TransitionIterator.finalPath, this.appId, this.appName );

    },

    /**
     * Method hides sdl activation button
     * @param {Number}
     */
    onDeleteApplication: function( appId ){
        if( MFT.SDLMediaController.currentAppId == appId ){

            MFT.SDLMediaController.set('currentAppId', 0);

            if( MFT.sdlView.stateObj.active || MFT.SDLAppController.model.active ){

                MFT.States.goToState('info.apps');

                MFT.SDLAppController.model.set('active', false);

                MFT.MediaController.set('activeState', 'media.avin');
            }
        }
    },

    /**
	 * Activate current application model
	 */
    turnOnSDL: function(){
        MFT.SDLMediaController.activateApp( this );
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
            str = '';
		if(this.countUp){
			number = this.duration + this.currTime;
		}else{
			number = this.duration - this.currTime;
		}
        str =  (parseInt(number/3600) < 10 ? '0' : '') + parseInt(number/3600) + ':'; //hours
        str += (parseInt(number/60)%60 < 10 ? '0' : '') + parseInt(number/60)%60 +  ":"; //minutes
        str += ((number % 60) < 10 ? '0' : '') + (number % 60); // seconds
		this.appInfo.set('mediaClock', str );

        if( !this.get('countUp') && this.duration == this.currTime ){
            clearInterval(this.timer);
            return;
        }

	}.observes('this.currTime'),

	changeDuration: function() {
		clearInterval(this.timer);
		this.currTime = 0;
		this.startTimer();
	}.observes('this.duration'),

    /**
     * SDL Setter for Media Clock Timer
     * @param {Object}
     */
    sdlSetMediaClockTimer: function(params){

        if( params.updateMode == "PAUSE" && this.pause){
            return 'IGNORED';
        }

        if( params.updateMode == "RESUME" && !this.pause){
            return 'IGNORED';
        }

        if(params.updateMode == "CLEAR" ) {
			this.stopTimer();
            return 'SUCCESS';
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
            if( params.startTime ){
                this.set('duration', 0);
                this.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
            }
            this.set('pause', false);
        }

        return 'SUCCESS';
    },

    /**
     * Method to clear App OverLay
     */
    clearAppOverLay: function(){

        clearInterval(this.timer);
        this.appInfo.set('field1',        '');
        this.appInfo.set('field2',        '');
        this.appInfo.set('field3',        '');
        this.appInfo.set('field4',        '');
        this.appInfo.set('alignment',     '');
        this.set('statusText',            '');
        this.appInfo.set('mediaClock',    '');
        this.appInfo.set('mediaTrack',    '');
        this.appInfo.set('trackIcon', 'images/sdl/audio_icon.jpg');
        this.updateSoftButtons();
        for(i=0; i< 6; i++){
            this.appInfo.set('customPresets.' + i, '');
        }
        MFT.SDLController.set('protocolVersion2State', false);

    },

    /**
     * Applin UI Show handler
     * @param {Object}
     */
    onSDLUIShow: function(params){
        clearInterval(this.timer);
        this.appInfo.set('field1',        params.mainField1);
        this.appInfo.set('field2',        params.mainField2);
        this.appInfo.set('field3',        params.mainField3);
        this.appInfo.set('field4',        params.mainField4);
        this.appInfo.set('alignment',     params.alignment);
        this.set('statusText',            params.statusBar);
        this.appInfo.set('mediaClock',    params.mediaClock);
        this.appInfo.set('mediaTrack',    params.mediaTrack);
        if( params.graphic ){
            this.appInfo.set('trackIcon', params.graphic);
        }else{
            this.appInfo.set('trackIcon', 'images/sdl/audio_icon.jpg');
        }

        this.updateSoftButtons( params.softButtons );

        if( params.customPresets ){
            var i=0;
            for(i=0; i< 6; i++){
                if(params.customPresets[i] != '' || params.customPresets[i] != null){
                    this.appInfo.set('customPresets.' + i, params.customPresets[i]);
                }else{
                    this.appInfo.set('customPresets.' + i, '');
                }
            }
            MFT.SDLController.set('protocolVersion2State', true);
        }else{
            MFT.SDLController.set('protocolVersion2State', false);
        }
    }
});
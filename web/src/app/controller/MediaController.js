/**
 * @name MFT.MediaController
 *
 * @desc Media module logic
 *
 * @category	Controller
 * @filesource	app/controller/MediaController.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.MediaController = Em.Object.create({
	
	/** Initial substate */
	activeState: 'media.radio.am',
	
	/** Visibility of Home Media Status */
	isHomeMediaStatusHidden: false,
	isHomeMediaStatusInfoHidden: false,
	isHomeStateActive: false,
	
	/* Show HomeMediastatus Preset */
	onMeidaPresets: function(){
		if (this.media_status_preset_messageTimerID === -1 ){
			this.set('isHomeMediaStatusHidden',false);
            this.set('isHomeMediaStatusInfoHidden',false);
		}			
	}.observes('media_status_preset_messageTimerID', 'currentModuleData.selectedIndex'),
	
	/** Update MediaHomeStatatus */
	updateMediaHomeStatus: function(){
		// if "Save Presets" is runing hide HomeMediastatus
		this.set('isHomeMediaStatusHidden', !(this.media_status_preset_messageTimerID === -1) );
	},
	/** Update HomeMediaStatusInfo */
	updateHomeMediaStatusInfo: function(){
		// if "Save Presets" is runing hide Home Media Info
		this.set('isHomeMediaStatusInfoHidden', !(this.media_status_preset_messageTimerID === -1) );
	},
	
	/** Visibility of Home Media Radio Status if preset have been activated */
	isPresetActivated: false,
	
	/** text labels (dinamicaly could be changed)*/
	amLabelBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_am1'),
	
	fmLabelBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_fm'),
	
	siriusLabelBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_sirius'),
	
	/** ****************************** DIRECT TUNE VARIABLES ********************************** */
	
	/** Selected direct tune station */
	directTune: [],
	/** Selected direct tune station for sirius*/
	siriusDirectTune: [],
	
	/** Hide direct tune component */
	directTuneHide: true,
	
	/** True when find station */
	directTuneFinished: false,
	
	/** Direct tune timer */
	directTuneTimer: null,
	
	/** Stations for direct tune */
	directTuneStations: [],
	
	directKeypressed: false,
	
	directTuneSelected: false,
	
	/** Current Direct tune data*/
	currentDirectTuneData: MFT.AmModel.directTunestations,
	
	/** ****************************** END DIRECT TUNE VARIABLES ********************************** */
	
	/** Detects top or bottom Left Menu items*/
	isTopListMenu: true,
		
	/** Current selected radios module Data reference*/
	currentModuleData: MFT.AmModel.am1,
	
	/** Current selected players module Data reference*/
	currentPlpayerModuleData: MFT.SDModel,
	
	/** Current selected player object  reference*/
	currentSelectedPlayer: null,

	/**  On radio module exit event */
	onRadioExit: function(data){
		data.band.set('activeBand',data.band.value);
		data.band.set('value',-1);
	},
	
	/**  On radio module enter event */
	onRadioEnter: function(data,state){
		/** Set Radio state */
		if(state){
			MFT.States.goToState('media.radio.'+state);
			/** Hide direct tune */
			this.offDirectTune();
			if(this.directTuneSelected){
				this.set('directTuneSelected', false);
			}
		}
		/** load  direct tune  data*/
		this.set('currentDirectTuneData', data.directTunestations);
		/** Set Active band*/
		data.band.set('value',data.band.activeBand);
	},
	
	/**  On player module enter event */	
	onPlayerEnter: function(data, state){
		MFT.States.goToState('media.player.'+state);
		// Hide direct tune
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
		this.set('currentPlpayerModuleData',data.PlayList);
		this.set('currentSelectedPlayer',data.player);
	},
	
	/**
	 * Determines activity of any player state except bluetooth
	 *
	 * @property activeState
	 * @return {Boolean}
	 */
	isPlayerActive: function(){
			var isPlayer = this.activeState.indexOf('cd') >= 0 ||
						   this.activeState.indexOf('usb') >= 0 ||
						   this.activeState.indexOf('sd') >= 0;
			return ( isPlayer );
	}.property('this.activeState'),
	
	/****************** AM *******************/
	
	/** toggle between AM stations */
	toggleAmStation: function(){
		if(MFT.States.media.radio.am.active){
			MFT.AmModel.band.toggleIndicators();
		}else{
			this.onRadioEnter(MFT.AmModel,'am');
		}
	},
	
	/**
	 * Observes for model value 
	 * changed - switch selected 
	 * station view
	 *
	 * @observes AmModel.band
	 */
	switchAmSubstate: function(){
		switch(MFT.AmModel.band.value){
			case 0:{
				this.set('currentModuleData', MFT.AmModel.am1); 
				this.set('amLabel', MFT.locale.label.view_media_am1);
				break;
			}
			case 1:{
				this.set('currentModuleData', MFT.AmModel.amAst);
				this.set('amLabel',MFT.locale.label.view_media_amAst);
				break;
			}
			default:
				this.set('amLabel', MFT.locale.label.view_media_am);
				break;			
		}
	}.observes('MFT.AmModel.band.value'),
	
	/****************** FM *******************/
	
	/** Toggle between FM stations */
	toggleFmStation: function(){
		if(MFT.States.media.radio.fm.active){
			MFT.FmModel.band.toggleIndicators();
		}else{
			this.onRadioEnter(MFT.FmModel,'fm');
		}
	},

    STORAGE_LOCATIONS_COUNT: 100,

    storageLocationsRemain: 100,

    isFullMemory: false,

    /**
     * Fm Options Tag Store Display On/Off
     *
     * 0 - On
     * 1 - Off
     */
    tagStoreOnOff: 1,

    isTagStoredDisabled: function() {
        return !!(this.get('currentActiveData').isHd && this.tagStoreOnOff == 0);
    }.property('this.currentActiveData.isHd','this.tagStoreOnOff'),

    /** Tagged tracks object **/
    taggedTracks: {},

    tagStore: function(){
        var curStation = this.get('currentActiveData');

        if (this.taggedTracks[curStation.uid]) {
            this.showTagStorePopup(2000, MFT.locale.label.view_media_fm_tag_alreadyStored);
        } else if(this.isFullMemory){
            this.showTagStorePopup();
        } else {
            /* Checking for remaining locations */
            if(this.storageLocationsRemain === 0){
                /* */
                this.set('isFullMemory', true);
                this.showTagStorePopup();
            } else{
                /* Reduces storage locations count */
                this.storageLocationsRemain--;
                this.taggedTracks[curStation.uid] = true;
                this.showTagStoreAddMessagesPopup();
            }
        }
    },

    deleteAllTags: function(){
        this.hideTagStorePopup();
        this.storageLocationsRemain = this.STORAGE_LOCATIONS_COUNT;
        this.taggedTracks = {};
        this.set('isFullMemory', false);
        this.showTagStorePopup(2000, MFT.locale.label.view_media_fm_tags_tagsDeleted + ' ' + this.storageLocationsRemain + ' ' + MFT.locale.label.view_media_fm_tags_remaining);
    },

    /**
     * Show tag store add messages popup
     */
    showTagStoreAddMessagesPopup: function () {
        var self = this;
        /* Show message */
        this.showTagStorePopup(5000, MFT.locale.label.view_media_fm_tag_tagging);
        setTimeout(function() {
            $('#tagStorePopup .popup-window').animate({opacity: 0},500, function() {
                self.set('popUpMessage', MFT.locale.label.view_media_fm_tag_stored + ' ' + self.storageLocationsRemain + ' ' + MFT.locale.label.view_media_fm_tags_remaining);
            });
            $('#tagStorePopup .popup-window').animate({opacity: 1},500);
        }, 2000);

    },

    /**
     * Show popup and close it after closeTime, if it exists
     * @param {number} closeTime time, after which the popup will close
     * @param {string} message, displaying message popup
     */
    showTagStorePopup: function(closeTime, message){
        var self = this;

        if (typeof message != 'undefined') {
            this.set('popUpMessage', message);
        }

        $('#tagStorePopup').show().animate({
            opacity: 1
        },500);
        if(!isNaN(closeTime)){
            setTimeout(self.hideTagStorePopup, closeTime);
        }
    },

    hideTagStorePopup: function(){
        $('#tagStorePopup').css({
            'opacity': 0,
            'display': 'none'
        });
    },

	/** 
	 * Observes for model value
	 * changed - switch selected 
	 * station view
	 *
	 * @observes FmModel.band
	 */
	switchFmSubstate: function(){
		
		switch(MFT.FmModel.band.value){
			case 0:{
				this.set('currentModuleData', MFT.FmModel.fm1); 
				this.set('fmLabel', MFT.locale.label.view_media_fm1);
				break;
			}
			case 1:{
				this.set('currentModuleData', MFT.FmModel.fm2); 
				this.set('fmLabel', MFT.locale.label.view_media_fm2);
				break;
			}
			case 2:{
				this.set('currentModuleData', MFT.FmModel.fmAst); 
				this.set('fmLabel', MFT.locale.label.view_media_fmAst);
				break;
			}				
			default:
				this.set('fmLabel','FM');
				break;
		}
	}.observes('MFT.FmModel.band.value'),
	
	/****************** SIRIUS *******************/
	
	/** Toggle between SIRIUS stations */
	toggleSiriusStation: function(){
		if(MFT.States.media.radio.sirius.active){
			MFT.SiriusModel.band.toggleIndicators();
		}else{
			this.onRadioEnter(MFT.SiriusModel, 'sirius');
		}	
	},
	
	/**
	 * Observes for model value
	 * changed - switch selected 
	 * station view
	 *
	 * @observes SiriusModel.band
	 */
	switchSiriusSubstate: function(){
		
		switch(MFT.SiriusModel.band.value){
			case 0:{
				this.set('currentModuleData', MFT.SiriusModel.sir1); 
				this.set('siriusLabel','SIRIUS 1');
				break;
			}
			case 1:{
				this.set('currentModuleData', MFT.SiriusModel.sir2); 
				this.set('siriusLabel','SIRIUS 2');
				break;
			}
			case 2:{
				this.set('currentModuleData', MFT.SiriusModel.sir3); 
				this.set('siriusLabel','SIRIUS 3');
				break;
			}
			default: {
				this.set('siriusLabel','SIRIUS');
				break;
			}
		}
	}.observes('MFT.SiriusModel.band.value'),
	
	/**
	 * Check for logo in station data
	 *
	 * @observes activeState, selectedItem, directTuneSelected
	 */
	onStationLogoChange: function(){
		if(this.directTuneSelected && this.currentDirectTuneData.get('selectedDirectItem') ){
			this.set('isStationLogo', this.currentDirectTuneData.get('selectedDirectItem').logo.length > 1);
		}else{
			this.set('isStationLogo', this.currentModuleData.get('selectedItem').logo.length > 1);
		}
	}.observes( 'this.activeState',
				'this.currentModuleData.selectedItem', 
				'this.directTuneSelected'
			),
			
	/**
	 * On Preset Direct data Change
	 *
	 * @property selectedItem, selectedDirectItem, directTuneSelected, selectedDirectItem
	 */
	currentActiveData: function(){
		if(this.directTuneSelected){
			return this.currentDirectTuneData.get('selectedDirectItem');
		}else{
			return this.currentModuleData.get('selectedItem');
		}
	}.property(
		'MFT.MediaController.currentModuleData.selectedItem',
		'MFT.MediaController.currentDirectTuneData.selectedDirectItem',
		'MFT.MediaController.directTuneSelected',
		'MFT.SiriusModel.directTunestations.selectedDirectItem'
	),		
	
	/****************** CD *******************/
	/** Turn on CD */
	turnOnCD: function(){
		this.onPlayerEnter(MFT.CDModel,'cd');
	},
	
	/****************** USB *******************/
	/** Turn on USB*/
	turnOnUSB: function(){
		this.onPlayerEnter(MFT.USBModel, 'usb');
	},
	
	/****************** SD *******************/
	/** Turn on SD */
	turnOnSD: function(){
		this.onPlayerEnter(MFT.SDModel, 'sd');
	},
	
	/** Turn on More Info */
	turnMoreInfo: function(){
		if (MFT.helpMode) {
			MFT.VideoPlayerController.start('ent_More_info');
			return;
		}
		MFT.States.goToState(MFT.States.currentState.get('path')+'.moreinfo');
	},
	
	/** Turn on  Browse */
	turnOnBrowse: function(){
		if (MFT.helpMode) {
			MFT.VideoPlayerController.start('ent_Browse');
			return;
		}
		MFT.States.goToState(MFT.States.currentState.get('path')+'.browse');
		this.resetDirectTune();
	},
	
	/** Turn on Options */
	turnOnOptions: function(){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.options');
	},
	
	/** Turn on Browse All*/
	turnBrowseAll: function(){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.browseall');
	},

    /** Bluetooth slots object **/
    slotsBT: {},
    isAllSlotsBTEmpty: true,

    /** Set Bluetooth active slots title for left menu **/
    setActiveSlotBT: function(){
        var activeSlot = this.slotsBT[MFT.BTModel.range.value];
        var deviceList = MFT.BTModel.deviceList;

        if (activeSlot) {
            this.set('activeSlotBT', activeSlot);
            this.set('activeSlotBTTitle', activeSlot.name);

            for (var key in deviceList) {
                deviceList[key].set('isConnected', !!(deviceList[key].uid == activeSlot.uid));
            }
        }
    },

    /** Active Bluetooth slots device data **/
    activeSlotBT: null,

    /** Active Bluetooth slots title for left menu **/
    activeSlotBTTitle: MFT.locale.label.view_media_bt_title,

    /** Toggle between BT pages */
    toggleBT: function(){
        if(MFT.helpMode){
            MFT.VideoPlayerController.start('ent_BT_audio', 'media.player.bluetooth');
            return;
        }

        if(MFT.States.media.player.bluetooth.active){
            if (!this.isAllSlotsBTEmpty && !this.isSlotBTEmpty()) {
                MFT.BTModel.range.toggleIndicators();
                this.setActiveSlotBT();
            }
        }else{
            MFT.States.goToState('media.player.bluetooth');
        }

        this.showDeviceListBTPopup();
    },

    isSlotBTEmpty: function () {
        var slot = MFT.BTModel.range.value;
        if (slot < 2) {
            return !this.slotsBT[slot+1];
        } else {
            return !this.slotsBT[0];
        }

    },

    /**
     * Show popup and close it after closeTime, if it exists
     * @param {number} closeTime time, after which the popup will close
     */
    showDeviceListBTPopup: function(closeTime){
        var self = this;
        $('#deviceListBTPopup').show().animate({
            opacity: 1
        },500);
        if(!isNaN(closeTime)){
            setTimeout(self.hideDeviceListBTPopup, closeTime);
        }
    },

    hideDeviceListBTPopup: function(){
        $('#deviceListBTPopup').css({
            'opacity': 0,
            'display': 'none'
        });
    },

    /** Connected Bluetooth device and save to slot **/
    connectBTDeviceTimer: '',
    connectBTDevice: function(data) {
        var self = this;
        this.hideDeviceListBTPopup();
        this.set('popUpMessage', MFT.locale.label.view_media_bt_connectingTo+" "+data.content.name+" ...");
        this.showMessage('connectBTPopup', 3000);
        this.connectBTDeviceTimer = setTimeout(function() {
            if (!self.isAllSlotsBTEmpty && self.isSlotBTEmpty()) {
                MFT.BTModel.range.toggleIndicators();
            }
            self.slotsBT[MFT.BTModel.range.value] = data.content;
            self.set('isAllSlotsBTEmpty', false);
            self.setActiveSlotBT();
        }, 3000);
    },

    cancelConnectBTDevice: function() {
        clearTimeout(this.connectBTDeviceTimer);
        this.hideMessage('connectBTPopup', 3000);
    },

	/** Turn on BT on vehicle change */
	turnOnBtOnVihicleChange: function(){
		if(MFT.States){
			if(MFT.States.media.active && MFT.BTModel.active && MFT.helpMode){
				MFT.States.goToState( 'media.bluetooth');
			}
		}
	}.observes('FFW.Backend.vehicleModel'),
	
	/** Turn on SD */
	turnOnAVin: function(){
		MFT.States.goToState('media.avin');
		// hide directTune
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
	},
	
	/** Scroll left menu up */
	listDown: function(){
		this.set('isTopListMenu',false);
	},
	
	/** Scroll left menu down */
	listUp: function(){
		this.set('isTopListMenu',true);
	},
	
	/** Determin Station HD State */
	isHDActive: function(){
        return MFT.States.media.radio.fm.active;
	}.property('MFT.States.media.radio.fm.active'),
	
	
	/** TurnOn Sound Settings */
	turnOnSoundSettings: function(){
		MFT.States.goToState('settings.sound');
	},
	
	/** Go to Sirius PTYCategory view*/
	setPTYforSeekSirius: function(){
		MFT.States.goToState('media.radio.sirius.options.PTYCategory');
	},
	/** Go to FM PTYCategory view*/
	setPTYforSeekFM: function(){
		MFT.States.goToState('media.radio.fm.options.PTYCategory');
	},
	
	/** Go to changePIN view*/
	changePIN: function(){
		MFT.States.goToState('media.radio.sirius.options.changePIN');
	},
	
	/** Go to mediaPlayer view*/
	goToMediaSettings: function(){
		MFT.States.goToState('settings.settings.mediaPlayer');
	},
	
	/** Go to deviceInformation view*/
	turnOnDeviceInformation: function(){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.deviceInformation');
	},
	
	/** Popup message text */
	popUpMessage: MFT.locale.label.view_media_options_popUp_message_autoSearch,
	
	/**
	 * TimerID cases:
	 * > 0 - is active
	 * -1  - end up
	 */
	media_option_bottom_messageTimerID: -1,
	media_status_preset_messageTimerID: -1,
	
	/**
	* Show element using id of element.
	*
	* @param {String} elementID param.
	*/
	showMessage: function( elementID, closeTime ){
        closeTime = closeTime || 2000;
		
		$('#'+elementID).show().animate({
			opacity: 1
		},500);
		
		self = this;
		// Hide popup after timeout
		this[elementID+'TimerID'] = setTimeout(function() {
            $('#' + elementID).animate({ opacity: 0 }, 500, function () {
                $(this).hide();
            });
            setTimeout(function () {
                self.set([elementID + 'TimerID'], -1);
            }, 500);
		},closeTime);
	},
	
	/**
	* Hide element using id of element.
	*
	* @param {String} elementID param.
	*/
	hideMessage: function( elementID ){
        clearTimeout(this[elementID+'TimerID']);
		$( '#' + elementID).stop().hide().css('opacity',0);
	},
	
	/** PopUp timer Id */
	popUpTimerId: null,
	
	/** AM Refresh/Store Autoset Presets (AST)*/
	refreshPresetsAm: function(){
		this.refreshPresets('am_refreshPresetPopup');
	},
	
	/** AM Refresh/Store Autoset Presets (AST)*/
	refreshPresetsFm: function(){
		this.refreshPresets('fm_refreshPresetPopup');
	},
	
	/** Refresh/Store Autoset Presets (AST)*/
	refreshPresets: function( popUpID ){
		
		/* Check: event isn't processing */
		if ( this.popUpTimerId != null){
			return false;
		}

		var self = this;
		
		/* Set and show first message */
		this.set('popUpMessage', MFT.locale.label.view_media_options_popUp_message_autoSearch);			
		this.showMessage( popUpID );		
		
		/* Set and shoe second message */
		this.popUpTimerId = setTimeout( function(){ 
			self.set('popUpMessage', MFT.locale.label.view_media_options_popUp_message_noStation);
			self.showMessage( popUpID );
			
			/*Set flag of end event*/				
			setTimeout(function() {
				self.set('popUpTimerId',null);}
			,3500);
		},3500)
	},
	
	/** Hide PopUp and reset timer*/
	resetPopUp: function( elementId ){
		
		this.hideMessage( elementId );
		clearInterval(this.popUpTimerId);
		this.set('popUpTimerId',null);
	},
	
	/** Options: Media index updating */
	
	updatingMessage: '',
	
	updatingMessageTimerId: null,
	
	/*
	 * Update Media Index
	 */
	updateMediaIndex: function(){
        var self = this;

		if( self.updatingMessageTimerId != null ){
			return false;
		}
        self.hideMessage('media_option_bottom_message');

        self.set('updatingMessage', MFT.locale.label.view_media_options_updateMediaIndex_message_usbDetected);
        self.showMessage('media_option_bottom_message');

		this.updatingMessageTimerId = setTimeout(function(){
            self.set('updatingMessage', MFT.locale.label.view_media_options_updateMediaIndex_message_indexing);
            self.showMessage('media_option_bottom_message');
            self.updatingMessageTimerId = setTimeout(function(){
                self.set('updatingMessage', MFT.locale.label.view_media_options_updateMediaIndex_message_indexComplete);
                self.showMessage('media_option_bottom_message');
                self.updatingMessageTimerId = setTimeout(function(){
                    self.set('updatingMessage', MFT.locale.label.view_media_options_updateMediaIndex_message_buildingVoiceCommands);
                    self.showMessage('media_option_bottom_message');
                    self.updatingMessageTimerId = setTimeout(function(){
                        self.set('updatingMessage', MFT.locale.label.view_media_options_updateMediaIndex_message_commandsAvail);
                        self.showMessage('media_option_bottom_message');
                        self.updatingMessageTimerId = setTimeout(function(){
                            self.set('updatingMessageTimerId', null);
                        }, 2500);
                    }, 3500);
                },3500)
            },3500)
		},3500);
	},

	/*
	 * Reset Update Media Index event
	 *
	 * @observes currentModuleData, BTModel
	 */
	resetUpdatingMessage: function(){
		clearTimeout( this.updatingMessageTimerId );

		this.hideMessage('media_option_bottom_message');
		this.set('updatingMessageTimerId', null);
	}.observes('this.currentModuleData', 'MFT.BTModel.active'),
	
	/**
	 * Fm Optoions RDSText Display On/Off
	 *
	 * 0 - On
	 * 1 - Off
	 */
	fmRDSTextOnOff: 1,
	
	/** Enable/Disable PTYButton status  */	
	PTYButtonIsDisabled: true,
	
	/*
	 * Set Enable/Disable PTYButton status
	 *
	 * @observes fmRDSTextOnOff
	 */
	setPTYstatus: function(){
		if (this.fmRDSTextOnOff == 0 ) {
			this.set('PTYButtonIsDisabled', false);
		} else {
			this.set('PTYButtonIsDisabled', true);
		}
	}.observes('this.fmRDSTextOnOff'),
	
	/** Fm RDSText visibility */
	fmRDSTextIsHidden: true,
	
	/** Current station is Hd */
	isActiveHdData: false,
	
	setHdDataStatus: function(){
	 	this.set('isActiveHdData', this.currentModuleData.get('selectedItem').isHd );
	}.observes('this.currentModuleData.selectedItem'),
	
	/**
	 * Hide Fm RDS Text
	 *
	 * @observes isActiveHdData, fmRDSTextOnOff
	 */
	hideFmRDSText: function(){
		if (  MFT.AmModel.active || this.isActiveHdData || this.fmRDSTextOnOff) {
			this.set('fmRDSTextIsHidden',true);
		}
			else this.set('fmRDSTextIsHidden',false);
	}.observes('this.isActiveHdData','this.fmRDSTextOnOff','MFT.AmModel.active'),
	
	/** Visibility of genres label */
	genreIsVisible: false,

	/**
	 * Show genr label
	 *
	 * @observes isActiveHdData, fmRDSTextOnOff
	 */
	showGenreText: function(){
		if ( this.isActiveHdData || !this.fmRDSTextOnOff) {
			this.set('genreIsVisible',true);
		}
			else this.set('genreIsVisible',false);
	}.observes('this.isActiveHdData','this.fmRDSTextOnOff'),
	
	/**
	 * Show sirius alert labels
	 *
	 * @observes siriusAlertsOnOff
	 */
	showSiriusAlerts: function(){
		if (!this.siriusAlertsOnOff) {
			this.set('siriusAlertsVisibility', true);
		} else {
			this.set('siriusAlertsVisibility', false);
		}
	}.observes('this.siriusAlertsOnOff'),
	
	/** SIRIUS Browse */
	turnOnSiriusBrowse: function(){
		this.onRadioEnter(MFT.SiriusModel, 'sirius.browse');
		this.listUp();
	},
	
	/** CD Browse */
	turnOnCDBrowse: function(){
		this.onPlayerEnter(MFT.CDModel,'cd.browse');
		this.listUp();
	},
	
	/** USB Browse */
	turnOnUSBBrowse: function(){
		this.onPlayerEnter(MFT.USBModel,'usb.browse');
		this.listUp();
	},
	
	/** SD Browse */
	turnOnSDBrowse: function(){
		this.onPlayerEnter(MFT.SDModel,'sd.browse');
		this.listDown();
	},
	
	/** Set Active current selected preset station */
	onPresetButtonActiveStateChange: function(index, playlist){
		/** newly selected preset station */
		playlist.set('selectedIndex', index);
	},
	
	/** Show Store Preset Message */
	showStorePresetMessage: function() {
		var self = this,
            homeMediaStatusTimer;

		/* Reset other UpdatingMessage timers */
		this.resetUpdatingMessage();
		
		this.set('updatingMessage', MFT.locale.label.view_media_popUp);
		
		// hide HomeMediastatus label
		this.set('isHomeMediastatusActive', false);
		// hide HomeMediaRadio status label
		this.set('isPresetActivated', true);
		
		// only in home state hide MediaStatus
		if ( this.isHomeStateActive ){
			this.set('isHomeMediaStatusHidden', true);
		}
		
		// Show Media and Status Bottom messages	
		this.showMessage('media_status_preset_message');
		this.showMessage('media_option_bottom_message');

	},
	
	
	/** Store preset station data */
	onStorePreset: function(playlistItem, playlist,index){
		/** Copy data from active station to current pressed*/
		if(this.directTuneSelected){
			if(!MFT.SiriusModel.active){
				playlistItem.copy(this.currentDirectTuneData.get('selectedDirectItem'));
			}else{
				
				playlistItem.copy(MFT.SiriusModel.directTunestations.get('selectedDirectItem'));
			}
			this.set('directTuneSelected',false);
		}else{
			playlistItem.copy(playlist.get('selectedItem'));
		}
		
		playlist.set('selectedIndex', index);
		
		// Show message
		this.showStorePresetMessage();
	},
	
	/** Player Next track event*/
	nextTrack: function() {
		this.currentSelectedPlayer.nextTrack();
	},
	
	/** Player Prev track event*/
	prevTrack: function() {
        if (this.currentSelectedPlayer.currentTime > 5) {
            this.currentSelectedPlayer.moveToBegining();
        } else {
            this.currentSelectedPlayer.prevTrack();
        }
	},
	
	/** Player Play track event*/
	playTrack: function() {
		this.currentSelectedPlayer.play();
	},
	
	/********************************************************  DIRECT TUNE LOGIC ******************************************/
	
	/**
	 * Direct tune methods
	 */
	onDirectTune: function() {
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Direct_tune');
			
			return;
		}
				
		this.set('directTuneHide', false );
			
		this.currentDirectTuneData.set('selectedDirectTuneStation',null);
		
		this.set('directTuneSelected',false);
	},
	
	/** Turn off direct tune buttom */ 
	offDirectTune: function() {
		this.set('directTuneHide', true );
	},
	
	/** On Direct tune hide event */
	onDirectTuneHide: function() {
		if ( this.directTuneHide ) {
			// SIRIUS (needs for visual representation)
			this.set('isSiriusDirectTunestations',false);
			
			this.set('directTuneStations', [] );
			
			this.set('directKeypressed', false); // need to determine that direct number was pressed
			
		} else {
			
			this.set('directTune', [] ); 
			
			this.set('directTuneFinished', false );
			
			if(!MFT.States.media.radio.sirius.active){
				this.set('directTuneStations', this.currentDirectTuneData.directTunes);
			}else{
				// SIRIUS
				this.set('isSiriusDirectTunestations',true);	
			}
		}
	}.observes('this.directTuneHide'),
	
	/** Direct tune key press handeler */
	onDirectTuneKeyPress: function( view ) {
		
		this.set('directKeypressed', true);
		
		this.set('directTuneSelected', false);
		
		this.currentDirectTuneData.set('selectedDirectTuneStation',null);
		
		this.directTune.pushObject( view.index  );
		
		this.setDirectTuneInterval();
	},
	
	/** Set Direct tune interval*/
	setDirectTuneInterval: function(){
		if(this.directTuneTimer){
			clearInterval(this.directTuneTimer);
		}
		this.directTuneTimer = setInterval(function(){
			MFT.MediaController.resetDirectTune();
		},5000);
	},
	
	/** Reset Direct tune*/
	resetDirectTune: function(){
		this.set('directTune', [] );
		this.set('directKeypressed', false);
		this.set('directTuneFinished', false);
	},
	
	/** Direct tune back press handeler */
	onDirectTuneBack: function() {
		this.set('directTuneFinished', false);
		this.set('directTuneSelected', false);
		this.directTune.popObject();
		this.setDirectTuneInterval();
	},
	
	/** Direct tune Enter press handeler */
	onDirectTuneSet: function() {
		clearInterval(this.directTuneTimer);
		this.set('directTuneFinished', false);
		this.set('directKeypressed', false);
		if(!MFT.States.media.radio.sirius.active){
			
			this.currentDirectTuneData.set('selectedDirectTuneStation',this.directTune.toString().replace(/,/g,''));
		}else{
			//Select saved direct tune data from SiriusModel
			this.currentDirectTuneData.set('selectedDirectTuneStation',Number(this.directTune.toString().replace(/,/g,'')) % 20);
			// Dinamicaly sett current station number to sirius direct data
			this.currentDirectTuneData.selectedDirectItem.set('channel','Channel '+this.directTune.toString().replace(/,/g,''));
		}
		this.set('directTuneSelected', true);
		this.set('directTune', [] );
	},
	
	/**
	 * Direct tune Station to String
	 *
	 * @property directTune
	 */
	directTuneToString: function(){
		if(this.directTune.length != 0){
			return this.directTune.toString().replace(/,/g,'');
		}else{
			clearInterval(this.directTuneTimer);
			this.set('directKeypressed', false);
			if(this.currentDirectTuneData.get('selectedDirectTuneStation') == null){
				return this.currentModuleData.get('selectedItem').frequency;
			}else{
				return this.currentDirectTuneData.get('selectedDirectItem').frequency;
			}
		}
	}.property('this.directTune.@each'),
	
	/**
	 * Keys for direct tune component
	 * 
	 * @property directTune ,directTuneStations 
	 * @return {number}
	 */
	directTuneKeys: function() {
		if(MFT.States && MFT.States.media.radio.sirius.active)
		{
			/************************ SIRIUS LOGIC ***********************/
			if(this.directTune.length != 0){
				// dont enable Enter button if Null selected
				if(Number(this.directTune.toString().replace(/,/g,'')) != 0){
					this.set('directTuneFinished', true);
				}
			
			}
			switch (this.directTune[0]){
				case 0: {
					return this.directTune.length === 3;	
					break;
				}
				case 1: {
					return this.directTune.length === 3;
					break;
				}
				default: {
					return this.directTune.length === 2;
					break;
				}
				
			
			}
		}else{
			
			var i, a, b, keys = [];
			
			for ( i = 0; i < this.directTuneStations.length; i++ ) {
				if ( this.compare( this.directTune, this.directTuneStations[i].slice(0, this.directTune.length) ) ) {
					keys.push( Number( this.directTuneStations[i][ this.directTune.length ] ) );
					
					// Set true if find station	
					if( this.directTune.length === this.directTuneStations[i].length ) {
						this.set('directTuneFinished', true);
					}
				}
			}
			return keys;
		
		}
	}.property('this.directTune.@each','this.directTuneStations'),
	
	disableZeroNum: function(){
		if(this.directTune.length == 2 && 
			Number(this.directTune.toString().replace(/,/g,'')) == 0)
		{
			return true;
		}else{
			return false;
		}
	}.property('this.directTune.@each'),
	
	/** Helper function to compare stations for direct tune */
	compare: function(tune, stations) {
		
		if ( tune.toString() === stations.toString() ) {
			return true;
		} else {
			return false;
		}
	},

	turnScanHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Scan');			
			return;
		}
	},
	
	turnReplayHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Replay');			
			return;
		}
	},
	
	turnShuffleHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Shuffle');			
			return;
		}
	},
	
	turnSimilarHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Similar');			
			return;
		}
	},
	
	turnAlertHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Alerts');			
			return;
		}
	},
	
	turnPresetHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Presets');			
			return;
		}
	},
	
	/**
	 * Define status of Radio HD button
	 *
	 * @property helpMode, locale
	 * @return {Boolean}
	 */
	isHDButtonActive: function(){
		return !(MFT.helpMode);
	}.property('MFT.helpMode','FLAGS.locale'),
	
	/**
	 * Get inverted status of Radio HD button
	 *
	 * @property isHDButtonActive
	 * @return {Boolean}
	 */
	isHDButtonActiveInverted: function(){
		return !this.get('isHDButtonActive');
	}.property('isHDButtonActive'),
	
	turnHdHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_HD_radio');			
			return;
		}
	}
});

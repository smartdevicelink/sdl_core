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
	activeState: 'media.radio',
	
	/** text labels (dinamicaly could be changed)*/
	amLabelBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_am1'),
	
	fmLabelBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_fm'),
	
	siriusLabelBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_sirius'),
	
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
	
	/** Detects top or bottom Left Menu items*/
	isTopListMenu: true,
	
	currentMediaGroup: 'radio',
	
	/** Current selected module Data reference*/
	currentModuleData: MFT.AmModel.am1,
	
	/** Current selected browse Data reference*/
	currentBrowseData: MFT.SiriusModel.browseData,
	
	/** Current selected browse Data reference*/
	currentOptionsData: MFT.AmModel.optionsData,
	
	/** Current selected player object  reference*/
	currentSelectedPlayer: null,
	
	radioDataArray : [MFT.AmModel],
	
	playerDataArray : [MFT.CDModel],
	
	/** Current Direct tune data*/
	currentDirectTuneData: MFT.AmModel.directTunestations,
	
	
	
	/**  On radio module exit event */
	onRadioExit: function(){
		if(this.radioDataArray.length >0){
			this.radioDataArray[0].band.set('activeBand',this.radioDataArray[0].band.value);
			this.radioDataArray[0].set('active',false);
			this.radioDataArray.pop().band.set('value',-1);
		}
		if(MFT.BTModel.active){
			MFT.BTModel.set('active',false);
		}
	},
	/**  On radio module enter event */
	onRadioEnter: function(data){
		// Exit player
		this.onPlayerExit();
		
		// Set Radio state 
		if(!MFT.States.media.radio.active){
			MFT.States.goToState('media.radio');
		}
		
		// Hide direct tune
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
		
		this.radioDataArray.push(data);
		/** load  options data*/
		this.set('currentOptionsData', this.radioDataArray[0].optionsData);
		/** load  direct tune  data*/
		this.set('currentDirectTuneData', this.radioDataArray[0].directTunestations);
		/** Set Active bans*/
		this.radioDataArray[0].band.set('value',this.radioDataArray[0].band.activeBand);
		/** Set current data active state*/
		this.radioDataArray[0].set('active',true);
	},
	
	/**  On player module exit event */
	
	onPlayerExit: function(){
		if(this.playerDataArray.length >0){
			if(this.playerDataArray[0].player.isPlaying){
				this.playerDataArray[0].player.pause();
			}
			this.playerDataArray.pop().set('active',false);
		}
		if(MFT.BTModel.active){
			MFT.BTModel.set('active',false);
		}

        if(MFT.ApplinkMediaModel.active){
        	MFT.ApplinkMediaModel.set('active',false);
        }
	},
	
	/**  On player module enter event */
	
	onPlayerEnter: function(data){
		// Hide direct tune
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
		
		//Player
		this.playerDataArray.push(data);	
		this.set('currentBrowseData', this.playerDataArray[0].browseData);
		this.set('currentModuleData',this.playerDataArray[0].PlayList);
		if(this.playerDataArray[0].optionsData){
			this.set('currentOptionsData', this.playerDataArray[0].optionsData);
		}
		this.set('currentSelectedPlayer',this.playerDataArray[0].player); 
		
		
		// Set player state
		if(!MFT.States.media.player.active && !MFT.States.media.browseall.active){
			MFT.States.goToState('media.player');
		}
		this.onRadioExit();
		this.playerDataArray[0].set('active',true);
	},
	
	/****************** AM *******************/
	
	/*toggle between AM stations */
	toggleAmStation: function(){
		if(MFT.AmModel.active){
			MFT.AmModel.band.toggleIndicators();
		}else{
			this.onRadioExit();
			this.onRadioEnter(MFT.AmModel);
		}
	},
	/*Observes for model value changed - switch selected station view*/
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
	
	/*toggle between FM stations */
	toggleFmStation: function(){
		if(MFT.FmModel.active){
			MFT.FmModel.band.toggleIndicators();
		}else{
			this.onRadioExit();
			this.onRadioEnter(MFT.FmModel);
		}	
	},

	/*Observes for model value changed - switch selected station view*/
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
	
	/*toggle between SIRIUS stations */
	toggleSiriusStation: function(){
		if(MFT.SiriusModel.active){
			MFT.SiriusModel.band.toggleIndicators();
		}else{
			
			this.onRadioExit();
			this.onRadioEnter(MFT.SiriusModel);
			/** load sirius browse data*/
			this.set('currentBrowseData', MFT.SiriusModel.browseData);
		}	
	},
	
	/*Observes for model value changed - switch selected station view*/
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
	
	/** Check for logo in station data*/
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
			
	/** On Preset Direct data Change */
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
	/*Turn on CD*/
	turnOnCD: function(){
		this.onPlayerExit();
		this.onPlayerEnter(MFT.CDModel);
	},
	
	/****************** USB *******************/
	/*Turn on USB*/
	turnOnUSB: function(){
		this.onPlayerExit();
		this.onPlayerEnter(MFT.USBModel);
	},
	
	/****************** SD *******************/
	/*Turn on SD*/
	turnOnSD: function(){
		this.onPlayerExit();
		this.onPlayerEnter(MFT.SDModel);
	},
	
	/*Turn on More Info*/
	turnMoreInfo: function(){
		if (MFT.helpMode) {
			MFT.VideoPlayerController.start('ent_More_info');
			return;
		}
		MFT.States.goToState('media.moreinfo');
	},
	/** Back MoreInfo*/
	backMoreInfo: function(){
		MFT.States.goToState('media.player');
	},
	
	/*Turn on Browse*/
	turnBrowse: function(){
		if (MFT.helpMode) {
			MFT.VideoPlayerController.start('ent_Browse');
			return;
		}
		
		MFT.States.goToState('media.browse');
		this.resetDirectTune();
	},
	
	/*Turn on Browse All*/
	turnBrowseAll: function(){
		MFT.States.goToState('media.browseall');
	},
	
	browseBack: function(){
		if(MFT.SiriusModel.active){
			MFT.States.goToState('media.radio');
		}else {
			MFT.States.goToState('media.player');
		}
	},
	
	/*Turn on SD*/
	turnOnBT: function(){
		// Exit form player or radio
		this.onPlayerExit();
		this.onRadioExit();
		// Set Bluetooth Data active
		MFT.BTModel.set('active',true);
		// Set Bluetoth Options Data
		this.set('currentOptionsData', MFT.BTModel.optionsData);
		// Go to Bluetooth state
		MFT.States.goToState('media.bluetooth');
		// hide directTune
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
	},
	/*Turn on SD*/
	turnOnAVin: function(){
		// Exit form player or radio
		this.onPlayerExit();
		this.onRadioExit();
		
		MFT.States.goToState('media.avin');
		// hide directTune
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
	},
	
	/* Scroll left menu up*/
	listDown: function(){
		this.set('isTopListMenu',false);
	},
	/* Scroll left menu down*/
	listUp: function(){
		this.set('isTopListMenu',true);
	},
	
	/** Options */
	turnOnOptions: function(){
		MFT.States.goToState('media.options');
	},

    /** Switching on Application */
    turnOnApplink: function(){

        // Exit form player or radio
        this.onPlayerExit();
        this.onRadioExit();
        // Set Applink Data active
        MFT.ApplinkMediaModel.set('active',true);
        // Go to Applink state
        MFT.States.goToState('media.applink');
        // hide directTune
        this.offDirectTune();
        if(this.directTuneSelected){
            this.set('directTuneSelected', false);
        }

    },

    /** Switching on Application */
    turnOnApplinkNonMedia: function(element){

        // Exit form player or radio
        this.onPlayerExit();
        this.onRadioExit();
        // Set Applink Data active
        MFT.ApplinkNonMediaModel.set('active',true);
        // Go to Applink state
        MFT.States.goToState('info.nonMedia');
        // hide directTune
        this.offDirectTune();
        if(this.directTuneSelected){
            this.set('directTuneSelected', false);
        }

        MFT.ApplinkNonMediaModel.turnOnApplink(element.appName, element.appId);

        /* Show Applink application in media left menu */
		//MFT.ApplinkMediaController.set('hideApplinkMediaButton', false);
		//MFT.MediaController.listDown();

    },

	optionsBack: function(){
		if(MFT.AmModel.active || MFT.FmModel.active || MFT.SiriusModel.active){
			MFT.States.goToState('media.radio');
		}else if(MFT.BTModel.active){
			MFT.States.goToState('media.bluetooth');
		}else{
			MFT.States.goToState('media.player');
		}
	},
	
	turnOnSoundSettings: function(){
		MFT.States.goToState('settings.sound');
	},
	
	//SIRIUS Browse
	turnOnSiriusBrowse: function(){
		// cancel scrollbar animation
		MFT.browseView.list.set('cancelAnimation',true);
		//enable scrollbar animation after 200 ms delay
		Ember.run.later(function(){
		 	MFT.browseView.list.set('cancelAnimation',false);
		}, 200);
		
		// set browse data
		this.set('currentBrowseData', MFT.SiriusModel.browseData);
		
		if(MFT.SiriusModel.active){
			this.onRadioExit();
		}
		//Turn Sirius Model Active
		this.onRadioEnter(MFT.SiriusModel);
		// Switch Browse state
		MFT.States.goToState('media.browse');

		this.listUp();
		
	},
	//CD Browse
	turnOnCDBrowse: function(){
		// cancel scrollbar animation
		MFT.browseView.list.set('cancelAnimation',true);
		//enable scrollbar animation after 200 ms delay
		Ember.run.later(function(){
		 	MFT.browseView.list.set('cancelAnimation',false);
		}, 200);
		// Turn CD Model Active
		this.turnOnCD();
		
		// Switch Browse state
		MFT.States.goToState('media.browse');
		
		this.listUp();
		
	},
	//USB Browse
	turnOnUSBBrowse: function(){
		this.turnOnUSB();
		// Switch Browse state
		MFT.States.goToState('media.browse');
		
		this.listUp();
	},
	//SD Browse
	turnOnSDBrowse: function(){
		this.turnOnSD();

		// Switch Browse state
		MFT.States.goToState('media.browse');
		
		this.listDown();
	},
	
	/*Set Active current selected preset station*/
	onPresetButtonActiveStateChange: function(index, playlist){
		/** newly selected preset station */
		playlist.set('selectedIndex', index);
	},
	
	/*Store preset station data*/
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
		
		// Show popup
		$('.preset-stored').fadeIn();
		// Hide popup after timeout
		setTimeout(function() {
			$('.preset-stored').fadeOut();
		},2000);
	},
	/** Player Next track event*/
	nextTrack: function() {
		this.playerDataArray[0].player.nextTrack();
	},
	/** Player Prev track event*/
	prevTrack: function() {
		this.playerDataArray[0].player.prevTrack();
	},
	/** Player Play track event*/
	playTrack: function() {
		this.playerDataArray[0].player.play();
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
	
	 /* Turn off direct tune buttom*/ 
	offDirectTune: function() {
		this.set('directTuneHide', true );
	},
	/* On Direct tune hide event */
	onDirectTuneHide: function() {
		if ( this.directTuneHide ) {
			// SIRIUS (needs for visual representation)
			this.set('isSiriusDirectTunestations',false);
			
			this.set('directTuneStations', [] );
			
			this.set('directKeypressed', false); // need to determine that direct number was pressed
			
		} else {
			
			this.set('directTune', [] ); 
			
			this.set('directTuneFinished', false );
			
			if(!MFT.SiriusModel.active){
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
		if(!MFT.SiriusModel.active){
			
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
	/** Direct tune Station to String */
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
	/** Keys for direct tune component */
	directTuneKeys: function() {
		if(!MFT.SiriusModel.active)
		{
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
		
		}else
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
		console.log(232323)
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
			MFT.VideoPlayerController.start('ent_Suffle');
			
			return;
		}
	},
	turnSimilarHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Similar');
			
			return;
		}
	},
	turnTagHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Tag');
			
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
	}
});
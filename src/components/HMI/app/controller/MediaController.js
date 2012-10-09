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
	activeState: 'media.Am',
	
	/** text labels (dinamicaly could be changed)*/
	amLabel :'AM 1', fmLabel :'FM', siriusLabel:'SIRIUS',
	
	/** Selected direct tune station */
	directTune: [],
	
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
	
	/** CD Player*/
	CDPlayer: MFT.MediaCDPlayer.create({data: MFT.CDModel.PlayList}),
	/** SD Player*/
	SDPlayer: MFT.MediaCDPlayer.create({data: MFT.SDModel.PlayList}),
	/** USB Player*/
	USBPlayer: MFT.MediaCDPlayer.create({data: MFT.USBModel.PlayList}),
			
	/*toggle between AM stations */
	toggleAmStation: function(){
		// Hide direct tune
		this.offDirectTune();
		this.set('directTuneSelected', false);
		
		if(MFT.States.media.Am.active){
			MFT.AmModel.band.toggleIndicators();
		}else{
			MFT.AmModel.band.set('value',MFT.AmModel.band.activeBand);
			if(MFT.States)MFT.States.goToState('media.Am');
			this.set('currentOptionsData', MFT.AmModel.optionsData);
		}
	},
	/*Observes for model value changed - switch selected station view*/
	switchAmSubstate: function(){
		switch(MFT.AmModel.band.value){
			case 0:{
				this.set('currentModuleData', MFT.AmModel.am1); 
				this.set('amLabel','AM 1');
				break;
			}
			case 1:{
				this.set('currentModuleData', MFT.AmModel.amAst);
				this.set('amLabel','AM AST');
				break;
			}
			default:
				this.set('amLabel','AM');
				break;			
		}
	}.observes('MFT.AmModel.band.value'),
	
	/*toggle between FM stations */
	toggleFmStation: function(){
		// Hide direct tune
		this.offDirectTune();
		this.set('directTuneSelected', false);
		
		if(MFT.States.media.Fm.active){
			MFT.FmModel.band.toggleIndicators();
		}else{
			MFT.FmModel.band.set('value',MFT.FmModel.band.activeBand);
			MFT.States.goToState('media.Fm');
			this.set('currentOptionsData', MFT.FmModel.optionsData);
		}	
	},
	
	/*Observes for model value changed - switch selected station view*/
	switchFmSubstate: function(){

		switch(MFT.FmModel.band.value){
			case 0:{
				this.set('currentModuleData', MFT.FmModel.fm1); 
				this.set('fmLabel','FM 1');
				break;
			}
			case 1:{
				this.set('currentModuleData', MFT.FmModel.fm2); 
				this.set('fmLabel','FM 2');
				break;
			}
			case 2:{
				this.set('currentModuleData', MFT.FmModel.fmAst); 
				this.set('fmLabel','FM AST');
				break;
			}				
			default:
				this.set('fmLabel','FM');
				break;
		}
	}.observes('MFT.FmModel.band.value'),
	
	/*toggle between SIRIUS stations */
	toggleSiriusStation: function(){
	
		// Hide direct tune
		this.offDirectTune();
		this.set('directTuneSelected', false);
		
		if(MFT.States.media.Sirius.active){
			MFT.SiriusModel.band.toggleIndicators();
		}else{
			/** load last active state*/
			MFT.SiriusModel.band.set('value',MFT.SiriusModel.band.activeBand);
			/** switch sirius view*/
			MFT.States.goToState('media.Sirius');
			/** load sirius browse data*/
			this.set('currentBrowseData', MFT.SiriusModel.browseData);
			/** load sirius options data*/
			this.set('currentOptionsData', MFT.SiriusModel.optionsData);
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
		if(this.currentModuleData.get('selectedDirectItem'))
		if(this.directTuneSelected){
			this.set('isStationLogo', this.currentModuleData.get('selectedDirectItem').logo.length > 1);
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
			return this.currentModuleData.get('selectedDirectItem');
		}else{
			return this.currentModuleData.get('selectedItem');
		}
	}.property(
		'MFT.MediaController.currentModuleData.selectedItem',
		'MFT.MediaController.currentModuleData.selectedDirectItem',
		'MFT.MediaController.directTuneSelected'
	),		
	
	/*Turn on CD*/
	turnOnCD: function(){
		MFT.States.goToState('media.cd');
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
		this.set('currentBrowseData', MFT.CDModel.browseData);
		this.set('currentModuleData',MFT.CDModel.PlayList);
		/** load cd options data*/
		this.set('currentOptionsData', MFT.CDModel.optionsData);
		this.set('currentSelectedPlayer',this.CDPlayer); 
	},
	/*Turn on More Info*/
	turnMoreInfo: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_More_info');
			
			return;
		}
		MFT.States.goToState('media.'+MFT.States.currentState.name+'.moreinfo');
	},
	/*Turn on Browse*/
	turnBrowse: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Browse');
			
			return;
		}
		this.resetDirectTune();
		MFT.States.goToState('media.'+MFT.States.currentState.name+'.browse');
	},
	/*Turn on Browse All*/
	turnBrowseAll: function(){
		MFT.States.goToState('media.'+MFT.States.currentState.parentState.name+'.browse.browseAll');
	},
	
	/*Turn on USB*/
	turnOnUSB: function(){
		MFT.States.goToState('media.usb');
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
		/** load cd options data*/
		this.set('currentOptionsData', MFT.USBModel.optionsData);
		this.set('currentBrowseData', MFT.USBModel.browseData);
		this.set('currentModuleData',MFT.USBModel.PlayList);
		this.set('currentSelectedPlayer',this.USBPlayer);  
	},
	/*Turn on SD*/
	turnOnSD: function(){
		MFT.States.goToState('media.sd');
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
		this.set('currentBrowseData', MFT.SDModel.browseData);
		this.set('currentModuleData',MFT.SDModel.PlayList);
		this.set('currentSelectedPlayer',this.SDPlayer); 
	},
	/*Turn on SD*/
	turnOnBT: function(){
		MFT.States.goToState('media.bt');
		/** load sirius options data*/
		this.set('currentOptionsData', MFT.BtModel.optionsData);
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
	},
	/*Turn on SD*/
	turnOnAVin: function(){
		MFT.States.goToState('media.avin');
		this.offDirectTune();
		if(this.directTuneSelected){
			this.set('directTuneSelected', false);
		}
	},
	listDown: function(){
		this.set('isTopListMenu',false);
	},
	listUp: function(){
		this.set('isTopListMenu',true);
	},
	
	/** **/
	turnOnOptions: function(){
		this.resetDirectTune();
		MFT.States.goToState(MFT.States.currentState.get('path')+'.options');
	},
	
	turnOnSiriusBrowse: function(){
		MFT.BrowseView.list.set('cancelAnimation',true);
		MFT.States.goToState('media.Sirius.browse');
		Ember.run.later(function(){
		 	MFT.BrowseView.list.set('cancelAnimation',false);
		}, 200);
		this.set('currentBrowseData', MFT.SiriusModel.browseData);
		/** load sirius options data*/
		this.set('currentOptionsData', MFT.SiriusModel.optionsData);
		this.listUp();
	},
	turnOnCDBrowse: function(){
		this.turnOnCD();
		this.listUp();
		MFT.BrowseView.list.set('cancelAnimation',true);
		MFT.States.goToState('media.cd.browse');
		Ember.run.later(function(){
		 	MFT.BrowseView.list.set('cancelAnimation',false);
		}, 200);
	},
	turnOnUSBBrowse: function(){
		this.turnOnUSB();
		this.listUp();
		MFT.States.goToState('media.usb.browse');
	},
	turnOnSDBrowse: function(){
		this.turnOnSD();
		this.listDown();
		MFT.States.goToState('media.sd.browse');
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
			playlistItem.copy(playlist.get('selectedDirectItem'));
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
		switch(MFT.States.currentState.name){
			case 'cd':{
				this.CDPlayer.nextTrack();
				break;
			}
			case 'usb':{
				this.USBPlayer.nextTrack();
				break;
			}
			case 'sd':{
				this.SDPlayer.nextTrack();
				break;
			}
		}
	},
	/** Player Prev track event*/
	prevTrack: function() {
		switch(MFT.States.currentState.name){
			case 'cd':{
				this.CDPlayer.prevTrack();
				break;
			}
			case 'usb':{
				this.USBPlayer.prevTrack();
				break;
			}
			case 'sd':{
				this.SDPlayer.prevTrack();
				break;
			}
		}
	},
	/** Player Play track event*/
	playTrack: function() {
		switch(MFT.States.currentState.name){
			case 'cd':{
				this.USBPlayer.pause();
				this.SDPlayer.pause();
				this.CDPlayer.play();
				break;
			}
			case 'usb':{
				this.SDPlayer.pause();
				this.CDPlayer.pause();
				this.USBPlayer.play();
				break;
			}
			case 'sd':{
				this.CDPlayer.pause();
				this.USBPlayer.pause();
				this.SDPlayer.play();
				break;
			}
		}
	},
	
	insertToParentView: function(view){
		if (!MFT.MediaView.get('childViews').contains(view) ) {				
				MFT.MediaView.get('childViews').pushObject(view);
		}
		
	},
	
	/**
	 * Direct tune methods
	 */
	onDirectTune: function() {
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('ent_Direct_tune');
			
			return;
		}		
		this.set('directTuneHide', false );
		this.currentModuleData.set('selectedDirectTuneStation',null);
		this.set('directTuneSelected',false);
	},
	
	offDirectTune: function() {
		this.set('directTuneHide', true );
	},
	
	onDirectTuneHide: function() {
		if ( this.directTuneHide ) {
			this.set('directTuneStations', [] );
			MFT.MediaController.set('directKeypressed', false);
			
		} else {
			this.set('directTune', [] );
			this.set('directTuneFinished', false );
			this.set('directTuneStations', this.currentModuleData.directTunes );
		}
	}.observes('this.directTuneHide'),
	
	/** Direct tune key press handeler */
	onDirectTuneKeyPress: function( view ) {
		this.set('directKeypressed', true);
		this.set('directTuneSelected', false);
		
		if(this.currentModuleData.get('selectedDirectTuneStation') != null){
			this.currentModuleData.set('selectedDirectTuneStation',null);
		}
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
		},4000);
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
		this.currentModuleData.set('selectedDirectTuneStation',this.directTune.toString().replace(/,/g,''));
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
			if(this.currentModuleData.get('selectedDirectTuneStation') == null){
				return this.currentModuleData.get('selectedItem').frequency;
			}else{
				return this.currentModuleData.get('selectedDirectItem').frequency;
			}
		}
	}.property('this.directTune.@each'),
	/** Keys for direct tune component */
	directTuneKeys: function() {
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
	}.property('this.directTune.@each','this.directTuneStations'),
	
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
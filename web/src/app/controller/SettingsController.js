/**
 * @name MFT.SettingsController
 * 
 * @desc Settings	Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/SettingsController.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsController = Em.Object.create({
		
	activeState:  'settings.clock',

	onState: function(event){
		MFT.States.goToState('settings.'+event.goToState);
	},

	onChildState: function(event){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	},

	/** time string for top clock */
	topTime: '',
	
	settingsTime: '',

	temperature: function() {
		if ( MFT.SettingsModel.temperatureType ) {
			return '27&deg;';
		} else {
			return '86&deg;';
		}
	}.property('MFT.SettingsModel.temperatureType'),

    /**
     * Brightness increase/decrease value
     **/
    brightIncrease: function() {
        MFT.SettingsModel.brightness.increase();
    },

    brightDecrease: function() {
        MFT.SettingsModel.brightness.decrease();
    },

    /**
     * Status Auto Dim display
     **/
    isAutoDim: function() {
        return !MFT.SettingsModel.autoDim;
    }.property('MFT.SettingsModel.autoDim'),

    /**
     * Auto Dim offset increase/decrease value
     **/
    autoDimOffsetIncrease: function() {
        MFT.SettingsModel.autoDimOffset.increase();
    },

    autoDimOffsetDecrease: function() {
        MFT.SettingsModel.autoDimOffset.decrease();
    },

	/**
	 * Get and format time for top clock
	 * @param date - Date object
	 */
	getTime: function(date) {
		
		var minutes = date.getMinutes();
		var hours = date.getHours();
		
		minutes = ( minutes < 10 )	?	'0'+minutes	: minutes;
		
		hours =	( hours > 12 )		?	hours -12	: hours;
		hours = ( hours == 0 )		?	12			: hours;
		
		this.set('time', hours + ':' + minutes);//this.get('temperature'));
	},

	// Valet mode popup
	valetPopUp: false,
	
	showValetPopUp: function(){
		this.set('valetPopUp',true);
	},
	
	hideValetPopUp: function(){
		this.set('valetPopUp',false);
	},
	
	// Enter pin
	onEnterPin: function(){
		this.hideValetPopUp();
		MFT.States.goToState('settings.vehicle.enterPIN');
	},
	
	setMeasurementSystems: function(){
		switch (FLAGS.locale){
		case "eng":
			MFT.SettingsModel.set('temperatureType',0);
			MFT.SettingsModel.set('distanceType',1);
			break;
        case "spa":
		case "fr":
			MFT.SettingsModel.set('temperatureType',1);
			MFT.SettingsModel.set('distanceType',0);
			break;
		} 
	},
	
	language: function(){
		switch (FLAGS.locale){
			case "eng": 
				return 0;
			case "spa":
				return 1;
			case "fr":
				return 2;
		}
	}.property(),
	
	/** Back to previous state from Settings Navigation view */
	settingNavigationBack: function(){
		if ( MFT.States.previousState.indexOf('snavigation') >= 0 ) {
			MFT.States.back();
		} else {
			MFT.States.goToPreviousState();
		}
	},
	
	/** Show MasterReset Waring Popup */
	showMasterResetWarning: function(){		
		this.showMessage( 'settings_warning_popup_view' , true );
		this.showMasterResetWaringContent();	
	},
	/** Hide MasterReset Waring Popup */
	hideMasterResetWaring: function(){
		this.hideMessage( 'settings_warning_popup_view');
	},
	
	/** Show MasterReset Waring Popup Dialogue content */	
	showMasterResetWaringContent: function() {		
		this.showMessage( 'settings_warning_popup_view_content' , true );
	},
	
	/** Hide MasterReset Waring Popup Dialogue content */	
	hideMasterResetWaringContent: function(){		
		this.hideMessage( 'settings_warning_popup_view_content' );
	},
	
	/** 
	 * Settings Waringn PopUp message
	 *
	 * True  - 1st Dialogue "Reset To Factory"
	 * False - 2nd Dialogue "Erase Data"
	 */
	isActiveResetToFactoryWarningContent: true,
	
	/** Show Master Reset - "Reset To Factory Warning" Dialogue */
	showResetToFactoryWarning: function() {
		
		this.set('isActiveResetToFactoryWarningContent',true);
		this.showMasterResetWarning();
		
	},
	
	/** Show Master Reset - "Erase Data Warning" Dialogue */
	showEraseDataWarning: function() {
		
		this.hideMasterResetWaringContent();
		
		var self = this;
		
		setTimeout(function() {
			self.set('this.isActiveResetToFactoryWarningContent',false);
			self.showMasterResetWaringContent();			
		},500);
	},
	
	/** Master Reset Dialoue Positive scenario */
	masterResetPositiveAction: function(){
		var self = this;
		
		if ( this.isActiveResetToFactoryWarningContent ) {
			
			/* In case of 1st Dialogue "Reset To Factory" */
			this.showEraseDataWarning();
			
		} else {
			
			/* In case of 1st Dialogue "Reset To Factory" */
			this.hideMasterResetWaring();
			this.showMasterResetNotifier();
			
		}
	},
	
	/** Master Reset Dialoue Positive scenario */
	masterResetNegativeAction: function(){
		this.hideMasterResetWaring();
	},
	
	/** Show Master Reset 3rd notifier */
	showMasterResetNotifier: function() {
		this.showMessage('settings_settings_system_MasterResetNotifier_popUp');
	},
	
	/**
	 * ShowMessage TimerId
	 * Cases:
	 *    > 0 - is active
	 *    -1  - end up
	 */
	showMessageTimerID: -1,
	
	/**
	 * Show Message 
	 *
	 * @param{string} elementID param
	 * @param{Boolean} stayAfter param. True - Message wouldn't be hidden. 
	 */
	showMessage: function( elementID , stayAfter ){
		
		elementID = '#'+elementID;
		
		$(elementID).css('display','block');
		
		$(elementID).animate({
			opacity: 1
		},500);
		$( elementID ).css('pointer-events', 'auto');
		
		if ( !stayAfter ) {
			self = this;
			// Hide popup after timeout
			this.showMessageTimerID = setTimeout(function() {
				$(elementID).animate({ opacity: 0 },500);
				$(elementID).css('display','none');
				setTimeout(function() {
					self.showMessageTimerID = -1;
				},500);
			},2000);
		}
	},
	
	/**
	 * Hide Message 
	 */
	hideMessage: function( elementID ){
		elementID = '#'+elementID;
		$(elementID).animate({ opacity: 0 },500);
		$(elementID).css('display','none');	
	}
});
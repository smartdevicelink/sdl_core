/**
 * @name MFT.AppController
 * 
 * @desc App general logic
 * 
 * @category	Controller
 * @filesource	app/controller/AppController.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.AppController = Em.Object.create({

    /** Active vehicle mode for the whole application */
    activeVehicleModel: function(){
        /** Return current or default vehicle model*/
        return MFT.vehicles[FFW.Backend.vehicleModel] || MFT.vehicles[FFW.Backend.DEFAULT_VEHICLE_MODEL];
    }.property('FFW.Backend.vehicleModel'),
	
	/****   System Settings  Popup Logic   *****/
	// visual representation variable for system select view popup
	active: false,

	// default navigation option
	navigationEnabled: true,

    popUpMessage: '',

    /** Reverse SDL functionality **/
    sdlAccessStatus: false,

    /**
     * Reverse SDL control status
     *
     * 1 - Driver control
     * 2 - Waiting passenger control
     * 3 - Passenger control rejected
     * 4 - Passenger control
     */
    sdlControlStatus: 1,

    SDLGrantAccessPopupVisible: false,
	
	/* Buttons array*/
	buttons: MFT.SelectSystempopup.content.climate_style.list._childViews,
	/* Set Active state to pressed button */
	setActiveButton: function(button){
		//remove active state for prev button
		this.buttons.setEach('active',false);
		// set active to current button
		button.set('active',true);
		// set Vechicle Model
		this.set('vehicleType', button.elementId);
	},

    getCurrentDate: function(){
        var now = new Date();
        return MFT.locale.label['month_' + now.getMonth()] + ' ' + now.getDate() + ', ' + now.getFullYear();
    }.property(),
	
	onYespress: function(){
		this.set('navigationEnabled',true);
	},
	
	onNopress: function(){
		this.set('navigationEnabled',false);
	},
	
	onSettingsSubmit: function(){
		// sen selected options to backend
		FFW.Backend.set('isNavigationEnabled',this.navigationEnabled);
		FFW.Backend.set('vehicleModel', this.vehicleType);
		//remove system settings view
		//view._parentView.remove();
		this.set('active',false);
		// save selected options
		FFW.Backend.saveVehicleModel();
		FFW.Backend.saveIsNavigationEnabled();
	},	
	/****   END System Settings  Popup Logic   *****/
	
	/**
	 * Invert value of IsNavigationEnabled
	 *
	 * @property(boolean) isNavigationEnabled
	 * @return boolean
	 */
	invertIsNavigationEnabled: function(){
		return !FFW.Backend.isNavigationEnabled
	}.property('FFW.Backend.isNavigationEnabled'),

    /**
     * Show popup and close it after closeTime, if it exists
     * @param {number} closeTime time, after which the popup will close
     * @param {string} message, displaying message popup
     * @param {string} callback function
     */
    showPopup: function(elementId, closeTime, message, callback){
        var self = this;

        if (typeof message != 'undefined') {
            this.set('popUpMessage', message);
        }

        $('#'+elementId).show().animate({
            opacity: 1
        },500);

        if(!isNaN(closeTime)){
            this[elementId+'TimerId'] = setTimeout(function() {
                self.hidePopup(elementId, callback);
            }, closeTime);
        }
    },

    hidePopup: function(elementId, callback){
        clearTimeout(this[elementId+'TimerId']);
        $('#'+elementId).css({
            'opacity': 0
        }).hide();

        if (typeof (callback) != 'undefined') {
            callback();
        }
    },

    showSDLGrantAccessPopup: function() {
        this.showPopup('SDLGrantAccessPopup');
        this.set('SDLGrantAccessPopupVisible', true);
    },

    hideSDLGrantAccessPopup: function() {
        this.hidePopup('SDLGrantAccessPopup');
        this.set('SDLGrantAccessPopupVisible', false);
    },

    sendAccessRequest: function() {
        if(MFT.AppController.sdlAccessStatus){
            FFW.RevSDL.sendCancelAccessRequest();
        } else if (!this.SDLGrantAccessPopupVisible) {
            this.showSDLGrantAccessPopup();
        } else {
            FFW.RevSDL.sendGrantAccessRequest();

            this.set('sdlControlStatus', 2);

            this.hideSDLGrantAccessPopup();
        }
    },

    driverFocus: function() {
        this.set('sdlAccessStatus', false);
        this.set('sdlControlStatus', 1);
        this.showPopup('SDLMessagesPopup', 2000, MFT.locale.label.view_sdl_messages_popup_driverControl);
    },

    changeAccessStatus: function(data) {
        var self = this;

        if (data.success) {
            this.set('sdlAccessStatus', true);
            this.set('sdlControlStatus', 4);
            this.showPopup('SDLMessagesPopup', 2000, MFT.locale.label.view_sdl_messages_popup_granted);
        } else if(data.resultCode == 'REJECTED') {
            this.set('sdlControlStatus', 3);
            this.showPopup('SDLMessagesPopup', 2000, MFT.locale.label.view_sdl_messages_popup_denied, function () {
                self.set('sdlControlStatus', 1);
            });
        }
    }
});
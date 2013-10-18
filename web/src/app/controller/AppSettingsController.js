/**
 * @name MFT.AppSettingsController
 *
 * @desc App general logic
 *
 * @category    Controller
 * @filesource    app/controller/AppSettingsController.js
 * @version        2.0
 *
 * @author        Andrii Velychko
 */

MFT.AppSettingsController = Em.Object.create({

    activeState: 'appSettings',

    errorSendMailPopupIsVisible: false,

    onChildState: function(event){
        MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
    },

    /*
     * Set Infor text according to OS
     */
    rateMessage: function(){
        if (FFW.isAndroid) {
            return MFT.locale.label.view_appSettings_appRate_note_android;
        } else if (FFW.isIOS){
            return MFT.locale.label.view_appSettings_appRate_note_ios;
        } else {
            return MFT.locale.label.view_appSettings_appRate_note_desktop;
        }
    },

    vehicleModelName: function(){
        return FFW.Backend.vehicleModel || FFW.Backend.DEFAULT_VEHICLE_MODEL
    }.property('FFW.Backend.vehicleModel'),

    setActiveVehicleModel: function(button){
        if(!this.vehicleButtons){
            this.set('vehicleButtons', MFT.configureView.vehicleType.vehicleList._childViews)
        }
        //remove active state for prev button
        this.vehicleButtons.setEach('active',this.value);
        // set active to current button
        button.set('active',true);

        // set Vechicle Model and send it to backend
        FFW.Backend.set('vehicleModel', button.value);
        FFW.Backend.saveVehicleModel();

        if (MFT.vehicles[button.value].isClimateDisabled && MFT.States.appSettings.from.indexOf('climate') != -1) {
            MFT.States.appSettings.from = MFT.SettingsController.activeState;
        }
    },

    navigationEnable: function(){
        if (MFT.States.appSettings.from == MFT.InfoController.activeState) {
            MFT.States.appSettings.from = MFT.NavigationController.activeState;
        }
        FFW.Backend.set('isNavigationEnabled', true);
        FFW.Backend.saveIsNavigationEnabled();
    },

    navigationDisable: function(){
        if (MFT.States.appSettings.from == MFT.NavigationController.activeState) {
            MFT.States.appSettings.from = MFT.InfoController.activeState;
        }
        FFW.Backend.set('isNavigationEnabled', false);
        FFW.Backend.saveIsNavigationEnabled();
    },

    /** Full Screen
     * 0 - on
     * 1 - off */
    fullScreen: 0,

    toggleFullScreen: function(){
        FFW.Backend.set('isFullScreen', !this.get('fullScreen'));
        if(!FFW.isAndroid){
            FFW.Backend.setFullScreen();
        }
    }.observes('fullScreen'),

    openEULA: function(){
        FFW.Backend.openEULA();
    },

    sendSupportEmail: function(){
        FFW.Backend.sendSupportEmail();
    },

    showErrorSendMailPopup: function() {
        this.set('errorSendMailPopupIsVisible', true);
    },

    hideErrorSendMailPopup: function() {
        this.set('errorSendMailPopupIsVisible', false);
    }

});
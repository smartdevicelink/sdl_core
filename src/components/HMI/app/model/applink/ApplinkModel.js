/**
 * @name MFT.ApplinkModel
 * 
 * @desc General model for Applink applications
 * 
 * @category  Model
 * @filesource  app/model/applink/ApplinkModel.js
 * @version    1.0
 *
 * @author    Artem Petrosyan
 */

MFT.ApplinkModel = Em.Object.create({

    /**
     * Flag to indicate AudioPassThruPopUp activity
     * @type {Bool}
     */
    AudioPassThruState:     false,

    /**
     * Default values for global properties
     */
    globalPropertiesDefault : {
        helpPrompt  : [ 
            {
                "text"  : "Some text for help prompt",
                "type"  : "TEXT"
            }
        ],

        timeoutPrompt   : [
            {
                "text"  : "Some text for timeout prompt",
                "type"  : "TEXT"
            }
        ]
    },

    /**
     * List of states for OnTBTClientState notification
     */
    tbtClientStates: [
        {
            name:   "ROUTE_UPDATE_REQUEST",
            id:     0
        },
        {
            name:   "ROUTE_ACCEPTED",
            id:     1
        },
        {
            name:   "ROUTE_REFUSED",
            id:     2
        },
        {
            name:   "ROUTE_CANCELLED",
            id:     3
        },
        {
            name:   "ETA_REQUEST",
            id:     4
        },
        {
            name:   "NEXT_TURN_REQUEST",
            id:     5
        },
        {
            name:   "ROUTE_STATUS_REQUEST",
            id:     6
        },
        {
            name:   "ROUTE_SUMMARY_REQUEST",
            id:     7
        },
        {
            name:   "TRIP_STATUS_REQUEST",
            id:     8
        },
        {
            name:   "ROUTE_UPDATE_REQUEST_TIMEOUT",
            id:     9
        }
    ],


    /**
     * Data for AudioPassThruPopUp that contains params for visualisation
     * @type {Object}
     */
    AudioPassThruData:     {},
    
    /**
     * List of registered applications,
     * To prevent errors without registered application "-1" used as test appId
     * @type object
     */
    registeredApps: {
    },

    /**
     * List of icons
     * 
     * @type {Object}
     */
    listOfIcons:{
        //appId: syncFileName
        0:  "images/media/ico_li.png"
    },

    /**
     * Array of active applications
     * @type {Array}
     */
    applicationsList:           [],

    /**
     * Array of connected devices
     * @type {Array}
     */
    devicesList:                [],

    /**
     * Global properties
     * @type {Object}
     */
    globalProperties:{
        helpPrompt:             [],
        timeoutPrompt:          []
    },

    /**
     * Current language of UI component
     * @type {String}
     */
    UILanguage: '',

    /**
     * Current language of TTS and VR component
     * @type {String}
     */
    TTSVRLanguage: '',

    /**
     * List of supported languages
     * @type {Array}
     */
    applinkLanguagesList: [
        'EN-US',
        'ES-MX',
        'FR-CA',
        'DE-EU',
        'ES-EU',
        'EN-EU',
        'RU-RU',
        'TR-TR',
        'PL-EU',
        'FR-EU',
        'IT-EU',
        'SV-EU',
        'PT-EU',
        'NL-EU',
        'EN-AU',
        'ZH-CN',
        'ZH-TW',
        'JA-JP',
        'AR',
        'KO-KR'
    ],

    /**
     * Method to open Phone view and dial phone number
     * @param {Object} params
     */
    dialNumber: function( params ){
        MFT.States.goToState('phone.dialpad');
        MFT.PhoneModel.set('dialpadNumber', params.number);
        MFT.PhoneController.onDialCall();
    },

    /**
     * Method to open Turn By Turn view
     * @param {Object} params
     */
    tbtActivate: function( params ){
        MFT.TurnByTurnView.activate( params );
    },

    /**
     * Method to set data for Turn List in applications model
     * @param {Object} params
     */
    tbtTurnListUpdate: function( params ){
        MFT.ApplinkController.getApplicationModel( params.appId ).turnList = params.turnList;
        MFT.TBTTurnList.updateList( params.appId );
    },

    /**
     * Method to set language for UI component with parameters sent from ApplinkCore to UIRPC
     * @param {string} lang Language code.
     */
    changeRegistrationUI: function( lang ){
        this.set( 'UILanguage', lang );
    },

    /**
     * Method to add activation button to VR commands and set device parameters to model
     * @param {Object} params
     */
    onAppRegistered: function( params ){

        if( params.isMediaApplication ){
            MFT.ApplinkController.registerApplication(params, 0 );
        }else{
            MFT.ApplinkController.registerApplication(params, 1 );
        }
        MFT.VRPopUp.AddActivateApp(params.appId, params.appName);
    },

    /**
     * Method to delete activation button from VR commands and delete device parameters from model
     * @param {Object} params
     */
    onAppUnregistered: function( params ){
        //MFT.ApplinkController.getApplicationModel(params.appId).set('appName', "<No app>");
        //MFT.ApplinkController.getApplicationModel(params.appId).set('hideApplinkButton', true);
        MFT.VRPopUp.DeleteActivateApp(params.appId);

        MFT.States.goToState('info.apps');

        MFT.ApplinkModel.get('applicationsList').removeObjects(
            MFT.ApplinkModel.applicationsList.filterProperty( params.appId )
        );

        delete MFT.ApplinkModel.registeredApps[ params.appId ];

        MFT.ApplinkModel.getApplicationModel( params.appId ).onDeleteApplication( params.appId );

        MFT.ApplinkController.findNewApps();
    },

    /**
     * Method to set language for TTS and VR components with parameters sent from ApplinkCore to UIRPC
     * @param {string} lang Language code
     */
    changeRegistrationTTSVR: function( lang ){
        this.set( 'TTSVRLanguage', lang );
    },

    /**
     * Applink UI ScrolableMessage activation function
     * dependent of Driver Distraction toggle state
     * @param {Object} Object with parameters come from ApplinkCore.
     */
    onApplinkScrolableMessage: function(params){

        if( MFT.ApplinkController.driverDistractionState ){
            MFT.DriverDistraction.activate();
        }else{
            MFT.ScrollableMessage.activate( MFT.ApplinkController.getApplicationModel(params.appId).appName , params );
        }

    },

    /*
     * resetGlobalProperties
     * @param {String} propertyName Name of propety to reset.
     */
    resetProperties: function( params ) {

        var i,
            len = params.properties.length;
        for (i=0; i < len; i++){
            if ( params.properties[i] == "HELPPROMPT" ){
                this.set('globalProperties.helpPrompt', this.globalPropertiesDefault.helpPrompt);
            }

            if ( params.properties[i] == "TIMEOUTPROMPT" ){
                this.set('globalProperties.timeoutPrompt', this.globalPropertiesDefault.timeoutPrompt);
            }
        }
    },

    /**
     * setGlobalProperties
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    setProperties: function(message) {

        this.set('globalProperties.helpPrompt', message.helpPrompt);
        this.set('globalProperties.timeoutPrompt', message.timeoutPrompt);

    },

    onGetAppList: function( appList ){

        var i=0,
            len = appList.length;
        for(i = 0; i < len; i++){
            MFT.ApplinkModel.onAppRegistered( appList[i] );
        }

    },

    onGetDeviceList: function( params ){
        if (null == params.resultCode || (null != params.resultCode && "SUCCESS" == params.resultCode)) {
            if( MFT.States.info.devicelist.active && params.deviceList && params.deviceList.length){
                MFT.DeviceListView.ShowDeviceList( params );
            }
        }
    },

    /**
     * Applink UI SetAppIcon handler
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    onApplinkSetAppIcon: function( message ){
        MFT.ApplinkController.getApplicationModel(message.appId).set('appIcon', message.syncFileName );
    },

    /**
     * Applink UI Alert Maneuver response handeler
     * show popup window 
     *
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    onUIAlertManeuver: function( message ) {

        MFT.AlertManeuverPopUp.AlertManeuverActive( message );
    },

    /**
     * Applink UI Alert response handeler
     * show popup window 
     *
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    onUIAlert: function( message ) {

        MFT.AlertPopUp.AlertActive( message );
    },

    /**
     * Applink UI AudioPassThru response handeler
     * show popup window 
     *
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    UIPerformAudioPassThru: function( message ) {
        this.set('AudioPassThruData', message );
        this.set('AudioPassThruState', true );
    },

    /**
     * Method ends processing of AudioPassThru
     * and call AudioPassThru UI response handeler
     */
    UIEndAudioPassThru: function() {
        if( this.AudioPassThruState ){
            FFW.UI.sendUIResult( "SUCCESS", FFW.UI.endAudioPassThruRequestId, "UI.EndAudioPassThru");
            MFT.ApplinkController.performAudioPassThruResponse("SUCCESS");
        }else{
            FFW.UI.sendUIResult( "GENERIC_ERROR", FFW.UI.endAudioPassThruRequestId, "UI.EndAudioPassThru");
        }
    },

    /** 
     * Prompt activation
     * @param {Object}
     */
    onPrompt: function(ttsChunks){
        var message = '';
        if(ttsChunks){
            for(var i = 0; i < ttsChunks.length; i++){
              message += ttsChunks[i].text + '\n';
            }
            MFT.TTSPopUp.ActivateTTS(message);
        }
    },
  
    /**
    * Applink VR AddCommand response handeler
    * add command to voice recognition window
    *
    *  @param {Object}
    */
    addCommandVR: function ( message ) {
        MFT.VRPopUp.AddCommand( message.cmdId, message.vrCommands, message.appId );
    },
  
    /**
    * Applink VR DeleteCommand response handeler
    * delete command from voice recognition window
    *
    * @param {Number}
    */
    deleteCommandVR: function ( commandId ) {    
        MFT.VRPopUp.DeleteCommand( commandId );
    },
    
    onDeactivateApp: function ( target, appId, appName ) {    

        var dest = target.split('.'),
            reason;

        switch( dest[0] ){
            case 'media' :{
                reason = 'AUDIO';
                break;
            }
            case "phone" :{
                reason = 'PHONEMENU';
                break;
            }
            case "navigation" :{
                reason = 'NAVIGATIONMAP';
                break;
            }
            case "settings" :{
                reason = 'SYNCSETTINGS';
                break;
            }
            default:{
                reason = 'GENERAL';
                break;
            }
        }

        FFW.AppLinkCoreClient.DeactivateApp( appName, reason, appId );
    }
});
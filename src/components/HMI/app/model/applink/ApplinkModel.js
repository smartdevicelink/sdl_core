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
     * init object
     */
    init: function() {
        // init global properties
        //this.resetProperties();
    },

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
        "-1": MFT.ApplinkMediaModel.create(),            // Used for media applications
        "-2": MFT.ApplinkNonMediaModel.create(),         // Used for non media applications
    },

    /**
     * Chosen device name
     *
     * @type {String}
     */
    deviceName: null,

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
            MFT.ApplinkController.registerApplication(params.appId, 0);
        }else{
            MFT.ApplinkController.registerApplication(params.appId, 1);
        }
        MFT.VRPopUp.AddActivateApp(params.appId, params.appName);
        // add new app to the list
        MFT.ApplinkController.getApplicationModel(params.appId).appInfo.set('appName', params.appName);
        //MFT.ApplinkController.getApplicationModel(params.appId).set('appId', params.appId);

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
            MFT.ScrollableMessage.activate( MFT.ApplinkController.getApplicationModel(params.appId).appInfo.appName , params );
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

        this.applicationsList.splice(0, this.applicationsList.length);
        for(var i = 0; i < appList.length; i++){
            this.set('listOfIcons.' + appList[i].appId, appList[i].icon );    
            this.applicationsList.push({
                type:       MFT.Button,
                params:     {
                    action:         'turnOnApplink',
                    target:         appList[i].isMediaApplication ? 'MFT.ApplinkMediaController' : 'MFT.NonMediaController',
                    text:           appList[i].appName,
                    appName:        appList[i].appName,
                    appId:          appList[i].appId,
                    className:      'scrollButtons button notpressed',
                    icon:           appList[i].icon,
                    templateName:   appList[i].icon ? 'rightIcon' : 'text'
                }                                   
            });
        }
        MFT.InfoAppsView.ShowAppList();

    },

    onGetDeviceList: function( params ){
        if (null == params.resultCode || (null != params.resultCode && "SUCCESS" == params.resultCode)) {
            if( MFT.States.info.devicelist.active && params.deviceList && params.deviceList.length){
                MFT.DeviceLilstView.ShowDeviceList( params );
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
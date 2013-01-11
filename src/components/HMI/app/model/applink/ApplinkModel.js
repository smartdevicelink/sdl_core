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

    /*
     * init object
     */
    init: function() {
        // init global properties
        this.resetProperties();
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
    globalProperties    : {
        helpPrompt  : null ,
        timeoutPrompt   : null
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
     * Type of current vehicle: make of the vehicle, model of the vehicle,
     * model Year of the vehicle, trim of the vehicle.
     * @type {Object}
     */ 
    vehicleType:{
      make:       "Ford",
      model:      "Fiesta",
      modelYear:  2013,
      trim:       "SE"  
    },

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
     * Method calls GetVehicleType response
     */
    getVehicleType: function(){
        FFW.VehicleInfo.GetVehicleTypeResponse( this.vehicleType );
    },

    /**
     * Method to set language for UI component with parameters sent from ApplinkCore to UIRPC
     * @param {string} lang Language code.
     */
    changeRegistrationUI: function( lang ){
        this.set( 'UILanguage', lang );
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
    resetProperties: function(propertyName) {
        if (propertyName == "HELPPROMPT" || propertyName == ""){
            this.globalProperties.helpPrompt    = this.globalPropertiesDefault.helpPrompt;
        }
    
        if (propertyName == "TIMEOUTPROMPT" || propertyName == ""){
            this.globalProperties.timeoutPrompt = this.globalPropertiesDefault.timoutPrompt;
        }
    },

    /**
     * setGlobalProperties
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    setProperties: function(message) {

        this.globalProperties.helpPrompt = message.helpPrompt;
        this.globalProperties.timeoutPrompt = message.timeoutPrompt;
        MFT.ControlButtons.infoTable.globalPropertiesData.propertiesData();

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
            this.devicesList.splice(0, this.devicesList.length);
            for(var i = 0; i < params.deviceList.length; i++){
                this.devicesList.push({
                    type:       MFT.Button,
                    params:     {
                        text:           params.deviceList[i],
                        deviceName:     params.deviceList[i],
                        className:      'scrollButtons button notpressed',
                        icon:           params.icon,
                        templateName:   'rightIcon',
                        actionUp:     function(){
                           MFT.ApplinkController.onDeviceChoosed(this);
                        }
                    }                                   
                });
            }
            
            if( MFT.States.info.devicelist.active ){
                MFT.DeviceLilstView.ShowDeviceList();
            }
        }
    },

    /**
     * Applink UI SetAppIcon handler
     * @param {Object} message Object with parameters come from ApplinkCore.
     */
    onApplinkSetAppIcon: function( message ){
        this.set('listOfIcons.' + message.appId, message.syncFileName );
        var tempId = MFT.ApplinkMediaModel.activeAppId;
        MFT.ApplinkMediaModel.activeAppId = -1;
        MFT.ApplinkMediaModel.set('activeAppId', tempId);
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
    //message = JSON.parse('{"appId":65537,"cmdId":4,"vrCommands":["F you bc hi iv"]}');
    
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
  }
});
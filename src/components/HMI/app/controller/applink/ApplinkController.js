/**
 * @name MFT.ApplinkController
 * 
 * @desc Main Applink Controller
 * 
 * @category	Controller
 * @filesource	app/controller/applink/ApplinkController.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkController = Em.Object.create({
	
	/**
	 * List of registered applications
	 * @type object
	 */
	 
	// To prevent errors without registered application "-1" used as test appId
	registeredApps: {
		"-1": 0,			// Used for media applications
		"-2": 1				// Used for non media applications
	},

    /*
     * Driver Distraction State
     *
     * @type bool
     */
    driverDistractionState:		false,

    /*
     * Protocol Version 2 State
     *
     * @type bool
     */
    protocolVersion2State:		false,

	/** 
	 * List of Applink application models
	 *
	 * @type object
	 */
	applicationModels: {
		0:	'ApplinkMediaModel',
		1:	'ApplinkNonMediaModel'
	},

    /**
     * Method to set language for UI component with parameters sent from ApplinkCore to UIRPC
     * @param {string} lang Language code
     */
    onLanguageChangeUI: function( lang ){
        FFW.UI.OnLanguageChange( lang );
    },

    /**
     * Method to set language for TTS and VR components with parameters sent from ApplinkCore to UIRPC
     * @param {string} lang Language code
     */
    onLanguageChangeTTSVR: function( lang ){
        FFW.TTS.OnLanguageChange( lang );
        FFW.VR.OnLanguageChange( lang );
    },
	
	/**
	 * Register application
	 * @desc ...
	 *
	 * @param applicationId: Number
	 * @param applicationType: Number
	 */
	registerApplication: function( applicationId, applicationType ) {
		if ( this.registeredApps[ applicationId ] ) {
			//Em.Logger.error('Application ['+ applicationId +'] already registered!');
			return;
		}
		
		this.registeredApps[ applicationId ] = applicationType;
		//Em.Logger.log('Application ['+ applicationId +'] registered!');
	},

	/**
	 * Unregister application
	 * @desc ...
	 *
	 * @param applicationId: Number
	 */
	unRegisterApplication: function( applicationId ) {
		delete this.registeredApps[ applicationId ];
	},

    /**
     * Applink Driver Distraction ON/OFF switcher
     * 
     * @param checked: bool
     */
    selectDriverDistraction: function(checked){
        if(checked){
            FFW.UI.onDriverDistraction( "DD_ON" );
            this.set('driverDistractionState', true);
        }else{
            FFW.UI.onDriverDistraction( "DD_OFF" );
            this.set('driverDistractionState', false);
        }
    },

    /**
     * Applink Protocol Version 2 ON/OFF switcher
     * 
     * @param checked: bool
     */
    selectProtocolVersion: function(checked){
        if(checked){
            FFW.AppLinkCoreClient.OnVersionChanged( 2 );
            this.set('protocolVersion2State', true);
        }else{
            FFW.AppLinkCoreClient.OnVersionChanged( 1 );
            this.set('protocolVersion2State', false);
        }
    },

	/**
	 * Get application model
	 * @desc ...
	 *
	 * @param applicationId: Number
	 * @return Object Model
	 */
	getApplicationModel: function( applicationId ) {
		return MFT[ this.applicationModels[ this.registeredApps[ applicationId ] ] ];
	},

	/* Function returns ChangeDeviceView 
	 * back to previous state
	 */
	turnChangeDeviceViewBack: function(){
        MFT.States.goToState('info.apps');
    },

	/**
	 * Enter screen vith list of devices application model
	 * 
	 */
	onGetDeviceList: function(  ) {
		MFT.States.goToState('info.devicelist');
		FFW.AppLinkCoreClient.getDeviceList();
	},

	/**
	 * Send notification if device was choosed
	 *
	 * @param element: MFT.Button
	 */
	onDeviceChoosed: function( element ) {
		FFW.UI.OnDeviceChosen( element.deviceName );
		MFT.ApplinkModel.set( 'deviceName', element.deviceName );
		this.turnChangeDeviceViewBack();
	},

	/**
	 * Method creates list of Application ID's
	 * Then call HMI method for display a list of Applications
	 * @param {Object}
	 */
	onGetAppList: function( appList ){
		for(var i = 0; i < appList.length; i++){
			if( appList[i].isMediaApplication ){
				MFT.ApplinkController.registerApplication(appList[i].appId, 0);
			}else{
				MFT.ApplinkController.registerApplication(appList[i].appId, 1);
			}
		}
		MFT.ApplinkModel.onGetAppList( appList );
	},

	/**
	 * Method call's request to get list of applications
	 */
	findNewApps: function(){
		FFW.AppLinkCoreClient.getAppList();
	},

	/**
	 * Method activates selected registered application
	 * @param {Object}
	 */
	onActivateApplinkApp: function(element){
		this.getApplicationModel(element.appId).turnOnApplink( element.appName, element.appId );
	},

	
	/**
	 * Method sent softButtons pressed and event status to RPC
	 * @param {Object}
	 */
	onSoftButtonActionUp: function( element ){
		FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONUP", element.softButtonID);
        if(element.time > 0){
            FFW.Buttons.buttonPressedCustom( "CUSTOM_BUTTON", "LONG", element.softButtonID);
        }else{
            FFW.Buttons.buttonPressedCustom( "CUSTOM_BUTTON", "SHORT", element.softButtonID);
        }
        element.time = 0;
    },

	/**
	 * Method sent softButtons pressed and event status to RPC 
	 * @param {Object}
	 */
	onSoftButtonActionDown: function( element ){
        FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONDOWN", element.softButtonID);
        element.time = 0;
        setTimeout(function(){ element.time ++; }, 1000);
	}
});
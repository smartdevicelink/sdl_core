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
     * Driver Distraction State
     *
     * @type bool
     */
    driverDistractionState:		false,

    /**
     * Protocol Version 2 State
     *
     * @type bool
     */
    protocolVersion2State:		false,
    
    /**
     * Current system context
     *
     * @type {String}
     */
    sysContext: function() {
        
        if ( MFT.VRPopUp.VRActive ) {
            return 'VRSESSION';
        }
        
        if ( MFT.AlertPopUp.activate ) {
            return 'ALERT';
        }
        
        if ( MFT.TTSPopUp.active || MFT.TBTClientStateView.active || MFT.VehicleInfo.active ) {
            return 'HMI_OBSCURED';
        }

        if ( MFT.OptionsView.active ) {
            return 'MENU';
        }
        
        if ( MFT.States.info.nonMedia.active || MFT.States.media.applink.active ) {
            return 'MAIN';
        } else {
            return 'MENU';
        }

    }.property(
        'MFT.OptionsView.active',
        'MFT.TTSPopUp.active',
        'MFT.VRPopUp.VRActive',
        'MFT.AlertPopUp.activate',
        'MFT.TBTClientStateView.active',
        'MFT.VehicleInfo.active',
        'MFT.States.info.nonMedia.active',
        'MFT.States.media.applink.active'
    ),
    
	/** 
	 * List of Applink application models
	 *
	 * @type object
	 */
	applicationModels: {
		0:	MFT.ApplinkMediaModel,
		1:	MFT.ApplinkNonMediaModel
	},

    /**
     * Handler for SoftButtons default action
     */
    defaultActionSoftButton: function( element ){
        switch( element.groupName ){

            case "AlertPopUp" :{
                MFT.AlertPopUp.deactivate( true );
                break;
            }
            case "ScrollableMessage" :{
                MFT.ScrollableMessage.deactivate();
                break;
            }
            case "TurnByTurnView" :{
                MFT.TurnByTurnView.deactivate();
                break;
            }
            case "info_nonMedia" :{
                MFT.ApplinkController.getApplicationModel( element.appId ).clearAppOverLay();
                break;
            }
            case "applink_view_container" :{
                MFT.ApplinkController.getApplicationModel( element.appId ).clearAppOverLay();
                break;
            }

        }
    },

    /**
     * Handler for SoftButtons stealFocus action
     */
    stealFocusSoftButton: function( element ){
        switch( element.groupName ){

            case "AlertPopUp" :{
                MFT.AlertPopUp.deactivate();
                MFT.ApplinkController.getApplicationModel( element.appId ).turnOnApplink();
                break;
            }
            case "ScrollableMessage" :{
                MFT.ScrollableMessage.deactivate();
                MFT.ApplinkController.getApplicationModel( element.appId ).turnOnApplink();
                break;
            }

        }
    },

    /**
     * Handler for SoftButtons keepContext action
     */
    keepContextSoftButton: function( element ){
        switch( element.groupName ){

            case "AlertPopUp" :{
                MFT.AlertPopUp.set('timer', 0);
                break;
            }
            case "ScrollableMessage" :{
                MFT.ScrollableMessage.set('timer', 0);
                break;
            }

        }
    },

    /**
     * Method to close AlertMeneuverPopUp view
     */
    closeAlertMeneuverPopUp: function(){
        MFT.AlertManeuverPopUp.set('activate', false);
    },

    /**
     * Method to open Turn List view from TBT
     * @param {Number} appId AppId of activated applink application
     */
	tbtTurnList: function( appId ){
        MFT.TBTTurnList.activate( appId );
    },

    /**
     * Method to set selected state of vehicle transmission to vehicleData
     * @param {string} prndl Vehicle transmission state
     */
    onPRNDLSelected: function( prndl ){
        MFT.ApplinkVehicleInfoModel.set('vehicleData.VEHICLEDATA_PRNDLSTATUS.data', prndl);
    },

    /**
     * Method to sent notification with selected state of TBT Client State
     * @param {String}
     */
    tbtClientStateSeleced: function( state ){
        FFW.UI.onTBTClientState( state, MFT.ApplinkAppController.model.appId );
    },

    /**
     * Method to sent notification ABORTED for PerformInteractionChoise
     */
    interactionChoiseCloseResponse: function(){
        FFW.UI.interactionResponse( "ABORTED" );
    },

    /**
     * Method to sent notification for Alert
     */
    alertResponse: function( result ){
        FFW.UI.alertResponse( result );
    },

    /**
     * Method to call performAudioPassThruResponse with Result code parameters
     * @param {Object} element Button object
     */
	callPerformAudioPassThruPopUpResponse: function( element ){
        this.performAudioPassThruResponse( element.responseResult );
    },

    /**
     * Method close PerformAudioPassThruPopUp and call response from UIRPC back to ApplinkCore
     * @param {string} result Result code
     */
	performAudioPassThruResponse: function( result ){
        MFT.ApplinkModel.set('AudioPassThruState', false);
        FFW.UI.sendUIResult( result, FFW.UI.performAudioPassThruRequestId, "UI.PerformAudioPassThru" );
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
	 *
	 * @param {Object} params
	 * @param {Number} applicationType
	 */
	registerApplication: function( params, applicationType ) {
		if ( MFT.ApplinkModel.registeredApps[ params.appId ] ) {
			return;
		}
		
		MFT.ApplinkModel.registeredApps[ params.appId ] = this.applicationModels[applicationType].create({
            appId:      params.appId,
            appName:    params.appName,
            deviceName: params.deviceName
		});

        MFT.ApplinkModel.get('applicationsList').pushObject( params.appId );

        MFT.VRPopUp.AddActivateApp(params.appId, params.appName);
	},

    /**
     * Applink Driver Distraction ON/OFF switcher
     * 
     * @param {Boolean}
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
     * Applink Send Data ON/OFF extended param switcher
     * 
     * @param {Boolean}
     */
    selectSendData: function( checked ){
        MFT.ApplinkModel.set('sendDataExtend', checked);
    },

    /**
     * Applink Protocol Version 2 ON/OFF switcher
     * 
     * @param {Boolean}
     */
    selectProtocolVersion: function(checked){
        if(checked){
            FFW.AppLinkCoreClient.OnVersionChanged( 2 );
        }else{
            FFW.AppLinkCoreClient.OnVersionChanged( 1 );
        }
    },

	/**
	 * Get application model
	 *
	 * @param {Number} Application id
	 * @return {Object} Application model
	 */
	getApplicationModel: function( applicationId ) {
		return MFT.ApplinkModel.registeredApps[ applicationId ];
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
		this.turnChangeDeviceViewBack();
	},

	/**
	 * Method creates list of Application ID's
	 * Then call HMI method for display a list of Applications
	 * @param {Object}
	 */
	onGetAppList: function( appList ){
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
		this.getApplicationModel(element.appId).turnOnApplink();
	},

	/**
	 * Method sent custom softButtons pressed and event status to RPC
	 * @param {Object}
	 */
	onSoftButtonActionUpCustom: function( element ){
        if(element.time > 0){
            FFW.Buttons.buttonPressedCustom( "CUSTOM_BUTTON", "LONG", element.softButtonID);
            FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONUP", element.softButtonID);
        }else{
            FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONUP", element.softButtonID);
            FFW.Buttons.buttonPressedCustom( "CUSTOM_BUTTON", "SHORT", element.softButtonID);
        }
        element.time = 0;
    },

	/**
	 * Method sent custom softButtons pressed and event status to RPC 
	 * @param {Object}
	 */
	onSoftButtonActionDownCustom: function( element ){
        FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONDOWN", element.softButtonID);
        element.time = 0;
        setTimeout(function(){ element.time ++; }, 2000);
	},
	
	/**
	 * Method sent softButtons pressed and event status to RPC
     * @param {String}
     * @param {Object}
	 */
	onSoftButtonActionUp: function( name, element ){
        if(element.time > 0){
            FFW.Buttons.buttonPressed( name, "LONG" );
            FFW.Buttons.buttonEvent( name, "BUTTONUP" );
        }else{
            FFW.Buttons.buttonEvent( name, "BUTTONUP" );
            FFW.Buttons.buttonPressed( name, "SHORT" );
        }
        element.time = 0;
    },

    /**
     * Method sent softButton OK pressed and event status to RPC
     * @param {String}
     */
    onSoftButtonOkActionUp: function( name ){
        FFW.Buttons.buttonEvent( name, "BUTTONUP" );
        FFW.Buttons.buttonPressed( name, "SHORT" );
        MFT.ApplinkAppController.model.set('isPlaying', !MFT.ApplinkAppController.model.isPlaying);
    },
        

	/**
	 * Method sent softButtons pressed and event status to RPC 
     * @param {String}
     * @param {Object}
	 */
	onSoftButtonActionDown: function( name, element ){
        FFW.Buttons.buttonEvent( name, "BUTTONDOWN" );
        element.time = 0;
        setTimeout(function(){ element.time ++; }, 2000);
	},
	
	/**
	 * Send system context
	 */
	onSystemContextChange: function() {
        FFW.UI.OnSystemContext( this.get('sysContext') );
	}
});

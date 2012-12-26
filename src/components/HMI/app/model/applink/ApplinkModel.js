/**
 * @name MFT.ApplinkModel
 * 
 * @desc General Applink model
 * 
 * @category	Model
 * @filesource	app/model/applink/ApplinkModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
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
     * Stored Vehicle Data
     */
    vehicleInfoPRNDL: [
        "Parking",
        "Reverse gear",
        "No gear",
        "Forward drive",
        "1st gear hold"
    ],

    /**
     * Stored Vehicle Data
     */
    vehicleData: [{
            'type':     "gps",
            'data':     null
        },
        {
            'type':     "speed",
            'data':     null
        },
        {
            'type':     "rpm",
            'data':     null
        },
        {
            'type':     "fuelLevel",
            'data':     null
        },
        {
            'type':     "avgFuelEconomy",
            'data':     null
        },
        {
            'type':     "batteryVoltage",
            'data':     null
        },
        {
            'type':     "externalTemperature",
            'data':     null
        },
        {
            'type':     "vin",
            'data':     null
        },
        {
            'type':     "prndl",
            'data':     null
        },
        {
            'type':     "tirePressure",
            'data':     null
        },
        {
            'type':     "batteryPackVoltage",
            'data':     null
        },
        {
            'type':     "batteryPackCurrent",
            'data':     null
        },
        {
            'type':     "batteryPackTemperature",
            'data':     null
        },
        {
            'type':     "engineTorque",
            'data':     null
        },
        {
            'type':     "odometer",
            'data':     null
        },
        {
            'type':     "tripOdometer",
            'data':     null
        },
        {
            'type':     "genericbinary",
            'data':     null
        }
    ],
	
	/**
	 * Chosen device name
	 *
	 * @type:	String
	 */
	deviceName: null,

    /**
     * List of icons
     *
     * @type:   Struct
     */
    listOfIcons:{
        //appId: syncFileName
        0:  "images/media/ico_li.png"
    },

    /**
      * Array of active applications
      */
    applicationsList:           new Array(),

    /**
      * Array of connected devices
      */
    devicesList:                new Array(),

    /*
     * Global properties
     */
    globalProperties    : {
        helpPrompt  : null ,
        timeoutPrompt   : null
    },

    /**
     * Applink UI ScrolableMessage handler
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
     */
    resetProperties: function(propertyName) {
        if (propertyName == "HELPPROMPT" || propertyName == "")
            this.globalProperties.helpPrompt    = this.globalPropertiesDefault.helpPrompt;
    
        if (propertyName == "TIMEOUTPROMPT" || propertyName == "")
            this.globalProperties.timeoutPrompt = this.globalPropertiesDefault.timoutPrompt;
    },

    /*
     * setGlobalProperties
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
            if( appList[i].isMediaApplication ){    
                this.applicationsList.push({
                    type:       MFT.Button,
                    params:     {
                        action:         'turnOnApplink',
                        target:         'MFT.ApplinkMediaController',
                        text:           appList[i].appName,
                        appName:        appList[i].appName,
                        appId:          appList[i].appId,
                        className:      'scrollButtons button notpressed',
                        icon:           appList[i].icon,
                        templateName:   appList[i].icon ? 'rightIcon' : 'text'
                    }                                   
                });
            }else{
                this.applicationsList.push({
                    type:       MFT.Button,
                    params:     {
                        action:         'turnOnApplink',
                        target:         'MFT.NonMediaController',
                        text:           appList[i].appName,
                        appName:        appList[i].appName,
                        appId:          appList[i].appId,
                        className:      'scrollButtons button notpressed',
                        icon:           appList[i].icon,
                        templateName:   appList[i].icon ? 'rightIcon' : 'text'
                    }                                   
                });
            }
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
     * @param message:Object
     */
	onUIAlert: function( message ) {
		
		MFT.AlertPopUp.AlertActive(
			message.appId,
			message.AlertText1,
			message.AlertText2,
            message.alertText3,
            message.ttsChunks,
			message.duration,
			message.playTone
		);
	},
	
	/** 
	 * Prompt activation
	 */
	onPrompt: function(ttsChunks){
		/*
		ttsChunks = JSON.parse('[{"text":"speak","type":"TEXT"},{"text":"INITIAL_JINGLE","type":"PRE_RECORDED"},{"text":"that was a jingle","type":"TEXT"},{"text":". 1 l ih v 1 .  _ l ay v .  r iy 1 d  . r eh d .","type":"SAPI_PHONEMES"}]');
		*/
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
	 * @param message:	Object
	 */
	onAddCommandVR: function ( message ) {
		//message = JSON.parse('{"appId":65537,"cmdId":4,"vrCommands":["F you bc hi iv"]}');
		
		MFT.VRPopUp.AddCommand( message.cmdId, message.vrCommands, message.appId );
	},
	
	/**
	 * Applink VR DeleteCommand response handeler
	 * delete command from voice recognition window
	 *
	 * @param commandId:	Number
	 */
	onDeleteCommandVR: function ( commandId ) {
		//message = 4;
		
		MFT.VRPopUp.DeleteCommand( commandId );
	},

    /** 
     * Function returns response message to VehicleInfoRPC
     */
    onGetVehicleData: function( message ){

        return this.vehicleData[message.dataType];

    },

    /** 
     * Function call FFW.VehicleInfo.OnVehicleData when data changes
     */
    onVehicleDataChanged: function(){

        var jsonData = { "gps": this.vehicleData[0].data, "speed": this.vehicleData[1].data, "rpm": this.vehicleData[2].data,
        "fuelLevel": this.vehicleData[3].data, "avgFuelEconomy": this.vehicleData[4].data, "batteryVoltage": this.vehicleData[5].data,
        "externalTemperature": this.vehicleData[6].data, "vin": this.vehicleData[7].data, "prndl": this.vehicleData[8].data,
        "tirePressure": this.vehicleData[9].data, "batteryPackVoltage": this.vehicleData[10].data, "batteryPackCurrent": this.vehicleData[11].data,
        "batteryPackTemperature": this.vehicleData[12].data, "engineTorque": this.vehicleData[13].data, "odometer": this.vehicleData[14].data,
        "tripOdometer": this.vehicleData[15].data, "genericbinary": this.vehicleData[16].data };
        FFW.VehicleInfo.OnVehicleData(jsonData);

    }.observes('this.vehicleData.@each.data')
});
 
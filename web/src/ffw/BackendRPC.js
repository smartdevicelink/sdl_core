/*
 * Reference implementation of Backend component.
 * 
 * Interface to get some essential information from OS.
 * Since web application is not able to access some OS feature through WebKit
 * it is necessary to use communication with native code to get some values 
 * and execute some functions, such as storing persistency to disk, 
 * getting window size and DPI, getting OS type, OS version 
 * and to determine if we are running in browser or native application. 
 * 
 *
 * @author Sergey Solovey
 */

FFW.Backend = FFW.RPCObserver.create({
		
	/*
     *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "BackendClient" }),
	
	/*
     * true if application is running first time after installation/update
 	 */		
	 isFirstStart: 	false,
	
	/*
     * true if application is running in full screen mode
 	 */		
	 isFullScreen: true,
	 
	/*
     * true if application is running in foreground
	 * false if application is in background
 	 */		
	 isInForeground: true,

	/*
     * height and width of WebView screen size in pixels
 	 */		
	 windowSize:	FFW.Size.create(),
	
	/*
     * Window density returned by Backend RPC from Native code
	 * Basis on this variable windowScale is calculated
	 * 
 	 */		
	 windowDensity:		0,
	 
	 
	/*
	 * information about application environment
 	 */		
	 OSInfo:		FFW.OSInfo.create(),

	/*
	 * Default vehicle model. It is selected when user haven't chosen any model.
	 *   edge, explorer, escape_2013, f150_2013, fiesta, flex_2013, focus, fusion_2013, taurus_2013
	 */
    DEFAULT_VEHICLE_MODEL: "edge",

    /** Current vehicle model, selected by the user. */
	vehicleModel: null,

	/*
	 * Specifies whether navigation is enabled.
	 */
	 isNavigationEnabled: true,

	/*
	 * identifiers for requests
 	 */		
	 isFirstStartRequestId: 		-1,
	 isFullScreenRequestId: 		-1,
     setFullScreenRequestId:        -1,
	 getWindowSizeRequestId: 		-1,
	 getWindowDensityRequestId: 	-1,
	 getOSInfoRequestId: 			-1,
	 FullScreenRequestId: 			-1,
	 getVehicleModelRequestId:		-1,
	 getIsNavigationEnabledRequestId:	-1,
	 saveVehicleModelRequestId:		-1,
	 saveIsNavigationEnabledRequestId:	-1,

	 onAppStatusChangedSubscribeRequestId:				-1,
	 onAppStatusChangedUnsubscribeRequestId:			-1,
	 onNavigationEnabledChangedSubscribeRequestId:		-1,
	 onNavigationEnabledChangedUnsubscribeRequestId:	-1,
	 onVehicleModelChangedSubscribeRequestId:			-1,
	 onVehicleModelChangedUnsubscribeRequestId:			-1,

	/*
	 * constants for properties names
	 */
	appStatusChangedProperty:			"Backend.onAppStatusChanged",
	navigationEnabledChangedProperty:	"Backend.onHasMapsChanged",
	vehicleModelChangedProperty:		"Backend.onVehicleChanged",
	 
	/*
	 * Stores the number of calls of notifyAppLoaded method.
	 */
	 appLoadedEventsNumber:			0,

	/*
	 * indicates if full screen mode changes was during video playing 
	 */
	 resizeVideo:			false,

	/*
   	 * This function sends message for native code logger (usually console or traces)
	 * 
 	 */	
	logToOS: function(message) {
	},

	/*
   	 * connect to RPC bus
 	 */
	connect: function() {
		// to be removed
		//this.stubGetValues();
		
		this.client.connect(this);
	},

	/*
	 * Subscribes to property. Returns the request's id.
	 */
	subscribeToProperty: function(property) {
		var msgId = this.client.generateId();
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		msgId,
			"method":	"MB.subscribeTo",
			"params":	{
				"propertyName": property
			}
		};
		this.client.send(JSONMessage);
		return msgId;
	},

	/*
	 * Unsubscribes from property. Returns the request's id.
	 */
	unsubscribeFromProperty: function(property) {
		var msgId = this.client.generateId();
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		msgId,
			"method":	"MB.unsubscribeFrom",
			"params":	{
				"propertyName": property
			}
		};
		this.client.send(JSONMessage);
		return msgId;
	},

	/*
   	 * disconnect from RPC bus
 	 */
	disconnect: function() {
		this.onAppStatusChangedUnsubscribeRequestId = this.unsubscribeFromProperty(this.appStatusChangedProperty);
		this.onNavigationEnabledChangedUnsubscribeRequestId = this.unsubscribeFromProperty(this.navigationEnabledChangedProperty);
		this.onVehicleModelChangedUnsubscribeRequestId = this.unsubscribeFromProperty(this.vehicleModelChangedProperty);

		this.client.disconnect();
	},

	/*
     * Client is registered - we can send request starting from this point of time
 	 */	
	onRPCRegistered: function () {
		Em.Logger.log("FFW.Backend.onRPCRegistered");
		this._super();

		this.notifyAppLoaded();

		this.onAppStatusChangedSubscribeRequestId = this.subscribeToProperty(this.appStatusChangedProperty);
		this.onNavigationEnabledChangedSubscribeRequestId = this.subscribeToProperty(this.navigationEnabledChangedProperty);
		this.onVehicleModelChangedSubscribeRequestId = this.subscribeToProperty(this.vehicleModelChangedProperty);

		// this data may have been changed in the background
		this.getIsFullScreen();
		this.getWindowSize();
		this.getVehicleModel();
		this.getIsNavigationEnabled();

		// if the requestId is uninitialized (-1), we assume it's the first connect
		var isFirstConnect = (this.isFirstStartRequestId == -1);
		if (isFirstConnect) {
			// request necessary parameters from Backend
			this.getIsFirstStart();
			this.getWindowDensity();
			this.getOSInfo();
		} else {
			// no need to update the data above
		}

	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.Backend.onRPCUnregistered");
		this._super();
	},

	/*
	 * Client disconnected.
	 */
	onRPCDisconnected: function() {
		this.notifyAppLoaded();
	},

	/*
	 * Checks that the response to a message is normal.
	 * Otherwise, prints a warning.
	 * Returns whether the response is normal.
	 */
	checkRPCResponse: function(response) {
		if (response.result != "OK") {
			Em.Logger.log("FFW.Backend: response is not OK: " + response.result);
			return false;
		}
		return true;
	},

	/*
     * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onRPCResult: function(response) {
		Em.Logger.log("FFW.Backend.onRPCResult");
		this._super();
		
		if (response.id == this.isFirstStartRequestId)
		{
			this.set('isFirstStart', response.result.isFirstStart);
		} else if (response.id == this.isFullScreenRequestId)
		{
			this.set('isFullScreen', response.result.isFullScreen);
		} else if (response.id == this.getWindowSizeRequestId)
		{
			this.windowSize.height  = response.result.height;
			this.windowSize.width 	= response.result.width;
		} else if (response.id == this.getWindowDensityRequestId)
		{
			this.set('windowDensity', response.result.windowDensity);
			//send msg to android to hide splash screen
			if(FFW.isAndroid){
				var JSONMessage = {
					"jsonrpc":	"2.0",
					"method":	"BackendClient.onAppLoaded"
				};
				this.client.send(JSONMessage);
			}
		} else if (response.id == this.getOSInfoRequestId)
		{
			this.OSInfo.OS			= response.result.OS;
			this.OSInfo.set('OSVersion',response.result.OSVersion);
			this.OSInfo.isNative 	= response.result.isNative;
		} else if (response.id == this.getVehicleModelRequestId) {
			this.set('vehicleModel', response.result.vehicle);
		} else if (response.id == this.getIsNavigationEnabledRequestId) {
			this.set('isNavigationEnabled', response.result.hasMaps);
		} else if ((response.id == this.onAppStatusChangedSubscribeRequestId) ||
				(response.id == this.onAppStatusChangedUnsubscribeRequestId) ||
				(response.id == this.onNavigationEnabledChangedSubscribeRequestId) ||
				(response.id == this.onNavigationEnabledChangedUnsubscribeRequestId) ||
				(response.id == this.onVehicleModelChangedSubscribeRequestId) ||
				(response.id == this.onVehicleModelChangedUnsubscribeRequestId) ||
				(response.id == this.saveIsNavigationEnabledRequestId) ||
				(response.id == this.saveVehicleModelRequestId))
		{
			this.checkRPCResponse(response);
		}
	 },
	
	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.Backend.onRPCError");
		this._super();

		if (!FFW.isAndroid) {
			if (this.client.socket.readyState == this.client.socket.OPEN) {
				var getter = null;
				if (response.id == this.isFirstStartRequestId) {
					getter = this.getIsFirstStart;
				} else if (response.id == this.isFullScreenRequestId) {
					getter = this.getIsFullScreen;
				} else if (response.id == this.getWindowSizeRequestId) {
					getter = this.getWindowSize;
				} else if (response.id == this.getWindowDensityRequestId) {
					getter = this.getWindowDensity;
				} else if (response.id == this.getOSInfoRequestId) {
					getter = this.getOSInfo;
				} else if (response.id == this.isNavigationEnabled) {
					getter = this.getIsNavigationEnabled;
				} else if (response.id == this.getVehicleModelRequestId) {
					getter = this.getVehicleModel;
				}

				// reinvoke the method after a while
				if (getter != null) {
					setTimeout(getter, 200);
				}
			}
		}

        if (error.error.code == -2) {
            MFT.AppSettingsController.showErrorSendMailPopup();
        }
	},

	/*
	 * Called when the app has become active or non-active.
	 */
	onAppStatusChanged: function(appBecameActive) {
		this.set("isInForeground", appBecameActive);

		if (FFW.isIOS) {
			// on iOS we have to close the network connection
			// when going to background
			if (!appBecameActive) {
				this.disconnect();
				FFW.RaterRPC.disconnect();
			} else {
				self = this;
				setTimeout(function() {
					self.connect();
					FFW.RaterRPC.connect();
				}, 10);
			}
		}
	},

	/*
	 * handle RPC notifications here
	 * for Backend component full screen setting change should be handled here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.Backend.onRPCNotification");
		this._super();

		if (notification.method == this.appStatusChangedProperty) {
			this.onAppStatusChanged(notification.params.appBecameActive);
		} else if (notification.method == this.navigationEnabledChangedProperty) {
			this.set('isNavigationEnabled', notification.params.hasMaps);
		} else if (notification.method == this.vehicleModelChangedProperty) {
			this.set('vehicleModel', notification.params.vehicle);
		}
	},
	
	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.Backend.onRPCRequest");
		this._super();

		if (request.method == "BackendClient.onFullScreenChanged") {
			this.resizeVideo = true;
			this.FullScreenRequestId = request.id;
			Em.Logger.log("resizeVideo = " + this.resizeVideo);
			Em.Logger.log("FullScreenRequestId = " + this.FullScreenRequestId);
			this.set("isFullScreen", request.params.isFullScreen);
		}
	},
	
	/*
	 * send back top, left, scale data when full screen mode changes 
 	 */	
	sendFullScreenResponse: function(topShift, leftShift, scale){
		Em.Logger.log("FFW.Backend.sendFullScreenResponse");
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.FullScreenRequestId,
			"result":	{
				"top": topShift,
				"left": leftShift,
				"scale": scale
			}
		};
		this.client.send(JSONMessage);
	},
	
	
	/*
     * request isFirstStart from Backend RPC
	 */
	getIsFirstStart: function() {
		this.isFirstStartRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.isFirstStartRequestId,
			"method":	"Backend.isFirstStart"
		};
		this.client.send(JSONMessage);
	},

	/*
     * request isFullScreen from Backend RPC
	 */
	getIsFullScreen: function() {
		this.isFullScreenRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.isFullScreenRequestId,
			"method":	"Backend.isFullScreen"
		};
		this.client.send(JSONMessage);
	},

    setFullScreen: function() {
        this.setFullScreenRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.setFullScreenRequestId,
            "method":	"Backend.setFullScreen",
            "params": {
                "isFullScreen": this.isFullScreen
            }
        };
        this.client.send(JSONMessage);
    },
	
	/*
     * request window size from Backend RPC
	 */
	getWindowSize: function() {
		this.getWindowSizeRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getWindowSizeRequestId,
			"method":	"Backend.getWindowSize"
		};
		this.client.send(JSONMessage);
	},
	
	/*
     * request window density from Backend RPC
	 */
	getWindowDensity: function() {
		this.getWindowDensityRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getWindowDensityRequestId,
			"method":	"Backend.getWindowDensity"
		};
		this.client.send(JSONMessage);
	},
	
	/*
     * request window density from Backend RPC
	 */
	getOSInfo: function() {
		this.getOSInfoRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getOSInfoRequestId,
			"method":	"Backend.getOSInfo"
		};
		this.client.send(JSONMessage);
	},

	getVehicleModel: function() {
		this.getVehicleModelRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getVehicleModelRequestId,
			"method":	"Backend.getVehicleModel"
		};
		this.client.send(JSONMessage);
	},

	getIsNavigationEnabled: function() {
		this.getIsNavigationEnabledRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getIsNavigationEnabledRequestId,
			"method":	"Backend.hasMaps"
		};
		this.client.send(JSONMessage);
	},

    /** Open End User License Agreement */
    openEULA: function() {
        this.openEULARequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.openEULARequestId,
            "method":	"Backend.openEULA"
        };
        this.client.send(JSONMessage);
    },

    /** Open “compose new email” window */
    sendSupportEmail: function() {
        this.sendSupportEmailRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.sendSupportEmailRequestId,
            "method":	"Backend.sendSupportEmail"
        };
        this.client.send(JSONMessage);
    },

	/*
	 * Saves the current vehicle model by sending it to the native part.
	 */
	saveVehicleModel: function() {
		this.saveVehicleModelRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.saveVehicleModelRequestId,
			"method":	"Backend.setVehicleModel",
			"params": {
				"vehicle": this.vehicleModel
			}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * Saves the isNavigationEnabled flag by sending it to the native part.
	 */
	saveIsNavigationEnabled: function() {
		this.saveIsNavigationEnabledRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.saveIsNavigationEnabledRequestId,
			"method":	"Backend.setHasMaps",
			"params": {
				"hasMaps": this.isNavigationEnabled
			}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * Notifies the backend that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	notifyAppLoaded: function() {
		++this.appLoadedEventsNumber;
		
		if (this.appLoadedEventsNumber >= 2) {
			if (!FFW.isAndroid) {
				if (this.client.socket.readyState == this.client.socket.OPEN) {
					var JSONMessage = {
						"jsonrpc":	"2.0",
						"method":	"Backend.onAppLoaded"
					};
					this.client.send(JSONMessage);
				}
			}
			MFT.set('appReady',true);
		}
	}
})

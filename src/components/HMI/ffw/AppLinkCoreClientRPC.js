/*
 * Reference implementation of AppLinkCoreClient component.
 * 
 * Interface to get or set some essential information from OS.
 * AppLinkCoreClient responsible for handling the basic commands of non-graphical part
 * such as the registration of mobile apps, geting the list of devices and applications
 * and data transfer.
 * 
 * @author Andriy Melnik
 */

FFW.AppLinkCoreClient = FFW.RPCObserver.create({
		
	/*
	 *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "AppLinkCoreClient" }),

	getAppListRequestId: 						-1,
	getDeviceListRequestId: 					-1,
	activateAppRequestId: 						-1,
	onAppRegisteredSubscribeRequestId:			-1,
	onAppUnregisteredSubscribeRequestId:		-1,
	onAppRegisteredUnsubscribeRequestId:		-1,
	onAppUnregisteredUnsubscribeRequestId:		-1,

	// const
	onAppRegisteredNotification:		"AppLinkCore.OnAppRegistered",
	onAppUnregisteredNotification:		"AppLinkCore.OnAppUnregistered",
	onDeviceListUpdatedNotification:	"AppLinkCore.OnDeviceListUpdated",

	/*
   	 * init object
 	 */
	init: function() {
	},

	/*
   	 * connect to RPC bus
 	 */
	connect: function() {
		
		this.client.connect(this, 600);
	},

	/*
   	 * disconnect from RPC bus
 	 */
	disconnect: function() {
		this.client.disconnect();
	},

	/*
         * Client is registered - we can send request starting from this point of time
 	 */	
	onRPCRegistered: function () {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCRegistered");
		this._super();

		// subscribe to notifications
		this.onAppRegisteredSubscribeRequestId 		= this.client.subscribeToNotification(this.onAppRegisteredNotification);
		this.onAppUnregisteredSubscribeRequestId 	= this.client.subscribeToNotification(this.onAppUnregisteredNotification);
		this.onDeviceListUpdatedNotificationId		= this.client.subscribeToNotification(this.onDeviceListUpdatedNotification);
	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCUnregistered");
		this._super();

		// unsubscribe from notifications
		this.onAppRegusteredUnsubscribeRequestId 	= this.client.unsubscribeFromNotification(this.onAppRegisteredNotification);
		this.onAppUnregusteredUnsubscribeRequestId	= this.client.unsubscribeFromNotification(this.onAppUnregisteredNotification);
		this.onDeviceListUpdatedNotificationId		= this.client.unsubscribeFromNotification(this.onDeviceListUpdatedNotification);
	},

	/*
	 * Client disconnected.
	 */
	onRPCDisconnected: function() {
		if( MFT.ApplinkAppController ){
			MFT.ApplinkAppController.onApplinkDisconected();
		}
	},

	/*
	 * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onRPCResult: function(response) {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCResult");
		this._super();

		if (response.result.method == "AppLinkCore.GetAppListResponse")
		{
			if(MFT.States.info.active){
				MFT.ApplinkController.onGetAppList(response.result.appList);
			}
		}

		if (response.result.method == "AppLinkCore.GetDeviceListResponse")
		{
			if(MFT.States.info.active){
				MFT.ApplinkModel.onGetDeviceList(response.result);
			}
		}

		if (response.id == this.activateAppRequestId)
		{
			// 
		}
	 },

	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCError");
		this._super();
	},

	/*
	 * handle RPC notifications here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCNotification");
		this._super();

		if (notification.method == this.onAppRegisteredNotification)
		{
			MFT.ApplinkModel.onAppRegistered( notification.params.application );
			this.getAppList();
		}

		if (notification.method == this.onAppUnregisteredNotification)
		{	
			//  remove app from list
			MFT.ApplinkModel.onAppUnregistered( notification.params );
		}

		if (notification.method == this.onDeviceListUpdatedNotification)
		{
			MFT.ApplinkModel.onGetDeviceList(notification.params);
		}
	},

	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCRequest");
		this._super();
		
		// nothing to do, it is client
	},

	/*
	 * Request for list of avaliable applications
	 */
	getAppList: function() {
		this.getAppListRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getAppListRequestId,
			"method":	"AppLinkCore.GetAppList",
			"params":	{}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * send notification when DriverDistraction PopUp is visible
 	 */	
	OnVersionChanged: function( version ) {
		Em.Logger.log("FFW.AppLinkCore.OnVersionChanged");

		// send repsonse
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"method":	"AppLinkCore.OnVersionChanged",
			"params":	{"versionNumber":	version}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * Request for list of avaliable devices
	 */
	getDeviceList: function() {
		this.getDeviceListRequestId = this.client.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getDeviceListRequestId,
			"method":	"AppLinkCore.GetDeviceList",
			"params":	{}
		};
		this.client.send(JSONMessage);
	},

    /** Sending data from HMI for processing in ApplinkCore */
    SendData: function( data ){
    	Em.Logger.log("FFW.ALCore.SendData");

    	if(!data){
    		data = ["Data for sending from HMI to Mobile application."];
    	}

		// send request
		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"id"		: 	this.client.idStart,
			"method"	:	"AppLinkCore.SendData",
			"params"	:	{
				"data": 	data
			}
		};

		if( MFT.ApplinkModel.sendDataExtend ){
			JSONMessage.params["url"] = "ftp://ford-applink.luxoft.com";
			JSONMessage.params["timeout"] = 10000;
		}

		this.client.send(JSONMessage);
    },

	/*
	 * handle RPC requests here
 	 */	
	ActivateApp: function( appId ) {
		Em.Logger.log("FFW.ALCore.ActivateApp");

		// send request

		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"id"		: 	this.client.idStart,
			"method"	:	"AppLinkCore.ActivateApp",
			"params"	:	{
				"appName":	MFT.ApplinkController.getApplicationModel(appId).appName,
				"appId":	appId
			}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * handle RPC requests here
 	 */	
	DeactivateApp: function( appName, reason, appId ) {
		Em.Logger.log("FFW.ALCore.OnAppDeactivated");

		// send request

		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"method"	:	"AppLinkCore.OnAppDeactivated",
			"params"	:	{
				"appName":	appName,
				"appId":	appId,
				"reason":	reason
			}
		};
		this.client.send(JSONMessage);
	}

})
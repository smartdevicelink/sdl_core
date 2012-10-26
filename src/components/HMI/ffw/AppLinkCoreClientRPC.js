/*
 * Reference implementation of UIBackend component.
 * 
 * Interface to get or set some essential information from OS.
 * Since web application is not able to access some OS feature through WebKit
 * it is necessary to use communication with native code to get some or set values 
 * and execute some functions. 
 * 
 *
 * @author Andriy Melnik
 */

FFW.AppLinkCoreClient = FFW.RPCObserver.create({
		
	/*
	 *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "AppLinkCoreClient" }),

	getAppListRequestId: 					-1,
	activateAppRequestId: 					-1,
	onAppRegisteredSubscribeRequestId:			-1,
	onAppUnregisteredSubscribeRequestId:			-1,
	onAppRegisteredUnsubscribeRequestId:			-1,
	onAppUnregisteredUnsubscribeRequestId:			-1,

	// const
	onAppRegisteredNotification:		"AppLinkCore.OnAppRegistered",
	onAppUnregisteredNotification:		"AppLinkCore.OnAppUnregistered",

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
		this.onAppRegusteredSubscribeRequestId 			= this.client.subscribeToNotification(this.onAppRegisteredNotification);
		this.onAppUnregusteredSubscribeRequestId 		= this.client.subscribeToNotification(this.onAppUnregisteredNotification);
	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCUnregistered");
		this._super();

		// unsubscribe from notifications
		this.onAppRegusteredUnsubscribeRequestId 		= this.client.unsubscribeFromNotification(this.onAppRegisteredNotification);
		this.onAppUnregusteredUnsubscribeRequestId 		= this.client.unsubscribeFromNotification(this.onAppUnregisteredNotification);
	},

	/*
	 * Client disconnected.
	 */
	onRPCDisconnected: function() {

	},

	/*
	 * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onRPCResult: function(response) {
		Em.Logger.log("FFW.AppLinkCoreClientRPC.onRPCResult");
		this._super();

		if (response.id = this.getAppListRequestId)
		{	
			// 
		}

		if (response.id = this.activateAppRequestId)
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
			// add new app to the list
		}

		if (notification.method == this.onAppUnregisteredNotification)
		{	
			//  remove app from list
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
	 * unregister component is RPC bus
	 */
	getAppList: function() {
		this.getAppListRequestId = this.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getAppListRequestId,
			"method":	"AppLinkCore.GetAppList",
			"params":	{
			}
		};
		this.send(JSONMessage);
	},


	/*
	 * unregister component is RPC bus
	 */
	activateApp: function(name) {
		this.activateAppRequestId = this.generateId();

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.activateAppRequestId,
			"method":	"AppLinkCore.ActivateApp",
			"params":	{
				"name" : name
			}
		};
		this.send(JSONMessage);
	},
})
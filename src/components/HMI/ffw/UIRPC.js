/*
 * Reference implementation of UI component.
 * 
 * Interface to get or set some essential information from OS.
 * Since web application is not able to access some OS feature through WebKit
 * it is necessary to use communication with native code to get some or set values 
 * and execute some functions. 
 * 
 *
 * @author Andriy Melnik
 */

FFW.UI = FFW.RPCObserver.create({
		
	/*
	 *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "UI" }),

	/*
   	 * Default values for global properties
 	 */
	globalPropertiesDefault : {
		helpPrompt	: [ 
			{
				"text" 	: "Some text for help prompt",
				"type"	: "TEXT"
			}
		],
		
		timeoutPrompt	: [
			{
				"text" 	: "Some text for timeout prompt",
				"type"	: "TEXT"
			}
		]
	},

	/*
   	 * Global properties
 	 */
	globalProperties	: {
		helpPrompt	: null ,
		timeoutPrompt	: null
	},
	
	
	/*
   	 * init object
 	 */
	init: function() {
		// init global properties
		this.resetProperties();
	},

	/*
   	 * resetGlobalProperties
 	 */
	resetProperties: function(propertyName) {
		if (propertyName == "HELPPROMPT" || propertyName == "")
			this.globalProperties.helpPrompt 	= this.globalPropertiesDefault.helpPrompt;
	
		if (propertyName == "TIMEOUTPROMPT" || propertyName == "")
			this.globalProperties.timeoutPrompt = this.globalPropertiesDefault.timoutPrompt;
	},

	/*
   	 * connect to RPC bus
 	 */
	connect: function() {
		
		this.client.connect(this, 400);
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
		Em.Logger.log("FFW.UI.onRPCRegistered");
		this._super();
	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.UI.onRPCUnregistered");
		this._super();
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
		Em.Logger.log("FFW.UI.onRPCResult");
		this._super();
	 },

	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.UI.onRPCError");
		this._super();
	},

	/*
	 * handle RPC notifications here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.UI.onRPCNotification");
		this._super();
	},

	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.UI.onRPCRequest");
		this._super();

		if (request.method == "UI.Show") {

			MFT.AppModel.PlayList.items[0].set('field1', request.params.mainField1);
			MFT.AppModel.PlayList.items[0].set('field2', request.params.mainField2);
			MFT.AppModel.PlayList.items[0].set('mediaClock', request.params.mediaClock);
			MFT.AppModel.PlayList.items[0].set('mediaTrack', request.params.mediaTrack);

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result":	{
					"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
				}
			};
			this.client.send(JSONMessage);
		}
		
		if (request.method == "UI.Alert") {

			MFT.UIPopUp.receiveMessage(request.params.AlertText1, request.paras.AlertText2, request.params.duration, request.params.playTone);

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result":	{
					"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
				}
			};
			this.client.send(JSONMessage);
		}

		if (request.method == "UI.SetGlobalProperties") {

			MFT.TTSPopUp.receiveMessage("Set global properties");

			// TODO: please process as array 
			this.globalProperties.set('helpPrompt', request.params.helpPrompt);
			// TODO: please process as array 
			this.globalProperties.set('timeoutPrompt', request.params.timeoutPrompt);

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result":	{
					"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
				}
			};
			this.client.send(JSONMessage);
		}
		
		if (request.method == "UI.ResetGlobalProperties") {
			
			// reset all requested properties
			for (var i=0;i<request.params.length;i++)
			{
			        this.resetProperties(reuqest.params[i]);
				MFT.TTSPopUp.receiveMessage("Reset property: " + reuqest.params[i]);
			}

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result":	{
					"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
				}
			};
			this.client.send(JSONMessage);
		}
	},
	
	/*
	 * handle RPC requests here
 	 */	
	onRPCActivateApp: function() {
		Em.Logger.log("FFW.UI.onRPCActivateApp");

		// send repsonse
		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"id"		: 	this.client.idStart,
			"method"	:	"AppLinkCore.activateApp",
			"params"	:	{
				[
					"appName"	:	MFT.AppModel.PlayList.items[0].appName
				]
			}
		};
		this.client.send(JSONMessage);
	}
})
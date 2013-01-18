/*
 * Reference implementation of VR component.
 * 
 * Interface to get or set some essential information
 * sent from ApplinkCore. VR is responsible for receiving voice commands
 * choosed by user and sending notification of chosen commands to ApplinkCore.
 *
 * @author Andriy Melnik
 */

FFW.VR = FFW.RPCObserver.create({
		
	/*
     *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "VR" }),
	
	
	/*
   	 * connect to RPC bus
 	 */
	connect: function() {
		this.client.connect(this, 500);
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
		Em.Logger.log("FFW.VR.onRPCRegistered");
		this._super();
	},

	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.VR.onRPCUnregistered");
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
		Em.Logger.log("FFW.VR.onRPCResult");
		this._super();
	 },

	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.VR.onRPCError");
		this._super();
	},

	/*
	 * handle RPC notifications here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.VR.onRPCNotification");
		this._super();
	},

	/*
	 * send notification when command was triggered
 	 */	
	onCommand: function(element) {
		Em.Logger.log("FFW.VR.onCommand");

		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"method"	:	"VR.OnCommand",
			"params"	:	{"commandId":element.commandId, }
		};
		this.client.send(JSONMessage);

        MFT.VRPopUp.set('VRActive', false);
	},

	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.VR.onRPCRequest");
		this._super();

		switch (request.method) {
			case "VR.AddCommand":{
				
				MFT.ApplinkModel.addCommandVR(request.params);

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result"	: 	{
						"resultCode" 	:  "SUCCESS", //  type (enum) from AppLink protocol
						"method" 	:  request.method + "Response"
					}
				};
				this.client.send(JSONMessage);

				break;
			}
			case "VR.DeleteCommand":{

				MFT.ApplinkModel.deleteCommandVR(request.params.cmdId);

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result"	: 	{
						"resultCode" 	:  "SUCCESS", //  type (enum) from AppLink protocol
						"method" 	:  request.method + "Response"
					}
				};
				this.client.send(JSONMessage);

				break;
			}
			case "VR.GetSupportedLanguages":{

				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result":	{
						"resultCode" : "SUCCESS", //  type (enum) from AppLink protocol
						"method" : "VR.GetSupportedLanguagesResponse",
						"languages" : MFT.ApplinkModel.applinkLanguagesList
					}
				};
				this.client.send(JSONMessage);

		    	break;
		    }
		    case "VR.GetLanguage":{

				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result":	{
						"resultCode" : "SUCCESS", //  type (enum) from AppLink protocol
						"method" : "VR.GetLanguageResponse",
						"language" : MFT.ApplinkModel.TTSVRLanguage
					}
				};
				this.client.send(JSONMessage);

		    	break;
		    }
			case "VR.GetCapabilities":{

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result"	:	{
						"capabilities": ["Text"],
						"method"	:	"VR.GetCapabilitiesResponse",
						"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
					}
				};
				this.client.send(JSONMessage);

				break;
			}
			case "VR.ChangeRegistration":{

				MFT.ApplinkModel.changeRegistrationTTSVR(request.params.language);

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result"	: 	{
						"resultCode" 	:  "SUCCESS", //  type (enum) from AppLink protocol
						"method" 	:  request.method + "Response"
					}
				};
				this.client.send(JSONMessage);

				break;
			}

			default:{
				//statements_def
				break;
			}
		}
			
	},

	/*
	 * send notification when command was triggered from VR
 	 */	
	onChoise: function(commandId) {
		Em.Logger.log("FFW.VR.PerformInteractionResponse");

		var JSONMessage = {
			"jsonrpc":	"2.0",
			"method":	"VR.OnChoise",
			"params":	{"choiceID":	commandId}
		};

		this.client.send(JSONMessage);
	},

	/*
	 * send notification when command was triggered
 	 */	
	onCommand: function(commandId, appId) {
		Em.Logger.log("FFW.VR.onCommand");

		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"method"	:	"VR.OnCommand",
			"params"	:	{
				"commandId":	commandId,
				"appId":		appId
			}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * Notifies if applink VR components language was changed
 	 */	
	OnLanguageChange: function( lang ) {
		Em.Logger.log("FFW.VR.OnLanguageChange");

		// send repsonse
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"method":	"VR.OnLanguageChange",
			"params":	{"language":	lang}
		};
		this.client.send(JSONMessage);
	}
})
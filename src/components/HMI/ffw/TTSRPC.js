/*
 * Reference implementation of TTS component.
 * 
 * TTS is responsible for playing sound data sent from SDLCore
 * to notify user about some events happened.
 *
 * @author Andriy Melnik
 */

FFW.TTS = FFW.RPCObserver.create({
		
	/*
     *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "TTS" }),
	
	
	/*
   	 * connect to RPC bus
 	 */
	connect: function() {
		this.client.connect(this, 300);
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
		Em.Logger.log("FFW.TTS.onRPCRegistered");
		this._super();
	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.TTS.onRPCUnregistered");
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
		Em.Logger.log("FFW.TTS.onRPCResult");
		this._super();
	 },
	
	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.TTS.onRPCError");
		this._super();
	},

	/*
	 * handle RPC notifications here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.TTS.onRPCNotification");
		this._super();
	},
	
	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.TTS.onRPCRequest");
		this._super();
		
		
		switch (request.method) {
		    case "TTS.Speak":{
		    	MFT.SDLModel.onPrompt(request.params.ttsChunks.splice(0, 1));

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result":	{
						"resultCode" : "SUCCESS", //  type (enum) from SDL protocol
						"method"   :    "TTS.SpeakResponse"
					}
				};
				this.client.send(JSONMessage);

		    	break;
		    }
			case "TTS.GetCapabilities":{

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result"	:	{
						"capabilities":["TEXT"],

						"resultCode" : "SUCCESS", //  type (enum) from SDL protocol
						"method" : "TTS.GetCapabilitiesResponse"
					}
				};
				this.client.send(JSONMessage);

		    	break;
			}
			case "TTS.GetSupportedLanguages":{

				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result":	{
						"resultCode" : "SUCCESS", //  type (enum) from SDL protocol
						"method" : "TTS.GetSupportedLanguagesResponse",
						"languages" : MFT.SDLModel.sdlLanguagesList
					}
				};
				this.client.send(JSONMessage);

		    	break;
		    }
		    case "TTS.GetLanguage":{

				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result":	{
						"resultCode" : "SUCCESS", //  type (enum) from SDL protocol
						"method" : "TTS.GetLanguageResponse",
						"language" : MFT.SDLModel.hmiTTSVRLanguage
					}
				};
				this.client.send(JSONMessage);

		    	break;
		    }
			case "TTS.ChangeRegistration":{

				MFT.SDLModel.changeRegistrationTTSVR(request.params.language);

				// send repsonse
				var JSONMessage = {
					"jsonrpc"	:	"2.0",
					"id"		: 	request.id,
					"result":	{
						"resultCode" : "SUCCESS", //  type (enum) from SDL protocol
						"method"   :    "TTS.ChangeRegistrationResponse"
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
	 * Notifies if sdl TTS components language was changed
 	 */	
	OnLanguageChange: function( lang ) {
		Em.Logger.log("FFW.TTS.OnLanguageChange");

		// send repsonse
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"method":	"TTS.OnLanguageChange",
			"params":	{"language":	lang}
		};
		this.client.send(JSONMessage);
	}
})
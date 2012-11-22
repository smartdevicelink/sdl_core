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
	 *	id for request Perform Interaction 
 	 */
 	 performInteractionRequestId: -1,

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

 		// notify other components that UI is ready 
		//  main purpose is to nitofy ALCore
		this.onReady();
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

		var resultCode = null;

		if (request.method == "UI.Show") {

			MFT.ApplinkModel.showInfo.set('field1', request.params.mainField1);
			MFT.ApplinkModel.showInfo.set('field2', request.params.mainField2);
			MFT.ApplinkModel.showInfo.set('mediaClock', request.params.mediaClock);
			MFT.ApplinkModel.showInfo.set('field3', request.params.mediaTrack);
			MFT.ApplinkModel.showInfo.set('statusBar', request.params.statusBar);

			this.sendUIResult("SUCCESS", request.id, request.method);
		}
		
		if (request.method == "UI.Alert") {

			MFT.AlertPopUp.AlertActive(request.params.AlertText1, request.params.AlertText2, request.params.duration, request.params.playTone);

			this.sendUIResult("SUCCESS", request.id, request.method);
		}

		if (request.method == "UI.SetGlobalProperties") {

			MFT.TTSPopUp.receiveMessage("Set global properties");

			// TODO: please process as array 
			this.globalProperties.set('helpPrompt', request.params.helpPrompt);
			// TODO: please process as array 
			this.globalProperties.set('timeoutPrompt', request.params.timeoutPrompt);

			this.sendUIResult("SUCCESS", request.id, request.method);
		}
		
		if (request.method == "UI.ResetGlobalProperties") {
			
			// reset all requested properties
			for (var i=0;i<request.params.length;i++)
			{
			        this.resetProperties(reuqest.params[i]);
				MFT.TTSPopUp.receiveMessage("Reset property: " + reuqest.params[i]);
			}

			this.sendUIResult("SUCCESS", request.id, request.method);
		}

		if (request.method == "UI.AddCommand") {
			
			MFT.ApplinkMediaController.applinkAddCommand(request.params);

			this.sendUIResult("SUCCESS", request.id, request.method);

		}

		if (request.method == "UI.DeleteCommand") {
			
			MFT.ApplinkOptionsView.DeleteCommand(request.params.cmdId);

			this.sendUIResult("SUCCESS", request.id, request.method);
		}

		if (request.method == "UI.AddSubMenu") {
			
			MFT.ApplinkOptionsView.AddSubMenu(request.params.menuId, request.params.menuName);

			this.sendUIResult("SUCCESS", request.id, request.method);
		}

		if (request.method == "UI.DeleteSubMenu") {

			var resultCode =  MFT.ApplinkOptionsView.DeleteSubMenu(request.params.menuId);

			this.sendUIResult(resultCode, request.id, request.method);

		}

		if (request.method == "UI.CreateInteractionChoiceSet") {

			MFT.ApplinkModel.interactionChoises.push(request.params);

			this.sendUIResult("SUCCESS", request.id, request.method);
		}

		if (request.method == "UI.DeleteInteractionChoiceSet") {

			for(var val in MFT.ApplinkModel.interactionChoises){
				if(MFT.ApplinkModel.interactionChoises[val].interactionChoiceSetID == request.params.interactionChoiceSetID ){
					MFT.ApplinkModel.interactionChoises.splice(val, 1);
					break;
				}
			}

			this.sendUIResult("SUCCESS", request.id, request.method);

		}

		if (request.method == "UI.PerformInteraction") {

			this.performInteractionRequestId = request.id;

			MFT.ApplinkMediaController.turnOnApplinkPerform(request.params);

		}

		if (request.method == "UI.SetMediaClockTimer") {

			var resultCode = MFT.ApplinkMediaController.applinkSetMediaClockTimer(request.params);

			this.sendUIResult(resultCode, request.id, request.method);
		}

		if (request.method == "UI.OnAppActivated") {
			
		}

		if (request.method == "UI.GetCapabilities") {

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result"	:	{
					"capabilities":{
						"displayCapabilities"	: {
							"displayType":	"GEN2_8_DMA",
							"textFields":[{
									"name":			"mainField1",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								},
								{
									"name":			"mainField2",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								},
								{
									"name":			"statusBar",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								},
								{
									"name":			"mediaClock",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								},
								{
									"name":			"mediaTrack",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								},
								{
									"name":			"alertText1",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								},
								{
									"name":			"alertText2",
									"characterSet":	"TYPE2SET",
									"width": 1,
									"rows": 1
								}],
							},
							"mediaClockFormats":["CLOCK1", "CLOCK2", "CLOCKTEXT1", "CLOCKTEXT2", "CLOCKTEXT3"]
						},
						"hmiZoneCapabilities"	: ["FRONT","BACK"]
					},

					"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
				};

			this.client.send(JSONMessage);
		}
	},
	
	/*
	 * send response from onRPCRequest
 	 */	
	sendUIResult: function(resultCode, id, method) {
		
		if(resultCode){

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	id,
				"result":	{
					"resultCode" : resultCode, //  type (enum) from AppLink protocol
					"method" : method + "Response"
				}
			};
			this.client.send(JSONMessage);
		}
	},

	/*
	 * send notification when command was triggered
 	 */	
	onCommand: function(commandId) {
		Em.Logger.log("FFW.UI.onCommand");

		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"method"	:	"UI.OnCommand",
			"params"	:	{"commandId":commandId, }
		};
		this.client.send(JSONMessage);
	},

	/*
	 * send notification when command was triggered
 	 */	
	onChoosed: function(commandId) {
		Em.Logger.log("FFW.UI.PerformInteractionResponse");

		// send repsonse
		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"id"		: 	this.performInteractionRequestId,
			"result":	{
				"resultCode":		"SUCCESS",
				"choiceID":			commandId,
				"method":			"UI.PerformInteractionResponse"
			}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * notification that UI is ready
	 * AppLinkCore should be sunscribed to this notification
 	 */	
	onReady: function() {
		Em.Logger.log("FFW.UI.onReady");

		var JSONMessage = {
			"jsonrpc"	:	"2.0",
			"method"	:	"UI.OnReady"
		};
		this.client.send(JSONMessage);
	},

	/*
	 * send notification when DriverDistraction PopUp is visible
 	 */	
	onDriverDistraction: function(driverDistractionState) {
		Em.Logger.log("FFW.UI.DriverDistraction");

		// send repsonse
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"method":	"UI.OnDriverDistraction",
			"params":	{"state":	driverDistractionState}
		};
		this.client.send(JSONMessage);
	},

	/*
	 * Notifies if system context is changed
 	 */	
	OnSystemContext: function(systemContextValue) {
		Em.Logger.log("FFW.UI.OnSystemContext");

		// send repsonse
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"method":	"UI.OnSystemContext",
			"params":	{"systemContext":	systemContextValue}
		};
		this.client.send(JSONMessage);
	},
})
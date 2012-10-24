/*
 * Reference implementation of ButtonsBackend component.
 * 
 * Interface to get or set some essential information from OS.
 * Since web application is not able to access some OS feature through WebKit
 * it is necessary to use communication with native code to get some or set values 
 * and execute some functions. 
 * 
 *
 * @author Andriy Melnik
 */

FFW.Buttons = FFW.RPCObserver.create({
		
	/*
     *	access to basic RPC functionality
 	 */		
	 client:		FFW.RPCClient.create({ componentName: "Buttons" }),
	
	
	/*
   	 * connect to RPC bus
 	 */
	connect: function() {
		// to be removed
		//this.stubGetValues();
		
		this.client.connect(this);
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
		Em.Logger.log("FFW.ButtonsBackend.onRPCRegistered");
		this._super();
	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.ButtonsBackend.onRPCUnregistered");
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
		Em.Logger.log("FFW.ButtonsBackend.onRPCResult");
		this._super();
	 },
	
	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.ButtonsBackend.onRPCError");
		this._super();
	},

	/*
	 * handle RPC notifications here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.ButtonsBackend.onRPCNotification");
		this._super();
	},
	
	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.ButtonsBackend.onRPCRequest");
		this._super();

		if (request.method == "Buttons.GetCapabilities") {

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result"	:	{
					"capabilities":{
						"name"					: "1btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "2btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "3btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "4btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "5btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "6btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "7btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "8btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "9btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "0btn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "Okbtn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "Upbtn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "Downbtn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "Leftbtn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},
					"capabilities":{
						"name"					: "Rightbtn",
						"shortPressAvailable"	: true,
						"longPressAvailable"	: true,
						"upDownAvailable"		: true,
					},

					"resultCode" : "SUCCESS" //  type (enum) from AppLink protocol
				}
			};
			this.client.send(JSONMessage);
		}
	},

	/*
	 * Notifies the ButtonsRPC that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	buttonPressed: function(id, type) {

		if (this.client.socket.readyState == this.client.socket.OPEN) {
			var JSONMessage = {
				"jsonrpc" :	"2.0",
				"method"  :	"Buttons.onButtonPress",
				"params"  :	{
					"name"	  : id,
					"mode"    : type
				}
			};

			this.client.send(JSONMessage);
		}
		
	},

	/*
	 * Notifies the ButtonsRPC that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	buttonEvent: function(id, type) {

		if (this.client.socket.readyState == this.client.socket.OPEN) {
			var JSONMessage = {
				"jsonrpc" :	"2.0",
				"method"  :	"Buttons.onButtonEvent",
				"params"  :	{
					"name"	  : id,
					"mode"    : type
				}
			};

			this.client.send(JSONMessage);
		}
		
	}
})
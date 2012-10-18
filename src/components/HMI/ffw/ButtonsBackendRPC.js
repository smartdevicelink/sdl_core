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

FFW.ButtonsBackend = FFW.RPCObserver.create({
		
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
/*
		if (request.method == "ButtonsBackendClient.onFullScreenChanged") {
			this.resizeVideo = true;
			this.FullScreenRequestId = request.id;
			Em.Logger.log("resizeVideo = " + this.resizeVideo);
			Em.Logger.log("FullScreenRequestId = " + this.FullScreenRequestId);
			this.set("isFullScreen", request.params.isFullScreen);
		}
*/
	},

	/*
	 * Notifies the ButtonsBackend that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	buttonPressed: function(id, type) {

		if (this.client.socket.readyState == this.client.socket.OPEN) {
			var JSONMessage = {
				"jsonrpc" :	"2.0",
				"method"  :	"Buttons.onButtonEvent",
				"params"  :	{
				"name"	  : id,
				"mode"    : type
				}
			};

			//this.client.send(JSONMessage);
			console.log(JSONMessage);

		}
		
	}
})
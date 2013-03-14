/*
 * Reference implementation of Buttons component.
 * 
 *  Buttons sends to SDLCore press and hold events of
 * soft buttons, presets and some hard keys. 
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
		
		this.client.connect(this, 200);
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
		Em.Logger.log("FFW.Buttons.onRPCRegistered");
		this._super();
	},
	
	/*
     * Client is unregistered - no more requests
 	 */	
	onRPCUnregistered: function () {
		Em.Logger.log("FFW.Buttons.onRPCUnregistered");
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
		Em.Logger.log("FFW.Buttons.onRPCResult");
		this._super();
	 },
	
	/*
	 * handle RPC erros here
 	 */	
	onRPCError: function(error) {
		Em.Logger.log("FFW.Buttons.onRPCError");
		this._super();
	},

	/*
	 * handle RPC notifications here 
 	 */	
	onRPCNotification: function(notification) {
		Em.Logger.log("FFW.Buttons.onRPCNotification");
		this._super();
	},
	
	/*
	 * handle RPC requests here
 	 */	
	onRPCRequest: function(request) {
		Em.Logger.log("FFW.Buttons.onRPCRequest");
		this._super();

		if (request.method == "Buttons.GetCapabilities") {

			// send repsonse
			var JSONMessage = {
				"jsonrpc"	:	"2.0",
				"id"		: 	request.id,
				"result"	:	{
					"capabilities":[
						{
							"name"					: "PRESET_0",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_1",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_2",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_3",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_4",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_5",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_6",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_7",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_8",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "PRESET_9",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "OK",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "SEEKLEFT",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "SEEKRIGHT",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "TUNEUP",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						},
						{
							"name"					: "TUNEDOWN",
							"shortPressAvailable"	: true,
							"longPressAvailable"	: true,
							"upDownAvailable"		: true
						}
					],
					"presetBankCapabilities": {"onScreenPresetsAvailable": true},
					"method"	:	"Buttons.GetCapabilitiesResponse",
					"resultCode" : "SUCCESS" //  type (enum) from SDL protocol
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

		Em.Logger.log("FFW.Buttons.buttonPressed " + type);

		var JSONMessage = {
			"jsonrpc" :	"2.0",
			"method"  :	"Buttons.OnButtonPress",
			"params"  :	{
				"name"	  : id,
				"mode"    : type
			}
		};

		this.client.send(JSONMessage);
	},

	/*
	 * Notifies the ButtonsRPC that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	buttonEvent: function(id, type) {

		Em.Logger.log("FFW.Buttons.buttonEvent " + type);

		var JSONMessage = {
			"jsonrpc" :	"2.0",
			"method"  :	"Buttons.OnButtonEvent",
			"params"  :	{
				"name"	  : id,
				"mode"    : type
			}
		};

		this.client.send(JSONMessage);
	
	},

	/*
	 * Notifies the ButtonsRPC that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	buttonPressedCustom: function(name, type, softButtonID) {

		//Em.Logger.log("FFW.Buttons.buttonPressedCustom " + type);

		var JSONMessage = {
			"jsonrpc" :	"2.0",
			"method"  :	"Buttons.OnButtonPress",
			"params"  :	{
				"name":			name,
				"mode":			type,
				"customButtonID":	softButtonID
			}
		};

		this.client.send(JSONMessage);
	},

	/*
	 * Notifies the ButtonsRPC that the web is all set.
	 * Should be called twice: when the RPC link is up or failed to connect
	 * and all the views are rendered.
	 */
	buttonEventCustom: function(name, type, softButtonID) {

		//Em.Logger.log("FFW.Buttons.buttonEventCustom " + type);

		var JSONMessage = {
			"jsonrpc" :	"2.0",
			"method"  :	"Buttons.OnButtonEvent",
			"params"  :	{
				"name":			name,
				"mode":			type,
				"customButtonID":	softButtonID
			}
		};

		this.client.send(JSONMessage);
	
	}
})

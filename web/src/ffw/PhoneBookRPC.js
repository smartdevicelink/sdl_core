/*
 * Reference implementation of PhoneBook component.
 */

FFW.PhoneBookRPC = FFW.RPCObserver.create({
	
	/*
     *	access to basic RPC functionality
 	 */		
	client:		FFW.RPCClient.create({ componentName: "PhoneBookClient" }),
	
	/*
	 * identifiers for requests
	 */		
	 
	dialRequestId: 						-1,
	getContactsRequestId: 				-1,
	onCallIsEndedSubscribeRequestId:	-1,
	onCallIsEndedUnsubscribeRequestId:	-1,

	callIsEndedProperty:			"Phone.onCallIsEnded",
	connect: function() {
		if (FLAGS.ANDROID_PHONE_BOOK) {
			this.client.connect(this);
		}
	},

	disconnect: function() {
		this.client.disconnect();
		this.onCallIsEndedUnsubscribeRequestId = this.unsubscribeFromProperty(this.callIsEndedProperty);
	},
	
	onRPCRegistered: function () {
		this._super();
		this.onCallIsEndedSubscribeRequestId = this.subscribeToProperty(this.callIsEndedProperty);
		
	},

	/**
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

	/**
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
	
	/**
	  * Dial Number
	 */
	 dial:	function(obj){
		if(typeof obj == 'string'){
			var number = obj;
		}else{
			var number = obj.number;
		}
	 	if (FLAGS.ANDROID_PHONE_BOOK) {  
			this.dialRequestId = this.client.generateId();
			
			var JSONMessage = {
				"jsonrpc":	"2.0",
				"id": 		this.dialRequestId,
				"method":	"Phone.makeCall", 
				"params": 	{
					"phoneNumber": 	number
				}
			};
			this.client.send(JSONMessage);
		}else{
			Em.Logger.log('NUMBER: ' + number);
		}
	 },
	
	/**
	  * Get Contacts
	 */
	getContacts: function(range){
		this.getContactsRequestId = this.client.generateId();
		
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.getContactsRequestId,
			"method":	"Phone.getContacts", 
			"params": 	{
				"range": range
			}
		};
		this.client.send(JSONMessage);

	},
	
	/*
     * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onRPCResult: function(response) {
		this._super();
		if (response.id == this.getContactsRequestId){
			MFT.PhoneController.generateContactsList(response.result.contacts);
		} else if (response.id == this.onCallIsEndedSubscribeRequestId ||
					response.id == this.onCallIsEndedUnsubscribeRequestId) {
			this.checkRPCResponse(response);
		}
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
	 * handle RPC notifications here
	 * for Backend component full screen setting change should be handled here 
 	 */	
	onRPCNotification: function(notification) {
		this._super();
		if (notification.method == this.callIsEndedProperty) {
			this.onCallIsEnded(notification.params.callIsEnded);
		}
	},
	
	onCallIsEnded: function(callIsEnded){
		MFT.PhoneController.onEndCall();
	},
	
	/*********EMULATE ANDROID PHONE BOOK*********/
	
	getContacts_em: function(range){
		
		var i;
		
		for (i=0;i<=Contacts.length-1;i++)
		{
			
			if(Contacts[i].range === range)
			{
				
				this.onGetContactsResult(Contacts[i].list);
			
			}
			
		}
		
	},
	
	onGetContactsResult: function(data){
		MFT.PhoneController.generateContactsList(data);
	},
	
});
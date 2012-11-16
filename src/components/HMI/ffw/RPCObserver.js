/*
 * Observer of RPCClient
 * 
 * 
 *
 * @author Sergey Solovey
 */

FFW.RPCObserver = Em.Object.extend({
		
	onRPCRegistered: function () {
		// request necessary parameters from Backend
		Em.Logger.log("FFW.RPCObserver.Registered");
	},

	onRPCUnregistred: function () {
		Em.Logger.log("FFW.RPCObserver.onUnregistered");
	},

	onRPCDisconnected: function() {
		Em.Logger.log("FFW.RPCObserver.onRPCDisconnected");
	},

	/*
     * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onRPCResult: function(result) {

		// parse JSON string and set necessary properties
	},
	
	/*
 	 */	
	onRPCError: function(error) {

		// parse JSON string and set necessary properties
	},

	/*
 	 */	
	onRPCNotification: function(notification) {

		// parse JSON string and set necessary properties
	},
	
	onRPCRequest: function(request) {

		// parse JSON string and send back necessary data
	},
})
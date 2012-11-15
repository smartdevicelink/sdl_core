/*
 * WebSocket interface mockup
 *
 * Android WebView and browser don't support WebSocket
 * This class provides interface mockup to have seamless code for different platforms
 * JavaScriptFacade is used instead of real WebSocket connection
 *
 * @author Sergey Solovey
 */
 
FFW.WebSocket = Em.Object.extend({
	
	readyState: 	1,

	/*
     *	add observer of messages from Android
 	 */		
	init: function ()
	{	
		FFW.WebSocketSimulator.addClient(this);
	},
	
	/*
     *	remove observer of Android messages on connection close
 	 */	
	close: function() { 
		FFW.WebSocketSimulator.removeClient(this);
		// simulate connection is closed
		this.onclose(null);
	},

	/*
     *	send message to Android native code
	 *  Java Script facade is used instead of real WebSocket connection
 	 */	
	send: function(jsonMessage) { 
		FFW.WebSocketSimulator.send(this.clientName, jsonMessage);
	},
	
	// handlers for web socket events
	onclose: 	null,
	onerror: 	null, 
	onmessage: 	null, 
	onopen: 	null,
	
	/*
     * used for identification of sender and receiver 
	 * of particular message from/to Android
 	 */	
	 clientName: '',
});

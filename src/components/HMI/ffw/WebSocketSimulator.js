/*
 * WebSocket core simulation
 *
 * It is entry point for communication with Android
 * sen and receive methods are used for actual communication with Android native code
 *
 * @author Sergey Solovey
 */
FFW.WebSocketSimulator = Em.Object.create({
	
	/*
     *	send message via JavaScriptFacade interface of Android
 	 */		
	send: function(clientName, jsonMessage) { 
		if (FFW.isAndroid) {
			Android.send(clientName, jsonMessage);
		}
	},

	/*
     *	receive message from Android
	 *  to be called this way:
	 *  	FFW.WebSocketSimulator.receive(<RPCComponentName>, <jsonMessage>)
 	 */		
	receive: function(clientName, jsonMessage) { 
		var index = this.findByName(clientName);
		if (index != -1)
		{
			this.clients[index].onmessage({data: jsonMessage});
		}
	},
	
	/*
     * add "WebSocket" to the pull
	 * later it will be used for receiving of messages from Android 
 	 */		
	addClient: function (client) {
		this.clients.push(client);
	},

	/*
     * remove "WebSocket" from the pull
	 * messages for specified client won't be observed anymore
 	 */		
	removeClient: function(client) {
		var index = this.findByName(client.clientName);
		if (index != -1)
		{
			this.clients.splice(index, 1);
		}
	},
	
	findByName: function(name) {
		for (var i=0;i<this.clients.length;i++)
			if (this.clients[i].clientName == name)
					return i;
		
		return -1;
	},
	
	/*
     *	pull of "Websocket"s sending and receiving messages
 	 */	
	clients: [
	]
});

/**
 * @name MFT.SettingsModel
 * 
 * @desc  Multi contour seat model CAN
 * 
 * @category model	
 * @filesource app/model/SettingsModelCAN.js	
 * @version		2.0
 *
 * @author	Melnik Andriy		
 */

MFT.SettingsModelCAN = MFT.SettingsData.create({
	
	/** Create socket connection */
	socket: 		null,
	
	init: function() {
		this.connect();
	},
	
	/*
     * url for message broker
 	 */		
	//url: 					"ws://localhost:8086", 	
	url:					"ws://Fordtest:8681",
	
	connect: function() {
		this.socket 			= (FLAGS.EMULATE_WS) ?  FFW.WebSocket.create({clientName: 'MCSClient'}) : new WebSocket(this.url, 'sample');
      
		var self = this;

		this.socket.onopen 		= function(evt) { self.onWSOpen(evt) };
		this.socket.onclose 	= function(evt) { self.onWSClose(evt) };
		this.socket.onmessage 	= function(evt) { self.onWSMessage(evt) };
		this.socket.onerror 	= function(evt) { self.onWSError(evt) };
	},
	
	/*
     * WebSocket connection is ready
	 * Now RPC component can be registered in message broker
 	 */	
	onWSOpen: function (evt) {
		Em.Logger.log("SettingsModelCAN.onWSOpen");
	},

	/*
     * when result is received from RPC component this function is called
	 * It is the propriate place to check results of reuqest execution
	 * Please use previously store reuqestID to determine to which request repsonse belongs to
 	 */	
	onWSMessage: function (evt) {		
		this.receive(evt.data);
	},
	
	/*
     * WebSocket connection is closed
	 * Please make sure that RPCComponent was dunregistered in advance
 	 */	
	 onWSClose: function (evt) {
		Em.Logger.log("SettingsModelCAN.onWSClose");
	},
  	
	/*
     * WebSocket connection errors handling
	 */
	onWSError: function (evt) {
		Em.Logger.log("SettingsModelCAN ERROR: ");
	},


	/** Send signal to websocket intreface */
	send: function( seat_valve_index, seat_command ) {
				
		var seat_name = this.indexToName(seat_valve_index);
		
		var message = JSON.stringify(
				[
					{
						'name': 'seat_valve_index',
						'value': seat_name
					},
					{
						'name': 'seat_command',
						'value': seat_command
					}
				]
		);
		
		Em.Logger.log('Message sent: ', message);
			
		if (this.socket && (this.socket.readyState == 1)) { // connection is open and communication is possible
			this.socket.send(message);
			if (FLAGS.EMULATE_WS) this.receive(this.makeResponse(message).data);
		}
	},
	
	/** Receive signal from websocket intreface */
	receive: function( response ) {
		Ember.Logger.log( 'Message received.', response );
		
		var data = JSON.parse( response );	
		
		// Activate MCS state
		if ( data.name == 'activate_screen') {
			MFT.States.goToState(data.value);
			
			return;
		}
		
		// Make selection
		if ( data.name == 'seat_valve_index') {
			MFT.MCSController.set('selectedIndex', this.nameToIndex(data.value) );
		} else {
			var i = this.nameToIndex(data.name);
			this.driverSeat[i-1].set( 'value', Math.round( data.value / 10 ) );
		}
	},
	
	nameToIndex: function (valveName) {
		switch (valveName) {
			case	"upper_bolster": 
					return 4;
					
			case	"lower_bolster": 
					return 5;
					
			case	"lower_lumbar": 
					return 3;
					
			case	"middle_lumbar":
					return 2;
					
			case	"upper_lumbar":
					return 1;
					
			case 	"lumbar_massage":
					return 6;

			case 	"cusion_massage":
					return 7;
					
			case	"off":
					return 0;
		}
	},

	indexToName: function (valveIndex) {
		switch(valveIndex)
		{
			case 1: return 'upper_lumbar';
			case 2: return 'middle_lumbar';
			case 3: return 'lower_lumbar';
			case 4: return 'upper_bolster';
			case 5: return 'lower_bolster';
			case 6: return 'lumbar_massage';
			case 7: return 'cusion_massage';
		}
	},

	makeResponse: function( message ) {
		
		var data = JSON.parse( message ),
			value, i;		
		
		/** Get current value from model */
		i = this.nameToIndex(data[0].value);
		value = this.driverSeat[i-1].value * 10;
		
		/** Change value */
		switch ( data[1].value ) {
			case 'off':
				
				//value = 0;
				
				break;
				
			case 'increase':
			
				if ( value <= 90 ) {
					value += 10;
				}
				
				break;
				
			case 'decrease':
			
				if ( value >= 10 ) {
					value -= 10;
				}
				
				break;
		}
		
		var result = {
			data: JSON.stringify( {
					'name': data[0].value,
					'value': value
			})
		};
	
		/** Return json message */
		return result;
	},
	
	
	/**
	 * Function for call increase method of appropriate adjust
	 * Handler for plus button that increases indicators value
	 */
	onIncrease: function( index ){
		this.send( index, 'increase' );
	},

	/**
	 * Function for call decrease method of appropriate adjust
	 * Handler for minus button that decreases indicators value
	 */ 
	onDecrease: function( index ){
		this.send( index, 'decrease' );
	},
	
	/**
	 * Function for call increase method of appropriate adjust
	 * Handler for plus button that increases indicators value
	 */
	changeMassageValue: function( index , value, seat ){
		this.get(seat + 'Seat')[index-1].set('value', value);
	},
});
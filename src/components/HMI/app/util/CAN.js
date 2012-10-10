MFT.CAN = {
	
	send: function(property, model, val) {
		/**
		 * TODOsend API call to CAN
		 */
		 
		 var response;
		 
		 response = val;
		 
		 response = {
		 	'driverTemp'	: val,
		 	'passengerTemp' : val
		 };
		 
		 for( var item in response ) {
		 	 model[property].set('value', response[item]);
		 }
		 
		 //model[property].set('value',val);
	},
	
	test: function(property, value){
				
		MFT.ClimateController.model[property].set('value', Number(value));
	
	}
};
FFW.RC = FFW.RPCObserver.create({

	client: FFW.RPCClient.create({
		componentName: "RC"
	}),

	errorResponsePull: {},

	connect: function(){
		this.client.connect(this, 900);
	},
	disconnect: function() {
		this.onRPCUnregistered();
		this.client.disconnect();
	},
	onRPCRegistered: function(){
		Em.Logger.log("FFW.RC.onRPCRegistered");
		this._super();
	},
	onRPCUnregistered: function(){
		Em.Logger.log("FFW.RC.onRPCUnregistered");
		this._super();
	},
	onRPCDisconnected: function(){

	},
	onRPCResult: function(response){
		Em.Logger.log("FFW.RC.onRPCResult");
		this._super();
	},
	onRPCError: function(error){
		Em.Logger.log("FFW.RC.onRPCError");
		this._super();
	},
	onRPCNotification: function(notification){
		Em.Logger.log("FFW.RC.onRPCNotification");
		this._super();
	},
	onRPCRequest: function(request){
		this._super();
		Em.Logger.log("FFW.RC.onRPCRequest");

		if(request.method == "RC.GetInteriorVehicleDataCapabilities"){

		//Assuming One Radio and two climate controls
			var zonesAndModulesArray = [
				{
					"moduleZone" : { //Central Radio
						"col" : 0,
						"row" : 0,
						"level" : 0,
						"colspan" : 2, //Controlled by driver and/or passenger
						"rowspan" : 1,
						"levelspan" :1 ,
					},
					"moduleType": "RADIO" 
				},

				{
					"moduleZone" : { //Driver side climate control
						"col" : 0,
						"row" : 0,
						"level" : 0,
						"colspan" : 1, //Controlled by driver only
						"rowspan" : 1,
						"levelspan" :1 ,
					},
					"moduleType": "CLIMATE"
				},

				{
					"moduleZone" : { //Passenger side climate control
						"col" : 1,
						"row" : 0,
						"level" : 0,
						"colspan" : 2, //Controlled by driver and/or passenger
						"rowspan" : 1,
						"levelspan" : 1,
					},
					"moduleType": "CLIMATE"
				}

			];

			var capabilitiesQuery = [];

			if(request.zone) { //zone specified in request
				if(request.zone.row == 0) {//front row 
					if (request.zone.column == 0) // driver side
						if(request.moduleTypes) //Module types specified in request
							for(i=0; i< request.moduleTypes.length; i++){
								if (request.moduleTypes[i]=="RADIO") {
									capabilitiesQuery.push(zonesAndModulesArray[0]);
								}
								else if (request.moduleTypes[i]=="CLIMATE") {
									capabilitiesQuery.push(zonesAndModulesArray[1]);
									capabilitiesQuery.push(zonesAndModulesArray[2]);
								}
							}
						else //no module specified
							capabilitiesQuery = zonesAndModulesArray; //driver has control of all modules.

					else if(request.zone.column == 1){ //Passenger side zone specified
						if(request.moduleTypes){
							for(i=0; i<request.moduleTypes.length; i++){
								if (request.moduleTypes[i]=="RADIO") {
									capabilitiesQuery.push(zonesAndModulesArray[0]);
								}
								else if (request.moduleTypes[i]=="CLIMATE") {
									capabilitiesQuery.push(zonesAndModulesArray[2]);
								}								
							}
						}
						else{
							capabilitiesQuery.push(zonesAndModulesArray[0]);
							capabilitiesQuery.push(zonesAndModulesArray[2]);
						}
					}
				}
			}
			else  { //no zone specifired
				if(request.moduleTypes){
					for(i=0; i<request.moduleTypes.length; i++){
						if(request.moduleTypes[i] == "RADIO")
							capabilitiesQuery.push(zonesAndModulesArray[0]);
						else if (request.moduleTypes[i]=="CLIMATE"){
							capabilitiesQuery.push(zonesAndModulesArray[1]);
							capabilitiesQuery.push(zonesAndModulesArray[2]);
						}
					}
				}
				else{
					capabilitiesQuery = zonesAndModulesArray;
				}
			}

			var JSONMessage = {
				"jsonrpc": "2.0",
				"id": request.id,
				"result": {
					"interiorVehicleDataCapabilities" : capabilitiesQuery,
					"code" : 0,
					"method": "RC.GetInteriorVehicleDataCapabilities"
				}
			}
			this.client.send(JSONMessage);
		}
		else if(request.method == RC.SetInteriorVehicleData){
			Em.Logger.log("FFW.onRPCRequest method: SetInteriorVehicleData")
			if(request.moduleData.moduleType=="CLIMATE"){
				Em.Logger.log("RC set Climate data");
				var currentFanSpeed = SDL.ClimateController.setClimate(request);

			var JSONMessage = {
				"jsonrpc": "2.0",
				"id": request.id,
				"result": {
					"moduleData" : {
						"moduleType" : "CLIMATE",
						"moduleZone" : {},
						"radioControlData" :{},
						"climateControlData" : {
							"fanSpeed" : currentFanSpeed
						}

					},
					"code" : 0,
					"method": "RC.SetInteriorVehicleData"
				}
			}
			this.client.send(JSONMessage);

			}
			else if(request.moduleData.moduleType=="RADIO"){
				Em.Logger.log("RC set RADIO data");
			}

		}

	},

	sendError: function(resultCode, id, method, message){
		Em.logger.log('FFW.'+method+"Response");

        if (resultCode) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "error": {
                    "code": resultCode, // type (enum) from SDL protocol
                    "message": message,
                    "data": {
                        "method": method
                    }
                }
            };
            this.client.send(JSONMessage);
        }
	}

});
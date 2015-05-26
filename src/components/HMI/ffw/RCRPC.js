FFW.RC = FFW.RPCObserver.create({

	client: FFW.RPCClient.create({
		componentName: "RC"
	}),

	subscribeClimateState: false,

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
		Em.Logger.log(JSON.stringify(request));

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
		else if(request.method == "RC.SetInteriorVehicleData"){
			var params = request.params;
			Em.Logger.log("FFW.onRPCRequest method: SetInteriorVehicleData");
			if(params.moduleData.moduleType=="CLIMATE"){
				var JSONMessage = {
					"jsonrpc": "2.0",
					"id": request.id,
					"result": {
						"moduleData" : {
							"moduleType" : "CLIMATE",
							"moduleZone" : {},
							"radioControlData" :{},
							"climateControlData" : {
								'fanSpeed' : 	SDL.ClimateController.model.currentFanSpeed,
								'currentTemp': SDL.ClimateController.model.currentTemp,
								'desiredTemp': SDL.ClimateController.model.desiredTemp,
								'temperatureUnit': SDL.ClimateController.model.temperatureUnit,
								'acEnable': SDL.ClimateController.model.acEnable,
								'recirculateAirEnable': SDL.ClimateController.model.recirculateAirEnable,
								'autoModeEnable': SDL.ClimateController.model.autoModeEnable,
								'defrostZone': SDL.ClimateController.model.defrostZone,
								'dualModeEnable': SDL.ClimateController.model.dualModeEnable
							}

						},
						"code" : 0,
						"method": "RC.SetInteriorVehicleData"
					}
				}

				if((params.moduleData.moduleZone.row == 0)&&(params.moduleData.moduleZone.col==0)){
					if(params.moduleData.climateControlData.desiredTemp){
						var desiredTemp = SDL.ClimateController.setTemp(request);
						JSONMessage.result.moduleData.climateControlData.desiredTemp;
						//this.client.send(JSONMessage);

					}
				}
				else if((params.moduleData.moduleZone.row == 0)&&(params.moduleData.moduleZone.col==1)){
					if(params.moduleData.climateControlData.desiredTemp){
						var desiredTemp = SDL.ClimateController.setPassengerTemp(request);
						JSONMessage.result.moduleData.climateControlData.desiredTemp;
						//this.client.send(JSONMessage);

					}
				}
				if(params.moduleData.climateControlData.fanSpeed){
					var currentFanSpeed = SDL.ClimateController.setClimate(request);
					JSONMessage.result.moduleData.climateControlData.fanSpeed = currentFanSpeed;
					//this.client.send(JSONMessage);
				}
				if(params.moduleData.climateControlData.acEnable){
					var acEnable = SDL.ClimateController.setAcEnable(request);
					JSONMessage.result.moduleData.climateControlData.acEnable = acEnable;
					//this.client.send(JSONMessage);
				}
				if(params.moduleData.climateControlData.recirculateAirEnable){
					var recirculateAirEnable = SDL.ClimateController.setRecirculateAirEnable(request);
					JSONMessage.result.moduleData.climateControlData.recirculateAirEnable = recirculateAirEnable;
					//this.client.send(JSONMessage);
				}
				if(params.moduleData.climateControlData.autoModeEnable){
					var autoModeEnable = SDL.ClimateController.setAutoModeEnable(request);
					JSONMessage.result.moduleData.climateControlData.autoModeEnable = autoModeEnable;
					//this.client.send(JSONMessage);
				}
				if(params.moduleData.climateControlData.defrostZone){
					var defrostZone = SDL.ClimateController.setDefrostZone(request);
					JSONMessage.result.moduleData.climateControlData.defrostZone = defrostZone;
					//this.client.send(JSONMessage);
				}
				if(params.moduleData.climateControlData.dualModeEnable){
					var dualModeEnable = SDL.ClimateController.setDualModeEnable(request);
					JSONMessage.result.moduleData.climateControlData.dualModeEnable = dualModeEnable;
					//this.client.send(JSONMessage);
				}

				this.client.send(JSONMessage);
				this.onInteriorVehicleDataNotification("CLIMATE");

			}
			else if(params.moduleData.moduleType=="RADIO"){
				Em.Logger.log("RC set RADIO data");
			}

		}
		else if(request.method=="RC.GetInteriorVehicleData"){
			var params = request.params;
			Em.Logger.log("RC.GetInteriorVehicleData");

			var moduleData = {
				'moduleType': null,
				'moduleZone': null,
				'radioControlData': null,
				'climateControlData': null
			}
			console.log(JSON.stringify(params));
			if(params.moduleDescription.moduleType=='CLIMATE'){

				moduleData.moduleType = 'CLIMATE';

				var moduleZone = {
					'col': 0,
					'row': 0,
					'level': 0,
					'colspan': 2,
					'rowspan': 0,
					'levelspan': 0
				}

				moduleData.moduleZone = moduleZone;

				moduleData.radioControlData = null;

				var climateControlData = {
					'fanSpeed' : 	SDL.ClimateController.model.currentFanSpeed,
					'currentTemp': SDL.ClimateController.model.currentTemp,
					'desiredTemp': SDL.ClimateController.model.desiredTemp,
					'temperatureUnit': SDL.ClimateController.model.temperatureUnit,
					'acEnable': SDL.ClimateController.model.acEnable,
					'recirculateAirEnable': SDL.ClimateController.model.recirculateAirEnable,
					'autoModeEnable': SDL.ClimateController.model.autoModeEnable,
					'defrostZone': SDL.ClimateController.model.defrostZone,
					'dualModeEnable': SDL.ClimateController.model.dualModeEnable
				}

				moduleData.climateControlData = climateControlData;

				if(params.subscribe){
					this.subscribeClimateState = true;
				}
				else{
					this.subscribeClimateState = false; //resets
				}
			}
			/*else if(params.moduleDescription.moduleType='RADIO'){
				moduleData.moduleZone = 'RADIO';
				var radioControlData = {
					'frequencyInteger': SDL.RadioController.model.frequencyInteger,
					'frequencyFraction': SDL.RadioController.model.frequencyFraction,
					'band': SDL.RadioController.model.band,
					'rdsData': SDL.RadioController.model.rdsData,
					'availableHDs': SDL.RadioController.model.availableHDs,
					'hdChannel': SDL.RadioController.model.hdChannel,
					'signalStrength': SDL.RadioController.model.signalStrength,
					'signalChangeThreshold': SDL.RadioController.model.signalChangeThreshold,
					'radioEnable': SDL.RadioController.model.radioEnable,
					'state': SDL.RadioController.model.state

				}
			}*/


			var JSONMessage = {
				"jsonrpc": "2.0",
				"id": request.id,
				"result": {
					"moduleData" : moduleData,
					"code" : 0,
					"method": "RC.GetInteriorVehicleData"
				}
			}
			this.client.send(JSONMessage);

		}

	},

	onInteriorVehicleDataNotification: function(module, zone){
		Em.Logger.log('FFW.onInteriorVehicleDataNotification');
		if(module=="CLIMATE"){

			var desiredTemp;
			if(zone.col==1){
				desiredTemp = SDL.ClimateController.model.passengerDesiredTemp;
			}
			else{
				desiredTemp = SDL.ClimateController.model.desiredTemp;
			}

			var JSONMessage = {
				"jsonrpc": "2.0",
				"method": "RC.OnInteriorVehicleData",
				"params": {
					"moduleData" : {
						"moduleType" : "CLIMATE",
						"moduleZone" : zone,
						"climateControlData" : {
							'fanSpeed' : 	SDL.ClimateController.model.currentFanSpeed,
							'currentTemp': SDL.ClimateController.model.currentTemp,
							'desiredTemp': desiredTemp,
							'temperatureUnit': SDL.ClimateController.model.temperatureUnit,
							'acEnable': SDL.ClimateController.model.acEnable,
							'recirculateAirEnable': SDL.ClimateController.model.recirculateAirEnable,
							'autoModeEnable': SDL.ClimateController.model.autoModeEnable,
							'defrostZone': SDL.ClimateController.model.defrostZone,
							'dualModeEnable': SDL.ClimateController.model.dualModeEnable
						}

					}
				}
			}
			this.client.send(JSONMessage);
		}

	},

	sendError: function(resultCode, id, method, message){
		Em.Logger.log('FFW.'+method+"Response");

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
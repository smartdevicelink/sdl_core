SDL.ClimateController = Em.Object.create({

	modelBinding: "SDL.ClimateControlModel",

	setClimate: function(request){
		Em.Logger.log("Climate controller set climate: " + JSON.stringify(request));
		this.model.setFanSpeed(request.params.moduleData.climateControlData.fanSpeed);
		//FFW.RC.onInteriorVehicleDataNotification("CLIMATE");
		return(this.model.currentFanSpeed);
	},
	setTemp: function(request){
		Em.Logger.log("Climate controller set temperature: " + JSON.stringify(request));
		this.model.setTemp(request.params.moduleData.climateControlData.desiredTemp);
		return(this.model.desiredTemp);
	},
	setAcEnable: function(request){
		this.model.setAcEnable(request.params.moduleData.climateControlData.acEnable);
		return(this.model.acEnable);
	},
	setRecirculateAirEnable: function(request){
		this.model.setRecirculateAirEnable(request.params.moduleData.climateControlData.recirculateAirEnable);
		return(this.model.recirculateAirEnable);	
	},
	setAutoModeEnable: function(request){
		this.model.setAutoModeEnable(request.params.moduleData.climateControlData.autoModeEnable);
		return(this.model.autoModeEnable);
	},
	setDefrostZone: function(request){
		this.model.setDefrostZone(request.params.moduleData.climateControlData.defrostZone);
		return(this.model.defrostZone);
	},
	setDualModeEnable: function(request){
		this.model.setDualModeEnable(request.params.moduleData.climateControlData.dualModeEnable);
		return(this.model.dualModeEnable);
	},
	setPassengerTemp: function(request){
		Em.Logger.log("Climate controller set  passenger temperature: " + JSON.stringify(request));
		this.model.setPassengerTemp(request.params.moduleData.climateControlData.desiredTemp);
		return(this.model.passengerDesiredTemp);
	}

});
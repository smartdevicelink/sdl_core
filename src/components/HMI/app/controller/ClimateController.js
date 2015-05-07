SDL.ClimateController = Em.Object.create({

	modelBinding: "SDL.ClimateControlModel",

	climateUp: function() {
		this.model.increaseSpeed();
	},

	climateDown: function() {
		this.model.decreaseSpeed();
	},
	setClimate: function(request){
		Em.Logger.log("Climate controller set climate: " + JSON.stringify(request));
		this.model.setSpeed(request.moduleData.climateControlData.fanSpeed);
		return(this.model.currentFanSpeed);
	},
	tempUp: function() {
		this.model.increaseTemp();
	},
	tempDown: function() {
		this.model.decreaseTemp();
	},
	setTemp: function(request){
		Em.Logger.log("Climate controller set temperature: " + JSON.stringify(request));
		this.model.setTemp(request.moduleData.climateControlData.desiredTemp);
		return(this.model.desiredTemp);
	}

});
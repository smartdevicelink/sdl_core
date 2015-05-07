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
	}

});
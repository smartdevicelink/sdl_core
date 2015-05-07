SDL.ClimateController = Em.Object.create({

	modelBinding: "SDL.ClimateControlModel",

	climateUp: function() {
		this.model.increaseSpeed();
	},

	climateDown: function() {
		this.model.decreaseSpeed();
	},
	setClimate: function(request){

		this.model.setSpeed(request.params.fanSpeed);
	}

});
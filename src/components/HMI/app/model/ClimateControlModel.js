SDL.ClimateControlModel = Em.Object.create({

	currentTemp: 72,
	desiredTemp: 72,
	currentFanSpeed: 0,

	increaseSpeed: function(){
		this.set('currentFanSpeed', this.currentFanSpeed+1);
		Em.Logger.log(this.currentFanSpeed);
	},

	decreaseSpeed: function(){
		this.set('currentFanSpeed', this.currentFanSpeed-1);
		Em.Logger.log(this.currentFanSpeed);
	},

	setSpeed: function(speed){
		this.set('currentFanSpeed', speed);
		Em.Logger.log(this.currentFanSpeed);
	},

	increaseTemp: function(){
		this.set('desiredTemp', this.desiredTemp+1);
		Em.Logger.log(this.desiredTemp);
	},

	decreaseTemp: function(){
		this.set('desiredTemp', this.desiredTemp-1);
		Em.Logger.log(this.desiredTemp);
	},

	setTemp: function(temp){
		this.set('desiredTemp', temp);
		Em.Logger.log(this.desiredTemp);
	},

})
SDL.ClimateControlModel = Em.Object.create({

	currentTemp: 72,
	desiredTemp: 72,
	currentFanSpeed: 0,
	temperatureUnit: 'FAHRENHEIT',
	acEnable: false,
	recirculateAirEnable: false,
	autoModeEnable: false,
	defrostZone: 'FRONT',
	dualModeEnable: false,
	autoModeEnableString: 'OFF',

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

	setAcEnable: function(state){
		this.set('acEnable', state);
		Em.Logger.log('acEnable: ' + this.acEnable);
	},

	setRecirculateAirEnable: function(state){
		this.set('recirculateAirEnable', state);
		Em.Logger.log('recirculateAirEnable: ' + this.recirculateAirEnable);
	},
	
	setAutoModeEnable: function(state){
		this.set('autoModeEnable', state);
		Em.Logger.log('autoModeEnable :' + this.autoModeEnable);
		if(state==false)
			autoModeEnableString="OFF";
		else
			autoModeEnableString="ON";
		Em.Logger.log('string: '+ autoModeEnableString);
	},

	toggleAutoMode: function(){
		Em.Logger.log("toggleAutoMode");
		if(this.autoModeEnable){
			this.set('autoModeEnable', false);
			this.set('autoModeEnableString', 'OFF');
		}
		else{
			this.set('autoModeEnable', true);
			this.set('autoModeEnableString', "ON");

		}
	},
	
	setDefrostZone: function(zone){
		this.set('defrostZone', zone);
		Em.Logger.log('defrostZone', this.defrostZone);
	},
	
	setDualModeEnable: function(state){
		this.set('dualModeEnable', state);
		Em.Logger.log('dualModeEnable: ' + this.dualModeEnable);
	}


})
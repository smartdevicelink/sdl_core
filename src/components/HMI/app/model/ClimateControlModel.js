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
	dualModeEnableString: 'OFF',
	passengerDesiredTemp: 70,
	recirculateAirEnableString: 'OFF',
	acEnableString: 'OFF',
	driverZone:{
		"col":0,
		"row":0,
		"level":0,
		"colspan":1,
		"rowspan":1,
		"levelspan":1
	},
	passengerZone:{
		"col":1,
		"row":0,
		"level":0,
		"colspan":1,
		"rowspan":1,
		"levelspan":1
	},
	fanSpeedUp: function(){
		this.set('currentFanSpeed', this.currentFanSpeed+1);
		Em.Logger.log(this.currentFanSpeed);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	fanSpeedDown: function(){
		this.set('currentFanSpeed', this.currentFanSpeed-1);
		Em.Logger.log(this.currentFanSpeed);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	setFanSpeed: function(speed){
		this.set('currentFanSpeed', speed);
		Em.Logger.log(this.currentFanSpeed);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	tempUp: function(){
		this.set('desiredTemp', this.desiredTemp+1);
		Em.Logger.log(this.desiredTemp);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	tempDown: function(){
		this.set('desiredTemp', this.desiredTemp-1);
		Em.Logger.log(this.desiredTemp);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
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
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	passengerTempUp: function(){
		this.set('passengerDesiredTemp', this.passengerDesiredTemp+1);
		Em.Logger.log(this.passengerDesiredTemp);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.passengerZone);
	},

	passengerTempDown: function(){
		this.set('passengerDesiredTemp', this.passengerDesiredTemp-1);
		Em.Logger.log(this.passengerDesiredTemp);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.passengerZone);
	},

	defrostFrontEnable: function(){
		Em.Logger.log("defrostFrontEnable");
		this.set('defrostZone', "FRONT");
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	defrostRearEnable: function(){
		Em.Logger.log("defrostRearEnable");
		this.set('defrostZone', "REAR");
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},	

	defrostAllEnable: function(){
		Em.Logger.log("defrostAllEnable");
		this.set('defrostZone', "ALL");
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},	

	toggleDualMode: function(){
		Em.Logger.log("Toggle Dual Mode");
		if(this.dualModeEnable){
			this.set('dualModeEnable', false);
			this.set('dualModeEnableString', 'OFF');
		}
		else{
			this.set('dualModeEnable', true);
			this.set('dualModeEnableString', "ON");
		}
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	toggleRecirculateAir: function(){
		Em.Logger.log("Toggle Recirculate Air");
		if(this.recirculateAirEnable){
			this.set('recirculateAirEnable', false);
			this.set('recirculateAirEnableString', 'OFF');
		}
		else{
			this.set('recirculateAirEnable', true);
			this.set('recirculateAirEnableString', "ON");
		}
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	toggleAcEnable: function(){
		Em.Logger.log("Toggle AC Enable");
		if(this.acEnable){
			this.set('acEnable', false);
			this.set('acEnableString', 'OFF');
		}
		else{
			this.set('acEnable', true);
			this.set('acEnableString', "ON");
		}
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},


	//Commands from mobile

	setTemp: function(temp){
		this.set('desiredTemp', temp);
		Em.Logger.log(this.desiredTemp);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	setAcEnable: function(state){
		this.set('acEnable', state);
		Em.Logger.log('acEnable: ' + this.acEnable);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},

	setRecirculateAirEnable: function(state){
		this.set('recirculateAirEnable', state);
		Em.Logger.log('recirculateAirEnable: ' + this.recirculateAirEnable);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},
	
	setAutoModeEnable: function(state){
		this.set('autoModeEnable', state);
		Em.Logger.log('autoModeEnable :' + this.autoModeEnable);
		if(state==false)
			autoModeEnableString="OFF";
		else
			autoModeEnableString="ON";
		Em.Logger.log('string: '+ autoModeEnableString);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},
	
	setDefrostZone: function(zone){
		this.set('defrostZone', zone);
		Em.Logger.log('defrostZone', this.defrostZone);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},
	
	setDualModeEnable: function(state){
		this.set('dualModeEnable', state);
		Em.Logger.log('dualModeEnable: ' + this.dualModeEnable);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.driverZone);
	},
	setPassengerTemp: function(temp){
		this.set('passengerDesiredTemp', temp);
		Em.Logger.log(this.passengerDesiredTemp);
		FFW.RC.onInteriorVehicleDataNotification("CLIMATE", this.passengerZone);
	},


})
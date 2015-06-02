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
	radioZone:{
		"col":0,
		"row":0,
		"level":0,
		"colspan":2,
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

	frequencyDisplay: "99.5",
	frequencyFM: 99.5,
	frequencyAM: 1000,
	frequencyInteger: 99,
	frequencyFraction: 5,
	band: 'FM',
	tuneUp: function(){
		if(this.band=='FM'){
			this.set('frequencyFM', this.frequencyFM+0.2);
			if(this.frequencyFM>108.1)
				this.set('frequencyFM', 88.1)
			this.set('frequencyDisplay', this.frequencyFM.toFixed(1));
		}
		else {
			this.set('frequencyAM', this.frequencyAM+10);
			if(this.frequencyAM>1600)
				this.set('frequencyAM', 540);
			this.set('frequencyDisplay', this.frequencyAM.toFixed(0));
		}
		Em.Logger.log("Tune up: "+this.frequencyDisplay);
		FFW.RC.onInteriorVehicleDataNotification("RADIO", this.radioZone);
	},
	tuneDown: function(){
		if(this.band=='FM'){
			this.set('frequencyFM', this.frequencyFM-0.2);
			if(this.frequencyFM<88.1)
				this.set('frequencyFM', 108.1)
			this.set('frequencyDisplay', this.frequencyFM.toFixed(1));
		}
		else {
			this.set('frequencyAM', this.frequencyAM-10);
			if(this.frequencyAM<540)
				this.set('frequencyAM', 1600);
			this.set('frequencyDisplay', this.frequencyAM.toFixed(0));
		}

		Em.Logger.log("Tune Down: "+this.frequencyDisplay);
		FFW.RC.onInteriorVehicleDataNotification("RADIO", this.radioZone);
	},
	selectAM: function(){
		this.set('band', 'AM');
		Em.Logger.log('Select AM');
		this.set('frequencyDisplay', this.frequencyAM.toFixed(0));
		FFW.RC.onInteriorVehicleDataNotification("RADIO", this.radioZone);

	},
	selectFM: function(){
		this.set('band', 'FM');
		this.set('frequencyDisplay', this.frequencyFM.toFixed(1));
		Em.Logger.log('Select FM');
		FFW.RC.onInteriorVehicleDataNotification("RADIO", this.radioZone);
	},

	setFrequencyInteger:function(freqInt){
		Em.Logger.log("Set freq int: "+freqInt);
		if(this.band=='FM'){
			Em.Logger.log(this.frequencyFM + " " + this.frequencyFM.toFixed(0));
			var fraction = this.frequencyFM-Math.floor(this.frequencyFM);
			Em.Logger.log("fraction: "+fraction);
			var newFreq = freqInt+fraction;
			Em.Logger.log("newFreq: "+newFreq);
			this.set('frequencyFM', newFreq);
			this.set('frequencyDisplay', newFreq.toFixed(1));
		}
		else{
			this.set('frequencyAM', freqInt);
		}
		FFW.RC.onInteriorVehicleDataNotification("RADIO", this.radioZone);
		
	},
	setFrequencyFraction:function(fraction){
		Em.Logger.log("Set fraction: "+freqInt);
		if(this.band=='FM'){
			var integer = this.frequencyFM.toFixed(0);
			Em.Logger.log("integer: "+integer);
			var newFreq = integer+fraction;
			Em.Logger.log("newFreq: "+newFreq);
			this.set('frequencyFM', newFreq);
			this.set('frequencyDisplay', newFreq.toFixed(1));
		}
		FFW.RC.onInteriorVehicleDataNotification("RADIO", this.radioZone);
		
	}

})
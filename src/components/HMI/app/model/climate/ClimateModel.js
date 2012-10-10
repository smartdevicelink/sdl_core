/**
 * @name MFT.ClimateModel
 * 
 * @desc Climate logic model
 * 
 * @category	Model
 * @filesource	app/model/ClimateModel.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateModel = MFT.ClimateData.create({

	/** Init defaults values */
	init: function() {
		// Temperature
		this.driverTemp.toString();
		this.passangerTemp.toString();
		
		// Fan speed
		this.fan.set('value', 1);
		this.rearFan.set('value', 1);
		this.rearTemp.set('value', 5);
		
		// Airflow mode
		this.airflow.mode = this.airflow.items.panel;
				
		this.rearTemp.disable();
		this.rearFan.disable();
		
		this._super();
	},
	
	/** Toggle climate on/off mode */
	activate: function() {
		this.on.increase();
		
		if (this.on.value) {
			this.autoFan.on();
 			this.airflow.mode.on();
 			
 			this.windShield.set('value', 	this.on.offStates.windShield);
 			this.ac.set('value', 			this.on.offStates.ac);
 			this.maxAc.set('value', 		this.on.offStates.maxAc);
 			this.recirculation.set('value', this.on.offStates.recirculation);
 			
 			if ( this.maxAc.value ) {
 				this.dualMode.off();
	 			this.driverTemp.min();
 			}
		}
	},
	
	/** Increase driver temperature */
	increaseDriverTemp: function() {
		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
			return;
		}
		
		this.driverTemp.increase();
	},
	
	/** Decrease driver temperature */
	decreaseDriverTemp: function() {
		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
			return;
		}
		
		this.driverTemp.decrease();
	},
	
	/** Increase passanger temperature */
	increasePassangerTemp: function() {
		this.dualMode.on();
		
		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
			this.passangerTemp.set('value',this.on.offStates.passangerTemp);
			return;
		}
	
		this.passangerTemp.increase();
	},
	
	/** Decrease passanger temperature */
	decreasePassangerTemp: function() {
		this.dualMode.on();
		
		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
			this.passangerTemp.set('value',this.on.offStates.passangerTemp);
			return;
		}
		
		this.passangerTemp.decrease();
	},
	
	/** Toggle dual temperature mode */
 	toggleDualMode: function() {
 		this.dualMode.increase();
 		
 		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
			this.passangerTemp.set('value',this.on.offStates.passangerTemp);
		} 		
 	},
 	
 	/**
 	 * Enable Dual mod temperature
 	 *
 	 * @observes driverTemp, dualMode
 	 */
 	enableDualMode: function() {
 		if ( this.dualMode.value === 0) {
 			this.passangerTemp.set('value', this.driverTemp.value);
 		}
 	}.observes('driverTemp.value', 'dualMode.value'),
 	 
	/** Store current temperature value */
	storeMyTemp: function() {
		this.myTemp.set('value', this.driverTemp.value);
	},
	
	/** Activate my temperature feature */
	activateMyTemp: function() {
		if ( !this.on.value ) {
			this.activate();
		}
		
		this.driverTemp.set('value', this.myTemp.value);
	},
	
	/** Change the value of driver heated seat and off cooled seat */
	changeDriverHeatedSeat: function() {
		this.driverHeatedSeat.decrease();
		this.driverCooledSeat.off();
	},

	/** Change the value of driver cooled seat and off heated seat */	
	changeDriverCooledSeat: function() {
		this.driverCooledSeat.decrease();
		this.driverHeatedSeat.off();
	},
	
	/** Change the value of driver heated steer */	
	changeDriverHeatedSteer: function() {
		this.driverHeatedSteer.increase();
	},
	
	/** Change the value of passanger heated seat and off cooled seat */
	changePassangerHeatedSeat: function() {
		this.passangerHeatedSeat.decrease();
		this.passangerCooledSeat.off();
	},
	
	/** Change the value of passanger cooled seat and off heated seat */
	changePassangerCooledSeat: function() {
		this.passangerCooledSeat.decrease();
		this.passangerHeatedSeat.off();
	},
	
	/** Toggle windshield value and change dependencies */
	toggleWindShield: function() {
		if ( !this.on.value ) {
 			this.on.on();
			this.autoFan.on();
			this.ac.revert();
		}
		
 		// toggle windshield
 		this.windShield.increase();
 	},
 	
 	/** Toggle rear defrost value */
 	toggleRearDeforost: function() {
 		this.rearDeforost.increase();
 	},
 	
 	/** Activate auto value and change dependencies */
 	toggleAuto: function() {
 		// toggle auto
 		this.autoFan.on();
 		this.autoAirflow.on();
 		
 		// change dependencies
 		this.windShield.off();
 		this.ac.off();
 		this.maxAc.off();
 		this.recirculation.off();
 		this.fan.disable();
 		
 		this.setAirflowMode(this.autoAirflow);
 	},
 	
 	/** Toggle AC value and change dependencies */
 	toggleAc: function() {
 		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
		}
 		
 		// toggle ac
 		this.ac.increase();
 		
 		// change dependencies
 		if ( this.ac.value === 0 ) {
 			this.maxAc.off();
 		}
 	},
 	
 	/** Toggle Max AC value and change dependencies */
 	toggleMaxAc: function() {
 		if ( !this.on.value ) {
			this.on.on();
		}
 		
 		// toggle macAc
 		this.maxAc.increase();
 		
 		// change dependencies
 		//this.auto.off();
 		
 		if ( this.maxAc.value ) {
 			this.ac.on();
 			this.dualMode.off();
	 		this.driverTemp.min();
	 		this.autoFan.on();
	 		this.fan.disable();
 			this.recirculation.on();
 		} else {
 			this.recirculation.off();
 		}
 		this.setPanelAirflow();
 	},
 	
 	/** Toggle recirculation value and change dependencies */
 	toggleRecirculation: function() {
 		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
		}
 		
 		//toggle recirculation
 		if ( this.windShield.value === 0) {
 			this.recirculation.increase();
 			
 			// change dependencies
 			this.maxAc.off();
 		}	
 	},
 	
 	/** Increase fan speed */
 	increaseFanSpeed: function() {
 		if ( !this.on.value ) {
			this.on.on();
			this.setAirflowMode(this.autoAirflow);
 			this.fan.set('value',1);
		}
 		
 		this.fan.decrease();
 		
 		this.autoFan.off();
 		this.maxAc.off();
 	},
 	
 	/** Decrease fan speed */
 	decreaseFanSpeed: function() {
 		if ( !this.on.value ) {
			this.on.on();
			this.setAirflowMode(this.autoAirflow);
 			this.fan.set('value',1);
		}
		
 		this.fan.increase();
 		
 		//this.auto.off();
 		this.autoFan.off();
 		this.maxAc.off();
 	},
 	
 	/** Change airflow mode */
 	setAirflowMode: function(mode) {
 		if ( !this.on.value ) {
 			this.on.on();
			this.autoFan.on();
		}
				
 		if ( mode ) {
 			this.airflow.mode.off();
 			this.airflow.mode = mode;
 			mode.on();
 		}
 		
 		this.windShield.off();
 	},
 	
 	/** Toggle defrost airflow */
 	setDefrostAirflow: function() {
		this.setAirflowMode(this.airflow.items.defrost);
 	},
 	
 	/** Toggle panel airflow */
 	setPanelAirflow: function() {
		this.setAirflowMode(this.airflow.items.panel);
 	},
 	
 	/** Toggle feet airflow */
 	setFeetAirflow: function() {
 		this.setAirflowMode(this.airflow.items.feet);
 	},
 	
 	/** Toggle both airflow */
 	setBothAirflow: function() {
 		this.setAirflowMode(this.airflow.items.both);
 	},
 	
 	/* 
 	 * Toggle airflow mode when 'Wind shield' is off
 	 *
 	 * @observes windShield
 	 */
 	isWindShield: function() {
 		if( this.airflow.mode ) {
 			if( this.windShield.value ) {
 				this.airflow.mode.off();
 				
 				this.recirculation.off();
 				
 				if ( this.maxAc.value ) {
 					this.ac.off();
 				}
 				this.maxAc.off();
			} else {
				this.airflow.mode.on();
				
				this.recirculation.revert();
 				this.maxAc.revert();			
 				if ( this.maxAc.value ) {
 					this.ac.on();
 				}
			}
 		}
 	}.observes('this.windShield.value'),
 	
 	
 	toggleRearPower: function() {
 		this.rearOn.increase();
 	},
 	
 	toggleRearControl: function() {
 		if ( !this.rearOn.value ) {
 			this.rearOn.on();
 		}
 		this.rearControl.increase();
 	},
 	
 	/** Increase rear fan speed */
 	increaseRearFanSpeed: function() {
 		if ( !this.rearOn.value ) {
 			this.rearOn.on();
 			return;
 		}
 		
 		if ( this.rearControl.value ) {
 			this.rearControl.off();
 			return;
 		}
 		
 		this.rearFan.decrease();
 	},
 	
 	/** Decrease rear fan speed */
 	decreaseRearFanSpeed: function() {
 		if ( !this.rearOn.value ) {
 			this.rearOn.on();
 			return;
 		}
 		
 		if ( this.rearControl.value ) {
 			this.rearControl.off();
 			return;
 		}
 		
 		this.rearFan.increase();
 	},
 	
 	/** Increase rear temperature */
 	increaseRearTemp: function() {
 		if ( !this.rearOn.value ) {
 			this.rearOn.on();
 			return;
 		}
 		
 		if ( this.rearControl.value ) {
 			this.rearControl.off();
 			return;
 		}
 		
 		this.rearTemp.decrease();
 	},
 	
 	/** Decrease rear temperature */
 	decreaseRearTemp: function() {
 		if ( !this.rearOn.value ) {
 			this.rearOn.on();
 			return;
 		}
 		
 		if ( this.rearControl.value ) {
 			this.rearControl.off();
 			return;
 		}
 		
 		this.rearTemp.increase();
 	},
 	
 	/*
 	 * Enable full auto mode
 	 *
 	 * @observes autoFan, autoAirflow
 	 */
 	isFullAuto: function() {
 		if ( this.autoFan.value && this.autoAirflow.value ) {
 			this.auto.on();
 		} else {
 			this.auto.off();
 		}
 	}.observes('this.autoFan.value','this.autoAirflow.value'),
 	
 	/*
 	 * Toggle dependencies
 	 * by climate mode activation/deactivation
 	 *
 	 * @observes on
 	 */
 	isActivated: function() { 		
 		if( this.on.value ) {
 			this.driverTemp.enable();
 			this.passangerTemp.enable();
 		} else {
 			this.on.set('offStates', {
 				windShield: 	this.windShield.value,
 				dualMode:		this.dualMode.value,
 				driverTemp:		this.driverTemp.value,
 				passangerTemp:	this.passangerTemp.value,
 				ac:				this.ac.value,
 				maxAc:			this.maxAc.value,
 				recirculation:	this.recirculation.value
 			});
 			
 			this.rearOn.off();
 			this.driverTemp.disable();
 			this.dualMode.off();
 			this.passangerTemp.disable();
 			this.fan.disable();
 			this.windShield.off();
 			this.airflow.mode.off();
 			this.autoFan.off();
 			this.ac.off();
 			this.maxAc.off();
 			this.recirculation.off();
 		}
 	}.observes('this.on.value'),
 	
 	/*
 	 * Toggle dependencies
 	 * by rear climate mode activation/deactivation
 	 *
 	 * @observes rearOn
 	 */
 	isRearActivated: function() {
 		if( this.rearOn.value ) {
 			if ( !this.on.value ) {
 				this.activate();
			}
 			this.rearTemp.enable();
			this.rearFan.enable();
 		} else {
 			this.rearControl.off();
 			this.rearTemp.disable();
			this.rearFan.disable();
 		}
 	}.observes('this.rearOn.value'),
 	
 	/*
 	 * Toggle dependencies
 	 * by rear climate control activation/deactivation
 	 *
 	 * @observes rearControl
 	 */
 	isRearControlActivated: function() {
 		if( !this.rearControl.value ) {
 			this.rearTemp.enable();
			this.rearFan.enable()
 		} else {
 			this.rearTemp.disable();
			this.rearFan.disable();
 		}
 	}.observes('this.rearControl.value')
})
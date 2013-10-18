/**
 * @name MFT.ClimateModel
 * 
 * @desc General climate logic model
 * 
 * @category	Model
 * @filesource	app/model/ClimateModel.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateModel = Em.Object.extend( MFT.ClimateData, {

	/**
	 * @constructor
	 */
	init: function() {
    	this._super();
		
		this.reset();
	},
	
	/** Toggle climate on/off mode */
	activate: function() {
		this.on.increase();
		
		if (this.on.value) {
 			this.panelAirflow.set('value',  this._beforeOffStates.panelAirflow);
 			this.feetAirflow.set('value',   this._beforeOffStates.feetAirflow);
 			this.defrost.set('value',       this._beforeOffStates.defrost);
 			this.maxDefrost.set('value',	this._beforeOffStates.maxDefrost);
 			this.ac.set('value', 			this._beforeOffStates.ac);
 			this.maxAc.set('value', 		this._beforeOffStates.maxAc);
 			
 			if ( this.maxAc.value ) {
 				this.dualMode.off();
	 			this.driverTemp.min();
 			}
		}
	},
	
	/** Reset climate model to default state */
	reset: function() {
        
        this.on.set('value', 1);
        
        // reset temperature
        this.driverTemp.setDefault();
        this.dualMode.off();
        this.myTemp.set('value', 72);
        
        // reset seats and steer
        this.driverHeatedSeat.off();
        this.driverCooledSeat.off();
        this.driverHeatedSteer.off();
             
        // reset airflow
        this.defrostAirflow.off();
        this.panelAirflow.on();
        this.feetAirflow.off();
        
        // reset fan
        this.fan.set('value', 1);
        this.fan.enable();
        
        // reset controlls
        this.defrost.off();
        this.maxDefrost.off();
        this.rearDefrost.off();
        this.ac.off();
        this.maxAc.off();
        this.recirculation.off();
        
        // reset rear climate
        this.rearOn.off();
		this.rearFan.set('value', 1);
		this.rearTemp.set('value', 5);
		this.rearTemp.disable();
		this.rearFan.disable();
	},
	
	/** Check and enable overal climate state */
	checkOnState: function() {
    	if ( !this.on.value ) {
        	this.on.on();
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
			this.passangerTemp.set('value',this._beforeOffStates.passangerTemp);
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
			this.passangerTemp.set('value',this._beforeOffStates.passangerTemp);
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
			this.passangerTemp.set('value',this._beforeOffStates.passangerTemp);
		} 		
 	},
 	 
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
 	
 	/** Toggle defrost value and change dependencies */
 	toggleDefrost: function() {
        this.checkOnState();
		
 		this.defrost.increase();
 		
 		if ( this.get('isAuto') ) {
            //this.ac.off();
        }
 	},
 	
 	/** Toggle max defrost value and change dependencies */
	toggleMaxDefrost: function() {
		this.checkOnState();
		
		this.maxDefrost.increase();
		
		if ( this.maxDefrost.value ) {
		
		    this.panelAirflow.off();
 	        this.feetAirflow.off();
			
			this.dualMode.off();
		}
 	},
 	
 	/** Toggle rear defrost value */
 	toggleRearDefrost: function() {
 		this.rearDefrost.increase();
 	},
 	
 	/** Activate auto value and change dependencies */
 	toggleAuto: function() {
     	this.checkOnState();
 		
 		this.setAutoAirflow();

 		this.ac.off();
 		this.maxAc.off();
 		this.recirculation.off();
 		this.fan.disable();
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
 		if ( !this.ac.value ) {
 			this.maxAc.off();
 		}
 	},
 	
 	/** Toggle Max AC value and change dependencies */
 	toggleMaxAc: function() {
        this.checkOnState();
        
 		this.maxAc.increase();
 		
 		if ( this.maxAc.value ) {
 			this.panelAirflow.on();
 			this.feetAirflow.off();
 			this.defrost.off();
 			this.maxDefrost.off();
 		}
 	},
 	
 	/** Toggle recirculation value and change dependencies */
 	toggleRecirculation: function() {
 		if ( !this.on.value ) {
			this.on.on();
			this.toggleAuto();
		}

 		//toggle recirculation
 		if ( !this.defrost.value ) {
 			this.recirculation.increase();
 			
 			// change dependencies
 			this.maxAc.off();
 		}
 	},
 	
 	/** Increase fan speed */
 	increaseFanSpeed: function() {
 		if ( !this.on.value ) {
			this.on.on();
 			this.fan.set('value',1);
		}
 		
 		this.fan.decrease();
 	},
 	
 	/** Decrease fan speed */
 	decreaseFanSpeed: function() {
 		if ( !this.on.value ) {
			this.on.on();
 			this.fan.set('value',1);
		}
		
 		this.fan.increase();
 	},
 	
 	/** Toggle both airflow */
 	setDefrostAirflow: function() {
     	
 	},
 	
 	/** Toggle panel airflow */
 	setPanelAirflow: function() {
        this.checkOnState();
 	
		this.panelAirflow.increase();
 	},
 	
 	/** Toggle feet airflow */
 	setFeetAirflow: function() {
        
        this.checkOnState();
        
     	this.feetAirflow.increase();
 	},
 	
 	/** Toggle both airflow */
 	setBothAirflow: function() {
     	
 	},
 	
 	/** Toggle auto airflow */
 	setAutoAirflow: function() {
 	    this.panelAirflow.off();
 	    this.feetAirflow.off();
     	this.defrost.off();
 		this.maxDefrost.off();
 	},
 	
 	/** Toggle rear power value */
 	toggleRearPower: function() {
 		this.rearOn.increase();
 	},
 	
 	/** Toggle rear controll value */
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

    /**
 	 * Enable 'Dual' mode temperature
 	 */
 	enableDualMode: function() {
 		if ( !this.dualMode.value ) {
 			this.passangerTemp.set('value', this.driverTemp.value);
 		}
 	}.observes('driverTemp.value', 'dualMode.value'),
 	
 	/**
 	 * Toggle dependencies during climate mode activation/deactivation
 	 */
 	onActivate: function() { 		
 		if( this.on.value ) {
 			this.driverTemp.enable();
 			this.passangerTemp.enable();
 			//this.fan.enable();
 		} else {
 			this._beforeOffStates = {
 			    panelAirflow:   this.panelAirflow.value,
 			    feetAirflow:    this.feetAirflow.value,
 				maxDefrost:		this.maxDefrost.value,
 				defrost:        this.defrost.value,
 				dualMode:		this.dualMode.value,
 				driverTemp:		this.driverTemp.value,
 				passangerTemp:	this.passangerTemp.value,
 				ac:				this.ac.value,
 				maxAc:			this.maxAc.value
 				//recirculation:	this.recirculation.value
 			}
 			
 			this.rearOn.off();
 			this.driverTemp.disable();
 			this.passangerTemp.disable();
 			this.defrost.off();
 			this.maxDefrost.off();
 			this.feetAirflow.off();
 			this.panelAirflow.off();
 			this.ac.off();
 			this.maxAc.off();
 			this.recirculation.off();
 			this.fan.disable();
 			this.dualMode.off();
 		}
 	}.observes('this.on.value'),
 	
 	/**
 	 * Toggle dependencies during Max A/C on/off
 	 */
 	onMaxAc: function() {
 		if ( this.maxAc.value ) {
			this.ac.on();
			this.dualMode.off();
			this.driverTemp.min();
			this.fan.disable();
			this.recirculation.on();
 		} else {
            this.recirculation.off();
 		}	
 	}.observes('this.maxAc.value'),
	
	/**
 	 * Toggle dependencies during Defrost on/off
 	 */
	onDefrost: function() {
	
	   if ( this.defrost.value ) {
            this.recirculation.off();
            this.maxAc.off();
	   } else {
            this.maxDefrost.off();
            this.maxAc.revert();
	   }
	   
	}.observes('this.defrost.value'),
 	
    /**
 	 * Toggle dependencies durring Max Defrost on/off
 	 */
 	onMaxDefrost: function() {
 		if ( this.maxDefrost.value ) {
            
            // store previous states
            this._beforeMaxDefrost = {
                driverTemp:     this.driverTemp.value,
                passangerTemp:  this.passangerTemp.value,
                dual:           this.dualMode.value,
                fan:            this.fan.value,
                defrost:        this.defrost.value,
                autoFan:        this.fan.enabled
            }

			this.defrost.on();
			this.rearDefrost.on();
 			this.driverTemp.max();
	 		this.fan.set('value',7);
	 		this.ac.off();
	 		
	 		this.fan.disable();
	 		
 		} else {           
            
 		    if ( !this.maxAc.value  ) {
                
                this.maxAc.revert();
                
                this.driverTemp.set('value', this._beforeMaxDefrost.driverTemp);
                		
                if ( this._beforeMaxDefrost.dual ) {
                    this.dualMode.set('value', this._beforeMaxDefrost.dual);
                    this.passangerTemp.set('value', this._beforeMaxDefrost.passangerTemp);
                }
                
                if ( this._beforeMaxDefrost.autoFan ) {
                    this.fan.enable();
                }
                
                
                if ( this.get('isOn') && !this.get('isAuto') ) {
                    this.defrost.set( 'value', this._beforeMaxDefrost.defrost );
                }                
 		    }

 			this.fan.set('value',this._beforeMaxDefrost.fan);
						
			this.panelAirflow.revert();
 	        this.feetAirflow.revert(); 	        
 		}
 		
 	}.observes('this.maxDefrost.value'),
 	
 	/**
 	 * Toggle dependencies during rear climate mode activation/deactivation
 	 */
 	onRearActivated: function() {
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
 	
 	/**
 	 * Toggle dependencies during rear climate control activation/deactivation
 	 */
 	onRearControlActivated: function() {
 		if( !this.rearControl.value ) {
 			this.rearTemp.enable();
			this.rearFan.enable()
 		} else {
 			this.rearTemp.disable();
			this.rearFan.disable();
 		}
 	}.observes('this.rearControl.value')
})
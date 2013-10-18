/**
 * @name MFT.FusionClimateModel
 * 
 * @desc Climate Fusion logic model
 * 
 * @category	Model
 * @filesource	app/model/FusionClimateModel.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.FusionClimateModel = MFT.ClimateModel.create({
    
    /** Increase driver temperature */
	increaseDriverTemp: function() {
		this._super();
		
		this._afterTemperatureChange();
	},
	
    /** Decrease driver temperature */
	decreaseDriverTemp: function() {
		this._super();
		
		this._afterTemperatureChange();
	},
	
	/** Increase passanger temperature */
	increasePassangerTemp: function() {
		this._super();
		
		this._afterTemperatureChange();
	},
	
	/** Decrease passanger temperature */
	decreasePassangerTemp: function() {
		this._super();
		
		this._afterTemperatureChange();
	},
	
	/** Toggle panel airflow */
 	setPanelAirflow: function() {
		this._super();
		
		this.maxDefrost.off();
		
		if ( this.get('isAutoAirflow') ) {
            //this.ac.off();
            this.maxAc.off();
		}
 	},
 	
 	/** Toggle feet airflow */
 	setFeetAirflow: function() {
     	this._super();
     	
     	this.maxDefrost.off();
     	
     	if ( this.get('isAutoAirflow') ) {
     	    //this.ac.off();
            this.maxAc.off();
		}
 	},
	
    /**
 	 * Toggle dependencies during Max A/C on/off
 	 */
 	onMaxAc: function() {
 		 		
 		if ( this.maxAc.value ) {

            if ( !this.maxDefrost.value ) {
                this._beforeTemp = {
                    driverTemp:     this.driverTemp.value,
                    passangerTemp:  this.passangerTemp.value,
                    dual:           this.dualMode.value
                }
            }
            
            this._beforeMaxAc.maxDefrost = this.maxDefrost.value;
            
            this.ac.on();
			this.dualMode.off();
			this.driverTemp.min();
			this.fan.disable();
			this.recirculation.on();
            
 		} else {
 		 	
 		 	this.driverTemp.set('value', this._beforeTemp.driverTemp);
 		 	
 		 	if ( this._beforeTemp.dual ) {
 		 	    this.dualMode.set('value', this._beforeTemp.dual )
                this.passangerTemp.set('value', this._beforeTemp.passangerTemp);
 		 	}
 		 	
 		 	if ( this._beforeMaxAc.maxDefrost && !this.maxDefrost.value ) {
 		 	   this.toggleAuto();
 		 	   
 		 	   return;
 		 	}
 		 	            
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
	   }
	}.observes('this.defrost.value'),
 	
 	/**
 	 * Toggle dependencies durring Max Defrost on/off
 	 */
 	onMaxDefrost: function() {
 		if ( this.maxDefrost.value ) {
 		
            if ( !this.maxAc.value ) {
                this._beforeTemp = {
                    driverTemp:     this.driverTemp.value,
                    passangerTemp:  this.passangerTemp.value,
                    dual:           this.dualMode.value
                }
            }
            
            // store previous states
            this._beforeMaxDefrost = {
                fan:            this.fan.value,
                defrost:        this.defrost.value,
                autoFan:        this.fan.enabled,
                maxAc:          this.maxAc.value
            }

			this.defrost.on();
			this.rearDefrost.on();
 			this.driverTemp.max();
	 		this.fan.set('value',7);
	 		this.ac.off();
	 		
	 		this.fan.disable();
	 		
 		} else {
            
 		    if ( !this.maxAc.value  ) {
                                                
                this.driverTemp.set('value', this._beforeTemp.driverTemp);
                		
                if ( this._beforeTemp.dual ) {
                    this.dualMode.set('value', this._beforeTemp.dual);
                    this.passangerTemp.set('value', this._beforeTemp.passangerTemp);
                }
                
                // Regarding to AVA2-CRS-569
                if ( this._beforeMaxDefrost.maxAc ) {
                    this.toggleAuto();
                                        
                    return;
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
 	        
            this.recirculation.revert();
 		}
 		
 	}.observes('this.maxDefrost.value'),
 	
 	/**
	 * @private
	 *
	 * Disable Max defrost after temperature +/- buttons click
	 */
	_afterTemperatureChange: function() {
        if ( this.maxDefrost.value ) {
            this._beforeTemp.driverTemp = this.driverTemp.value;
            this._beforeTemp.passangerTemp = this.passangerTemp.value;

            this._beforeTemp.dual = this.dualMode.value;

            this.maxDefrost.off();
        }
	},
	
	/**
 	 * Determine overal climate state on/off
 	 *
 	 * @return {boolean}
 	 */
 	isOn: function() {
     	
     	if ( this.on.value ) {
         	return true;
     	} else {
         	return false;
     	}
     	
 	}.property('this.on.value'),
 	
 	/**
 	 * Determine fan state (auto)
 	 *
 	 * @return {boolean}
 	 */
 	isAutoFan: function() {
     	if ( this.get('isOn') && !this.get('fan.enabled') ) {
         	return true;
     	} else {
         	return false;
     	}
 	}.property(
        'this.isOn',
        'this.fan.enabled'
    ),
    
    /**
 	 * Determine overflow state (auto)
 	 *
 	 * @return {boolean}
 	 */
    isAutoAirflow: function() {
        if ( this.get('isOn') && !this.feetAirflow.value && !this.panelAirflow.value && !this.maxDefrost.value && !this.defrost.value) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.feetAirflow.value',
        'this.panelAirflow.value',
        'this.defrost.value',
        'this.maxDefrost.value',
        'this.isOn'
    ),
    
    /**
 	 * Determine fan and overflow state (full auto)
 	 *
 	 * @return {boolean}
 	 */
    isAuto: function() {
    
        if ( this.get('isAutoFan') && this.get('isAutoAirflow') ) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.isAutoFan',
        'this.isAutoAirflow'
    )
})
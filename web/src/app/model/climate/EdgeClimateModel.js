/**
 * @name MFT.EdgeClimateModel
 * 
 * @desc Climate Edge logic model
 * 
 * @category    Model
 * @filesource    app/model/EdgeClimateModel.js
 * @version        2.0
 *
 * @author        Artem Petrosyan
 */

MFT.EdgeClimateModel = MFT.ClimateModel.create({
    
    /** Toggle panel airflow */
    setDefrostAirflow: function() {
        this.checkOnState();
         
        this.panelAirflow.off();
        this.feetAirflow.on();
        this.defrostAirflow.on();

        this.defrost.off();
    },
    
    /** Toggle panel airflow */
    setPanelAirflow: function() {
        this.checkOnState();
        
        this.panelAirflow.on();
        this.feetAirflow.off();
        this.defrostAirflow.off();

        this.defrost.off();
    },
    
    /** Toggle feet airflow */
    setFeetAirflow: function() {
        this.checkOnState();
                
        this.feetAirflow.on();
        this.panelAirflow.off();
        this.defrostAirflow.off();
        
        this.defrost.off();
    },
    
    /** Toggle both airflow */
    setBothAirflow: function() {
        this.checkOnState();
        
        this.feetAirflow.on();
        this.panelAirflow.on();
        this.defrostAirflow.off();
        
        this.defrost.off();
    },
    
    /** Toggle auto airflow */
    setAutoAirflow: function() {
        this.checkOnState();
        
        this.feetAirflow.off();
        this.panelAirflow.off();
        this.defrostAirflow.off();
        
        this.defrost.off();
    },
    
    /** Toggle defrost value and change dependencies */
    toggleDefrost: function() {
         this.checkOnState();
         
         if ( this.defrost.value ) {
             this._revertAirflowStates();
         } else {
             this._saveAirflowStates();
         }
         
         this.defrost.increase();
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
     * Determine auto airflow state
     *
     * @return {boolean}
     */
    isAutoAirflow: function() {
        if ( this.get('isOn') && !this.feetAirflow.value && !this.panelAirflow.value && !this.defrostAirflow.value && !this.defrost.value) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.feetAirflow.value',
        'this.panelAirflow.value',
        'this.defrostAirflow.value',
        'this.defrost.value',
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
    ),

    /**
     * Determine defrost airflow state
     *
     * @return {boolean}
     */    
    isAirflowDefrost: function() {
        if ( this.feetAirflow.value && this.defrostAirflow.value ) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.feetAirflow.value',
        'this.defrostAirflow.value'
    ),

    /**
     * Determine feet airflow state
     *
     * @return {boolean}
     */
    isAirflowFeet: function() {
        if ( this.feetAirflow.value && !this.panelAirflow.value && !this.defrostAirflow.value) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.feetAirflow.value',
        'this.panelAirflow.value',
        'this.defrostAirflow.value'
    ),

    /**
     * Determine panel airflow state
     *
     * @return {boolean}
     */
    isAirflowPanel: function() {
        if ( !this.feetAirflow.value && this.panelAirflow.value ) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.feetAirflow.value',
        'this.panelAirflow.value'
    ),

    /**
     * Determine both airflow state
     *
     * @return {boolean}
     */    
    isAirflowBoth: function() {
        if ( this.feetAirflow.value && this.panelAirflow.value ) {
            return true;
        } else {
            return false;
        }
    }.property(
        'this.feetAirflow.value',
        'this.panelAirflow.value'
    ),
    
    /**
     * @private
     *
     * Save airflow state
     */
    _saveAirflowStates: function() {
    
        this._airflowStates = {
            defrost:      this.defrostAirflow.value,
            feet:         this.feetAirflow.value,
            panel:        this.panelAirflow.value,
            maxAc:        this.maxAc.value
        }
        
        this.feetAirflow.off();
        this.panelAirflow.off();
        this.defrostAirflow.off();
        
        this.maxAc.off();
        this.recirculation.off();
    },
    
    /**
     * @private
     *
     * Restore airflow state
     */
    _revertAirflowStates: function() {
        this.defrostAirflow.set('value',      this._airflowStates.defrost);
        this.feetAirflow.set('value',         this._airflowStates.feet);
        this.panelAirflow.set('value',        this._airflowStates.panel);
        
        this.maxAc.set('value',               this._airflowStates.maxAc);
    }
})
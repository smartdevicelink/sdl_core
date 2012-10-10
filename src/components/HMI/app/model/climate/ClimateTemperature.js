/**
 * @name ClimateTemperature
 *
 * @desc Climate temperature
 *       Designed to fomat object value into string and to check constrinats for temperature
 * 
 * @category	Model
 * @filesource	app/model/ClimateTemperature.js
 * @version		2.0
 * @author 		Sergey Solovey, Artem Petrosyan
 */
 
MFT.ClimateTemperature = Em.Object.extend({
	
	minValue: 			59,
	maxValue: 			86,
	defaultValue: 		70,
	value: 				70,
	valueStr:			'',
	enabled:			true,
	
	/** Set temperature defalut value */
	setDefault: function() {
		this.set('value', this.defaultValue);
	},
	
	disable: function() {
		this.set('enabled', false);
	},
	
	enable: function() {
		this.set('enabled', true);
	},
	
	/** Set temperature minimal value */
	min: function() {
		this.set('value', this.minValue);
	},
	
	/** Set temperature maximum value */
	max: function() {
		this.set('value', this.maxValue);
	},
	
	/**
	 * Increases seat value until it reaches the maximum possible value
	 */
	increase: function()
	{
		if (this.value < this.maxValue){
			this.set('value', this.value+1);
		}
	},
	
	/**
	 * Decreases seat value until it reaches the minimum possible value
	 */
	decrease: function()
	{
		if (this.value > this.minValue){
			this.set('value', this.value-1);
		}
	},
	
	/**
	 * Format temperature to string
	 */
 	toString: function() {
 		if ( this.enabled ) {
 			if (this.value >= this.maxValue) {
	 			this.set('valueStr', 'HI');
	 		} else if (this.value <= this.minValue) {
	 			this.set('valueStr', 'LO');
	 		} else {
	 			this.set('valueStr', this.value+'&deg;')
	 		}
 		} else {
 			this.set('valueStr','OFF');
 		}	
 	}.observes('value','enabled')
})
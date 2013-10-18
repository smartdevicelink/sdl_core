/**
 * @name Ranged Value
 * 
 * @desc Abstract ranged value class
 * 
 * @category	Model
 * @filesource	app/model/abstract/RangedValue.js
 * @version		2.0
 *
 * @author 		Sergey Solovey
 */


MFT.RangedValue = Em.Object.extend({	

	range: 		3,
	minValue:	1,
	value: 		0,
	cycle:		true,
	enabled:	true,

	/**
	 * @desc Cyclically increases seat value until it reaches the range value
	 * 		 than value is switched to off (0)
	 */
	increase: function()
	{
		if ( !this.enabled ) {
			this.enable();
			return;
		}
		
		if (this.value < this.range){
			this.set('value', this.value+1);
		}else{
			if (!this.cycle ) {
				return;
			}
			this.set('value', 0);
		}
	},
	/**
	 * Toggle indicators one by one	
	*/
	toggleIndicators: function(){
		if ( !this.enabled ) {
			this.enable();
			return;
		}
		
		if (this.value < this.range-1){
			this.set('value', this.value+1);
		}else{
			this.set('value', 0);
		}
	},
	/**
	 * @desc Cyclically increases seat value until it reaches zero value (off)
	 * 		 than value is changed to the range value
	 */
	decrease: function()
	{
		if ( !this.enabled ) {
			this.enable();
			return;
		}
		
		if (this.value >= this.minValue){
			if ( !this.cycle && (this.value == this.minValue) ) {
				return;
			}
			this.set('value', this.value-1);
		}else{ 
			this.set('value', this.range);
		}
	},
	
	/**
	 * @desc Switch off
	 */
	off: function()
	{
		this.set('beforeOff', this.value);
		this.set('value', 0);
	},
	
	/**
	 * Switch on
	 */
	on: function() {
		this.set('value', this.range);
	},
	
	/**
	 * Switch visual enable
	 */
	enable: function() {
		this.set('enabled', true);
	},
	
	/**
	 * Switch visual disable
	 */
	disable: function() {
		this.set('enabled', false);
	},
	
	
	/**
	 * Revert to value before toggle Off
	 */
	revert: function() {
		if ( this.beforeOff ) {
			this.set('value', this.beforeOff);
		}
	}
})
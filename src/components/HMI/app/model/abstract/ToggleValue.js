/**
 * @name Toggle Value
 * 
 * @desc Abstract toggle value class
 * 
 * @category	Model
 * @filesource	app/model/abstract/ToggleValue.js
 * @version		2.0
 *
 * @author 		Artem Petrosyan
 */

MFT.ToggleValue = Em.Object.extend({
	
	value: false,
	
	/**
	 * Switch on
	 */
	on: function() {
		this.set('value', true);
	},
	
	/**
	 * Switch off
	 */
	off: function() {
		this.set('value', false);
	}
})
/**
 * @name FFW.AbstractButton
 * 
 * @desc Abstact Button component
 * 
 * @category	ui
 * @filesource	ffw/ui/AbstactButton.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

FFW.AbstractButton = Em.View.extend({
	
	classNames: ['ffw-button'],
	
	classNameBindings: [
		'pressed:pressed:notpressed',
		'disabled'
	],
	
	// pressed state flag
	pressed: false,
	
	// disabled state flag
	disabled: false,
	
	// action down handeler
	// trigger when user mouse/touch down
	actionDown: function() {
		this.set('pressed', true);
	},
	
	// action up handeler
	// trigger when user mouse/touch up
	actionUp: function() {
		this.set('pressed', false);
	},
	
	// action move handeler
	// trigger when user mouse/touch move
	actionMove: function() {
		
	}
});
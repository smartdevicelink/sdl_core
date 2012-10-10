/**
 * @name MFT.SettingsController
 * 
 * @desc Settings	Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/SettingsController.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsController = Em.Object.create({
		
	activeState:  'settings.clock',
		
	hiddenLeftMenu: false,

	onState: function(event){
		MFT.States.goToState('settings.'+event.goToState);
	},

	onChildState: function(event){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	},

	onPreviousState: function(){
		MFT.States.goToState(this.previousState);
		MFT.StateVisitor.visit(MFT.States.currentState);
	},

	/** time string for top clock */
	time: '',

	temperature: ' 86°',

	/**
	 * Get and format time for top clock
	 * @param date - Date object
	 */
	getTime: function(date) {
		
		if ( MFT.States.settings.clock.active ) {
			return;
		}

		var minutes = date.getMinutes();
		var hours = date.getHours();
		
		minutes = ( minutes < 10 )	?	'0'+minutes	: minutes;
		
		hours =	( hours > 12 )		?	hours -12	: hours;
		hours = ( hours == 0 )		?	12			: hours;
				
		this.set('time', hours + ':' + minutes +  this.get('temperature'));
	},

	// Valet mode popup
	valetPopUp: false,
	
	showValetPopUp: function(){
		this.set('valetPopUp',true);
	},
	
	hideValetPopUp: function(){
		this.set('valetPopUp',false);
	},
	
	// Enter pin
	onEnterPin: function(){
		this.hideValetPopUp();
		MFT.States.goToState('settings.vehicle.enterPIN');
	}
});
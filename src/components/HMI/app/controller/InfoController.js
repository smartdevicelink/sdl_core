/**
 * @name MFT.InfoController
 * 
 * @desc Info	Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/InfoController.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.InfoController = Em.Object.create({
	
	activeState:  'info.services',
	
	hiddenLeftMenu: false,

	onState: function(event){
		MFT.States.goToState('info.'+event.goToState);
	},
	
	onChildState: function(event){
		
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	},
});
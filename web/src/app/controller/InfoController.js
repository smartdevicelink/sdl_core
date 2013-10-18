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
    init: function() {
        this._super();
        this.setActiveState();
    },
	
	activeState: null,

    setActiveState: function() {
        if (FLAGS.locale === 'eng') {
            this.set('activeState', 'info.services');
        } else if (FLAGS.locale === 'spa') {
            this.set('activeState', 'info.alerts');
        } else {
            this.set('activeState', 'info.travelLink');
        }
    },

	hiddenLeftMenu: false,

	onState: function(event){
		MFT.States.goToState('info.'+event.goToState);
	},
	
	onChildState: function(event){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	}
});
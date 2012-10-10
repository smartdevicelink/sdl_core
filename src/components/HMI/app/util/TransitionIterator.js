/**
 * @name MFT.TransitionIterator
 * 
 * @desc Transition iterator used for proper transitions between states
 * 
 * @category	utils
 * @filesource	app/util/TransitionIterator.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.TransitionIterator = {
	
	// this flag determine that some of views is loading now
	// need to prevent next state transition, before finish loading
	ready: true,
	
	// final state path
	finalPath: MFT.homeView,
	
	// active states
	stateOnEnter: [],
	
	// inactive states
	stateOnExit: [],
	
	/**
	 * Init transition:
	 * set destination state path
	 * block other transitions before views inserted in DOM
	 * @path {string}
	 */
	init: function( path ) {
		this.finalPath = path;
		this.ready = false;
	},
	
	/**
	 * Start transitions:
	 * recursive activate all active states
	 * and deactivate all inactive states
	 * @state {Em.State}: target state object
	 */
	start: function(state) {
		
		// transition will start only in final state		
		if( this.finalPath != state.get('path') ) {
			return;
		}
		
		// activation states
		while ( this.stateOnEnter.length ) {
			this.stateOnEnter.pop().set('active',true);
		}
		
		// deactivation states
		while ( this.stateOnExit.length ) {
			this.stateOnExit.pop().set('active',false);
		}
		
		// unblock transition
		this.ready = true;
	}
};
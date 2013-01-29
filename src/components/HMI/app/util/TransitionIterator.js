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

MFT.TransitionIterator = Em.Object.create({
	
	// this flag determine that some of views is loading now
	// need to prevent next state transition, before finish loading
	ready: true,
	
	// final state path
	finalPath: MFT.homeView,
	
	// active states
	stateOnEnter: [],
	
	// inactive states
	stateOnExit: [],
	
	// start transition timestamp
	// used in DEBUG_MODE
	startTime: 0,
	
	/**
	 * Init transition:
	 * set destination state path
	 * block other transitions before views inserted in DOM
	 * @path {string}
	 */
	init: function( path ) {
		
		if ( FLAGS.DEBUG_MODE ) {
			this.startTime = Date.now();
		}
		if ( path ) {
			this.set('finalPath', path);
		}
		this.set('ready', false);
	},
	
	/**
	 * Start transitions:
	 * recursive activate all active states
	 * and deactivate all inactive states
	 * @state {Em.State}: target state object
	 */
	start: function(state) {
		
		// transition will start only in final state		
		if( this.get('finalPath') != state.get('path') ) {
			return;
		}
		
		// activation states
		while ( this.get('stateOnEnter.length') ) {
			this.get('stateOnEnter').pop().set('active',true);
		}
		
		// deactivation states
		while ( this.get('stateOnExit.length') ) {
			this.get('stateOnExit').pop().set('active',false);
		}
		
		// unblock transition
		this.set('ready', true);
		
		// show transition time
		// used in DEBUG_MODE
		if (this.startTime) {
			MFT.set('debugText', ((Date.now() - this.startTime)/1000).toFixed(2) );
		}
	}
});
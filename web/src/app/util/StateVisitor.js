/**
 * @name MFT.StateVisitor
 * 
 * @desc State visitor user for different enter state beheviors
 * 
 * @category	utils
 * @filesource	app/util/StateVisitor.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.StateVisitor = {
	
	// collection of loaded scripts
	scriptCollection: [],
	
	// visitor logic
	visit: function( state ) {
		// load script if necessary
		if ( !state.viewLoaded ) {
			FFW.AppendScript.pushScript( state.get('path') );
		} else {
			MFT.TransitionIterator.start( state );
		}
	}
};
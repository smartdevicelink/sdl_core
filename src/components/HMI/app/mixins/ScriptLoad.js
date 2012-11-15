/**
 * @name MFT.LoadableView
 * 
 * @desc Used for initialization view in runtime
 * 
 * @category	mixin
 * @filesource	app/mixins/ScriptLoad.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.LoadableView = Ember.Mixin.create({
	
	classNameBindings: [
		'stateObj.active:active_state:inactive_state'
	],
		
	init: function() {
		this._super();
		
		this.set( 'stateObj', MFT.States.getState( FFW.AppendScript.getState() ) );
		
		// EXCEPTION
		if ( !this.stateObj ) {
			alert( this.get('elementId')+' is not associated to state object' );
			MFT.States.back();
			
			return;
		}
		
		this.stateObj.set('viewLoaded', true);
				
		// for backwards compatible
		this.stateObj.set('view', this);
		
		//this.loadTime = Date.now();
		
		
		// for testing
		/*var self = this;
		setTimeout(function(){
			self.loadView();
		},1000)
		*/
		this.loadView();
	},
	
	loadView: function(){		
		if( this.stateObj.parentState.get('path') ) {
			// for child views
			this.stateObj.parentState.view.get('childViews').pushObject(this);
		} else {
			// for root views
			MFT.views.activeAview.get('childViews').pushObject(this);
		}
	},
	
	didInsertElement: function() {
		this._super();
		MFT.TransitionIterator.start(this.stateObj);
		
		// dispatch onLoad event to loader
		FFW.AppendScript.onLoad();
		
		//Em.Logger.log( this.get('elementId')+': ',(Date.now() - this.loadTime));
	}
});
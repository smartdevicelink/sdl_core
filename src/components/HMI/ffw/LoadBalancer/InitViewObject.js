/**
 * @name FFW.InitViewObject
 * 
 * @desc Dynamic view object ininted immidiatly after js file rendered
 * 
 * @category	Framework
 * @filesource	app/LoadBalancer/InitViewObject.js
 * @version	2.0
 *
 * @author	Igor Zhavoronkin
 */
FFW.InitViewObject = Em.Object.extend({
	
	view: null,
	
	state: null,
	
	init: function(){
		//Set view object to state object
		this.state.set('view', this.view);
		this.state.view.set('stateView', this.state);
		//Load View
		this.loadView();		
	},
	
	loadView: function(){
		
		if(MFT.views){
			if ( this.state.view) {
				// Insert to Super Root View if it is needed by design
				if(this.root){
					console.log('root');
					if (!this.root.view.get('childViews').contains(this.state.view) ) {				
						this.root.view.get('childViews').pushObject(this.state.view);
					}
				}else{
					// Root views
					if ( !this.state.parentState.view ) {
						this.state.parentState.view = MFT.views.activeAview;
					}
					// View first open
					if (!this.state.parentState.view.get('childViews').contains(this.state.view) ) {				
						this.state.parentState.view.get('childViews').pushObject(this.state.view);
					}
				}

			}
		}
		// Destroy object
		this.destroy();
	}
});
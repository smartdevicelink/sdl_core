/**
 * @name MFT.NavigationController
 * 
 * @desc Navigation module logic
 * 
 * @category	Controller
 * @filesource	app/controller/NavigationController.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.NavigationController = Em.Object.create({
	
	modelBinding: 'MFT.NavigationModel',
	
	viewType: 	'typeview1',
	
	mapType:  	'',
	
	zoomMap: 	'',
	
	activeState: 'navigation',
	
	toggleTypeView: function(){	
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('nav_Views');
			
			return;
		}
		this.model.typeView.toggleIndicators();
	},
	
	toggleMapView: function(){
		this.model.mapView.toggleIndicators();
	},
	
	toggleMute: function(){
		this.model.mute.increase();
	},
	
	zoomIn: function(){
		this.model.zoomRate.zoomIn();
	},
	
	zoomOut: function(){
		this.model.zoomRate.zoomOut();
	},
	
	subState: function() {
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('nav_Destination');
			
			return;
		}
		MFT.States.goToState('navigation.dest');
	},
	
	turnRepeatHelpVideoOn: function(){
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('nav_Repeat');
			
			return;
		}
	},
	
	onChildState: function(event){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	}
});




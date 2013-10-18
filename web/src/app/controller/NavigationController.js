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
		this.turnOffMenu();
		this.model.typeView.toggleIndicators();
	},
	
	toggleMapView: function(){
		this.turnOffMenu();
		this.model.mapView.toggleIndicators();
	},
	
	turnOnMenu: function(){
		MFT.States.goToState('navigation.menu');
	},
	
	turnOffMenu: function(){
		if(MFT.States.navigation.menu.active){
			MFT.States.back();
		}
	},
	
	onHelpMode: function(){
		if(MFT.helpMode)this.turnOffMenu();
	}.observes('MFT.helpMode'),
	
	turnOnNavSettings: function(){
		MFT.States.goToState('settings.settings.snavigation');
		MFT.States.set('previousState','navigation');
	},
	
	turnOnSiriusLink: function(){
		MFT.States.goToState('info.travelLink');
	},
	
	toggleMute: function(){
		this.model.mute.increase();
	},
	
	zoomIn: function(){
		this.turnOffMenu();
		this.model.zoomRate.zoomIn();
	},
	
	zoomOut: function(){
		this.turnOffMenu();
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




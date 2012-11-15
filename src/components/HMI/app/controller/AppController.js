/**
 * @name MFT.AppController
 * 
 * @desc App general logic
 * 
 * @category	Controller
 * @filesource	app/controller/AppController.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.AppController = Em.Object.create({
	
	/****   System Settings  Popup Logic   *****/
	// visual representation variable for system select view popup
	active: false,
	// default vehicle option
	vehicleType: 'edge',
	// default navigation option
	navigationEnabled: true,
	
	/* Buttons array*/
	buttons: MFT.SelectSystempopup.content.climate_style.list._childViews,
	/* Set Active state to pressed button */
	setActiveButton: function(button){
		//remove active state for prev button
		this.buttons.setEach('active',false);
		// set active to current button
		button.set('active',true);
		// set Vechicle Model
		this.set('vehicleType', button.elementId);
		
	},
	
	onYespress: function(){
		this.set('navigationEnabled',true);
	},
	
	onNopress: function(){
		this.set('navigationEnabled',false);
	},
	
	onSettingsSubmit: function(view){
		// sen selected options to backend
		FFW.Backend.set('isNavigationEnabled',this.navigationEnabled);
		FFW.Backend.set('vehicleModel', this.vehicleType);
		//remove system settings view
		view._parentView.remove();
		// save selected options
		FFW.Backend.saveVehicleModel();
		FFW.Backend.saveIsNavigationEnabled();
	},
		
	/****   END System Settings  Popup Logic   *****/
	
	// Flag to determine first launch of welcome orientation video in order to show system settings popup on video end
	
	wo_video_first_launch: false
	
});
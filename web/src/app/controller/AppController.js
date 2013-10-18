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

    /** Active vehicle mode for the whole application */
    activeVehicleModel: function(){
        /** Return current or default vehicle model*/
        return MFT.vehicles[FFW.Backend.vehicleModel] || MFT.vehicles[FFW.Backend.DEFAULT_VEHICLE_MODEL];
    }.property('FFW.Backend.vehicleModel'),
	
	/****   System Settings  Popup Logic   *****/
	// visual representation variable for system select view popup
	active: false,

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

    getCurrentDate: function(){
        var now = new Date();
        return MFT.locale.label['month_' + now.getMonth()] + ' ' + now.getDate() + ', ' + now.getFullYear();
    }.property(),
	
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
		//view._parentView.remove();
		this.set('active',false);
		// save selected options
		FFW.Backend.saveVehicleModel();
		FFW.Backend.saveIsNavigationEnabled();
	},	
	/****   END System Settings  Popup Logic   *****/
	
	/**
	 * Invert value of IsNavigationEnabled
	 *
	 * @property(boolean) isNavigationEnabled
	 * @return boolean
	 */
	invertIsNavigationEnabled: function(){
		return !FFW.Backend.isNavigationEnabled
	}.property('FFW.Backend.isNavigationEnabled')
});
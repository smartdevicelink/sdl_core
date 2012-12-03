/**
 * @name MFT.ApplinkController
 * 
 * @desc Main Applink Controller
 * 
 * @category	Controller
 * @filesource	app/controller/applink/ApplinkController.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkController = Em.Object.create({
	
	/**
	 * List of registered applications
	 * @type object
	 */
	registeredApps: {
		1: 0,	// temp
		2: 1	// temp
	},
	
	/** 
	 * List of Applink application models
	 *
	 * @type object
	 */
	applicationModels: {
		0:	'ApplinkMediaModel',
		1:	'ApplinkNonMediaModel'
	},
	
	/**
	 * Register application
	 * @desc ...
	 *
	 * @param applicationId: Number
	 * @param applicationType: Number
	 */
	registerApplication: function( applicationId, applicationType ) {
		if ( this.registeredApps[ applicationId ] ) {
			//Em.Logger.error('Application ['+ applicationId +'] already registered!');
			return;
		}
		
		this.registeredApps[ applicationId ] = applicationType;
		//Em.Logger.log('Application ['+ applicationId +'] registered!');
	},
	
	/**
	 * Unregister application
	 * @desc ...
	 *
	 * @param applicationId: Number
	 */
	unRegisterApplication: function( applicationId ) {
		delete this.registeredApps[ applicationId ];
	},
	
	/**
	 * Get application model
	 * @desc ...
	 *
	 * @param applicationId: Number
	 * @return Object Model
	 */
	getApplicationModel: function( applicationId ) {
		return MFT[ this.applicationModels[ this.registeredApps[ applicationId ] ] ];
	}
});
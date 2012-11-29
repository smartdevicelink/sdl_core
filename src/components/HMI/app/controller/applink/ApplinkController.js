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
	
	},
	
	/**
	 * Register application
	 * @desc ...
	 *
	 * @param applicationId: Number
	 * @param applicationType: ?
	 */
	registerApplication: function( applicationId, applicationType ) {
		if ( this.registeredApps[ applicationId ] ) {
			Em.Logger.error('Application ['+ applicationId +'] already registered!');
			return;
		}
		
		this.registeredApps[ applicationId ] = applicationType;
		Em.Logger.log('Application ['+ applicationId +'] registered!');
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
	 * Get application type
	 * @desc ...
	 *
	 * @param applicationId: Number
	 * @return ?
	 */
	getApplicationType: function( applicationId ) {
		return this.registeredApps[ applicationId ];
	}
});
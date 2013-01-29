/**
 * @name MFT.NonMediaController
 * 
 * @desc Applink NonMedia Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/applink/NonMediaController.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.NonMediaController = Em.Object.create({
	
	/**
	 * Current NonMedia application id
	 *
	 * @type {Number}
	 */
	currentAppId: 0,
    
    /**
     * Return current NonMedia application name
     * used for application button
     */
    currentAppName: function() {
        if ( this.currentAppId ) {
            return MFT.ApplinkController.getApplicationModel(this.currentAppId).appName;
        }
    }.property('this.currentAppId'),
    
    /**
     * Return current NonMedia application icon
     * used for application button
     */
    currentAppIcon: function() {
        if ( this.currentAppId ) {
            return MFT.ApplinkController.getApplicationModel(this.currentAppId).appIcon;
        }
    }.property('this.currentAppId'),
    
    /**
     * Activate application model
     *
     * @param {ApplinkAppModel}
     */
    activateApp: function( applicationModel ){
        
        // store active application id
        this.set( 'currentAppId' , applicationModel.appId);
        
        // set active model
        MFT.ApplinkAppController.set('model',applicationModel);
        
        // send response
        FFW.AppLinkCoreClient.ActivateApp( applicationModel.appId );
        
        // Go to Applink state
        MFT.States.goToState('info.nonMedia');
    },
    
    /**
     * Restore current application to active state
     */
    restoreCurrentApp: function() {
        if ( MFT.ApplinkAppController.model.appId === this.currentAppId ) {
            return;
        }
        this.activateApp( MFT.ApplinkController.getApplicationModel( this.currentAppId ) );
    }
});
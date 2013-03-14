/**
 * @name MFT.NonMediaController
 * 
 * @desc SDL NonMedia Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/sdl/NonMediaController.js
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
            return MFT.SDLController.getApplicationModel(this.currentAppId).appName;
        }
    }.property('this.currentAppId'),
    
    /**
     * Return current NonMedia application icon
     * used for application button
     */
    currentAppIcon: function() {
        if ( this.currentAppId ) {
            return MFT.SDLController.getApplicationModel(this.currentAppId).appIcon;
        }
    }.property('this.currentAppId','MFT.SDLAppController.model.appIcon'),
    
    /**
     * Activate application model
     *
     * @param {SDLAppModel}
     */
    activateApp: function( applicationModel ){
        
        // store active application id
        this.set( 'currentAppId' , applicationModel.appId);
        
        // set active model
        MFT.SDLAppController.set('model',applicationModel);
        
        // send response
        //FFW.AppLinkCoreClient.ActivateApp( applicationModel.appId );
        
        // Go to SDL state
        MFT.States.goToState('info.nonMedia');
    },
    
    /**
     * Restore current application to active state
     */
    restoreCurrentApp: function() {
        if ( MFT.SDLAppController.model.appId === this.currentAppId ) {
            FFW.AppLinkCoreClient.ActivateApp( this.currentAppId );
            return;
        }
        this.activateApp( MFT.SDLController.getApplicationModel( this.currentAppId ) );
    }
});
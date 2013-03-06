/**
 * @name MFT.ApplinkAppController
 * 
 * @desc Applink abstract application controller
 * 
 * @category	Controller
 * @filesource	app/controller/applink/AppController.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkAppController = Em.Object.create({
	
	/**
	 * Active application model binding
	 *
	 * type {ApplinkAppModel}
	 */
	model: null,
	
	/**
	 * Handeler for command button press
	 * 
	 * @param element:	MFT.Button
	 */
	onCommand: function( element ) {

        // if submenu
        if ( element.menuId ) {

            // activate driver destruction if necessary
            if ( MFT.ApplinkController.driverDistractionState ) {
                MFT.DriverDistraction.activate();
            } else {
                this.onSubMenu( element.menuId );
            }

            return;
        }

		FFW.UI.onCommand( element.commandId, this.model.appId );
	},
	
	/**
	 * Open commands submenu
	 *
	 * @param {Number}
	 */
	onSubMenu: function( id ) {
	   this.model.set('currentSubMenuId', id );
	},

	/**
	 * Handeler for command button press
	 * 
	 * @param element:	MFT.Button
	 */
	onVRCommand: function( element ) {
		FFW.VR.onCommand( element.commandId, element.appId );
	},
	
	/**
	 * Handeler for preform interaction choice
	 * send response to device and deactivate interactions window
	 *
	 * @param element:	MFT.Button 
	 */
	onChoiceInteraction: function( element ) {

		FFW.UI.interactionResponse( 'SUCCESS', element.performInteractionRequestId, element.choiceId );

		MFT.InteractionChoicesView.deactivate();
	},
	
	/**
	 * Open commands list
	 */
	openCommandsList: function() {
	   MFT.OptionsView.activate();
	},
	
	/**
	 * Method clears all applications data
	 * and unregister models
	 */
	onApplinkDisconected: function() {
		var i=0,
            apps = MFT.ApplinkModel.registeredApps;

        for( i in apps){
			MFT.ApplinkModel.onAppUnregistered( { "appId" : apps[i].appId } );
		}
	}
});

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
	 * Handeler for command button press
	 * 
	 * @param element:	MFT.Button
	 */
	onCommand: function( element ) {
		FFW.UI.onCommand( element.commandId, element.appId );
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
	 * Handeler for command submenu press
	 * 
	 * @param element:	MFT.Button
	 */
	onCommandSubMenu: function( element ) {
		MFT.ApplinkController.getApplicationModel( element.appId ).set('currentSubMenuId',		element.menuId );
		MFT.ApplinkController.getApplicationModel( element.appId ).set('currentSubMenuLabel',	element.text );
		
		MFT.States.goToState( MFT.States.get('currentState.path') + '.subMenu' );
	},
	
	/**
	 * Handeler for preform interaction choice
	 * send response to device and deactivate interactions window
	 *
	 * @param element:	MFT.Button 
	 */
	onChoiceInteraction: function( element ) {
	
		FFW.UI.onChoosed( element.choiceId );
		
		MFT.InteractionChoicesView.deactivate();
	}
});

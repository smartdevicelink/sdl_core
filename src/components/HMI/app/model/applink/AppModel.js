/**
 * @name MFT.ApplinkAppModel
 * 
 * @desc Abstarct model for Applink applications
 * 
 * @category	Model
 * @filesource	app/model/applink/AppModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkAppModel = Em.Object.extend({
	
	/**
	 * Current command submenu identificator
	 *
	 * @type:	Number
	 */
	currentSubMenuId:		0,
	
	/**
	 * Current command submenu text label
	 *
	 * @type:	String
	 */	
	currentSubMenuLabel:	'',
	
	/**
	 * Interaction chooses data
	 *
	 * @type:	Object
	 */	
	interactionChoises: {}
});
 
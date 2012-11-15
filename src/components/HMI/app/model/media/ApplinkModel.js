/**
 * @name MFT.SDModel
 * 
 * @desc SD Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/SDModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.ApplinkModel = Em.Object.create({
	
	active: false,
	
	/** App Player*/
	init: function(){
		this._super();
	},
	
	/**
	  * Array of Interaction Choises
	  */
	 interactionChoises:	new Array(),

	/**
	  * Array of menus in ApplinkOptionsSubMenuView
	  */
	 subMenuCommands:	new Array(),

 	/**
	  * Array of Interaction Choises
	  */
	showInfo: Em.Object.create({
		field1: 'title',
		field2:'album',
		mediaTrack:'artist',
		mediaClock: "0:00",
		appName: "<App name>",
		deviceName: "Device name"
	})
});
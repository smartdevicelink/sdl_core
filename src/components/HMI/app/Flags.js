/**
 * @name MFT
 * 
 * @desc flags for application
 * 
 *	 flags used for diffeerent configurations of application
 * 	 As the same code base is used fro Production and RnD work pakackages,
 *	 it is necessary to configare application for different needs/releases
 * 
 * 	 It is NOT recommended to extend this object with new flags.
 *	 Each modification should be discussed with PM in advance 
 * 
 * @category	Application
 * @filesource	app/AppFlags.js
 * @version		1.0
 *
 * @author		Sergey Solovey
 */

FLAGS = Em.Object.create({
  	/** 
	 *  Emulate CAN network for Climate
	 *  true for MFT Guide production build
	 */
  	EMULATE_CAN_CLIMATE:	true,

  	/** 
	 *  Emulate Phone
	 *  true for MFT Guide production build
	 */
  	EMULATE_PHONE: 			true,

  	/** 
	 *  Emulate WebSocket connection
	 *  false for MFT Guide production build
	 */
	EMULATE_WS: 			false,

  	/** 
	 *  Emulate Android environment
	 *  it is necessary to debug some interfaces under browser
	 *  false for MFT Guide production build
	 */
  	EMULATE_ANDROID: 		false,

  	/** 
	 *  Extended logging only for debug and development 
	 *  !!! MUST BE false for release build !!! 
	 */
  	EXTENDED_LOG:			false,

	/** hide mouse cursor from screen */
	APP_HIDE_MOUSE_CURSOR:	false,


	HELP_MODE:			false,

  	/** 
	 *  show MCS screen in settings -> vehicle menu 
	 *  to be disabled until v2.1, in version v2.1 this feature will be released to end user
	 */
	MCS_ENABLED:			false,

  	/** 
	 *  Emulate CAN network for Multi Contour Seat
	 *  false for MFT Guide production build
	 */
  	MCS_EMULATE_CAN:		true,

	/** Sync bladders activity in MCS*/
	MCS_BLADDERS_SYNC:		true,

  	/** 
	 *  Active concept for MCS massage feature
	 */

	MCS_ACTIVE_CONCEPT:		1,

  	/** 
	 *  Use RPC or JavaScriptFacade interface for Video Player communications 
	 *  TEMPORARY
	 */
  	VIDEO_PLAYER_JS:		false
});
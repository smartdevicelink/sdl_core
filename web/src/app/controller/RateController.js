/**
 * @name MFT.RateController
 * 
 * @desc Rate module logic
 * 
 * @category	Controller
 * @filesource	app/controller/RateController.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.RateController = Em.Object.create({
	
	/** Visibility of "Rarating No Respond" Warning view */
	ratingNoRespondWarningIsVisible: false,
	
	/** Visibility of "Rating Network" Warning view */
	ratingNetworkWarningIsVisible: false,

	/** Visibility of "Leaving of MFTGuide" Warning view */
	leavingMFTGuideWarningIsVisible: false,	
	
	/** Visibility of "Rate MFT Guide" Dialogue view */
	rateMFTGuideDialogueIsVisible: false,	
	
	/** 
	 *  Connect to video player engine
	 *  Android 	- over RPC 
	 *  other OSes 	- actually does nothing  
	 */
	connect: function(){
		FFW.RaterRPC.connect();
	},
	
	/**
	 * Show element using visibility param.
	 *
	 * @param {String} visibility param.
	 */
	showPopUp: function( isVisibleProperty ){
			
		this.set(isVisibleProperty,true);

	},
	
	/**
	 * Hide popUp
	 *
	 * @param {String} visibility param.
	 */
	hidePopUp: function( isVisibleProperty ){
		
		this.set(isVisibleProperty,false);
		
	},
	
	/** Show Rate MFT Guide Dialogue */
	showRateMFTGuideDialogueView: function() {
		this.showPopUp('rateMFTGuideDialogueIsVisible');
	},
	
	/** Rate App */
	rateApp: function() {
		FFW.RaterRPC.rateMFTGuideResponse('OK');
		this.hidePopUp('rateMFTGuideDialogueIsVisible');
	},
	
	/** Cancel Rating of App */
	cancelRating: function() {
		FFW.RaterRPC.rateMFTGuideResponse('CANCEL');	
		this.hidePopUp('rateMFTGuideDialogueIsVisible');
	},
	
	/** Hide RatingNetwork Warning */
	hideRatingNetworkWarningView : function() {
		this.hidePopUp('ratingNetworkWarningIsVisible');		
	},
	
	/** Hide RatingNetwork Warning */
	hideRatingNoRespondWarningView : function() {
		this.hidePopUp('ratingNoRespondWarningIsVisible');		
	}
})
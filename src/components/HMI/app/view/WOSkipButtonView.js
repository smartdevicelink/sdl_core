/**
 * @name MFT.WOSkippButtonView
 * 
 * @desc Welcome orientation skipp button visual representation
 * 
 * @category	View
 * @filesource	app/view/WOSkippButtonView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
 MFT.WOSkippButtonView = MFT.Button.create({
	/** Element Id*/
	elementId: 'woSkippButton',
	/** Button appeares after 3 sec delay after Wo video started*/
	classNameBindings: ['this.showSkipButton:visible_display'],
	
	text: 'Skip',
	
	rightText: '>>',
	
	actionUp: function(){
		this._super();
		/**  Stop Video */
		MFT.VideoPlayerController.stop();
		/** Show WO Deny PopUp*/
		MFT.WelcomeOrientationView.set('woDeny',true);
		MFT.WelcomeOrientationView.set('hide',false);
		
	}
 });
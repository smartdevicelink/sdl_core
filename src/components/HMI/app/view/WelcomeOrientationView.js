/**
 * @name MFT.WelcomeOrientationView
 * 
 * @desc Warning View visual representation
 * 
 * @category	View
 * @filesource	app/view/WelcomeOrientationView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
 MFT.WelcomeOrientationView = Em.ContainerView.create({
	/** View id */
	elementId: 'app_welcome_orientation_view',
	/** Bind class for visual representation */
	classNameBindings: ['isActive:active_state'],
	/** Class Name */
	classNames: ['hidden'],
	/** Child Views */
	childViews: [
		'mask',
		'popup'
	],
	/** Enable Welcome Orientation screen if MCS is disabled*/
	isActive: function(){
		return (FFW.Backend.isFirstStart == true && FLAGS.MCS_ENABLED == false)
	}.property('FFW.Backend.isFirstStart'),
	/** Background layer */
	mask: Em.View.extend({
		elementId: 'app_welcome_orientation_balck_mask_view',
	}),
	
	/** Remove view when video started */
	removeView: function(){
		
		if(FFW.Backend.isFirstStart && FLAGS.MCS_ENABLED == false)
		{	
			//remove observers
			this.removeObserver('MFT.VideoPlayerController.model.isReady', this);
			this.removeObserver('MFT.VideoPlayerController.model.isError', this);
			
			if(MFT.VideoPlayerController.model.isError)
				{
					this.remove();
					//show system settings screen
					MFT.AppController.set('active',true);
				}
				if(MFT.VideoPlayerController.model.isReady)
				{
					this.remove(); // remove View
					setTimeout(function(){
						//check againe for player ready state
						if(MFT.VideoPlayerController.model.isReady){
							MFT.WOSkippButtonView.set('showSkipButton',true);
						}
					},3000);
				}
		}
	}.observes('MFT.VideoPlayerController.model.isReady', 'MFT.VideoPlayerController.model.isError'),

	/** Popup View */
	popup: Em.ContainerView.extend({
		/** View Id */
		elementId: 'app_welcome_orientation_popup_view',
		/** Child Views */
		childViews: [
			'text',
			'buttons',
			'lable'
		],
		/** Text Lable */
		text: MFT.Label.extend({
			elementId: 'app_welcome_orientation_popup_text',
			content: 'Do you want to start with a Welcome Orientation?',
		}),
		
		lable: MFT.Label.extend({
			elementId: 'app_welcome_orientation_popup_lable',
			content: 'Welcome Orientation is also available in the FAQ menu for future viewing',
		}),
		
		/** Buttons */
		buttons: Em.ContainerView.extend({
			/** View Id */
			elementId: 'app_welcome_orientation_popup_buttons',
			/** Child Views */
			childViews: [
				'yesButton',
				'noButton'
			],
			
			yesButton: MFT.Button.extend({
				elementId: 'wo_popup_yes_button',
				text: 'Yes',
				actionUp: function(){
					this._super();
					MFT.VideoPlayerController.start('FAQ_Welcome_orientation'); // start Wecome Orientation Video 
					
					MFT.AppController.set('wo_video_first_launch',true);
				}
			
			}),
			
			noButton: MFT.Button.extend({
				elementId: 'wo_popup_no_button',
				text: 'No',
				actionUp: function(){
					this._super();
					this._parentView._parentView._parentView.remove(); // remove View
					
					//remove observers
					this._parentView._parentView._parentView.removeObserver('MFT.VideoPlayerController.model.isReady', this);
					this._parentView._parentView._parentView.removeObserver('MFT.VideoPlayerController.model.isError', this);
					
					//show system settings screen
					MFT.AppController.set('active',true);
				}
			})
		})
	})
 });
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
	classNameBindings: ['isActive:active_state','hide:not-visible'],
	/** Class Name */
	classNames: ['hidden'],
	/** Child Views */
	childViews: [
		'mask',
		'popup',
		'denypopup'
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
				}
				if(MFT.VideoPlayerController.model.isReady)
				{
					this.set('hide',true);
					setTimeout(function(){
						MFT.WOSkippButtonView.set('showSkipButton',true);
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
			'buttons'
		],
		/** Text Lable */
		text: MFT.Label.extend({
			elementId: 'app_welcome_orientation_popup_text',
			content: 'Do you want to start with a Welcome Orientation?',
		}),
		/** Buttons */
		buttons: Em.ContainerView.extend({
			/** View Id */
			elementId: 'app_welcome_orientation_popup_buttons',
			/** Child Views */
			childViews: [
				'yesButton',
				'noButton',
				'delim'
			],
			
			delim: Em.View.extend({
				elementId: 'wo_popup_btn_divider'
			}),
			
			yesButton: MFT.Button.extend({
				elementId: 'wo_popup_yes_button',
				text: 'YES',
				actionUp: function(){
					this._super();
					MFT.VideoPlayerController.start('FAQ_Welcome_orientation'); // start Wecome Orientation Video 
				}
			
			}),
			
			noButton: MFT.Button.extend({
				elementId: 'wo_popup_no_button',
				text: 'NO',
				actionUp: function(){
					this._super();
					this._parentView._parentView._parentView.set('woDeny',true); // show deny WO popup
				}
			
			})
		})
	}),
	/** Welcome orientation Deny Popup */
	denypopup: Em.ContainerView.extend({
			/** Element Id  */
			elementId: 'wo_denypopup',
			/** Bind class for visual representation */
			classNameBindings: ['parentView.woDeny:active_state'],
			/** Class Name */
			classNames: 'hidden',
			
			/** Child Views */
			childViews: [
				'text',
				'button'
			],
			
			/** Text Lable */
			text: MFT.Label.extend({
				elementId: 'app_welcome_orientation_denypopup_text',
				content: 'To watch the Welcome Orientation in the future, go to the FAQ menu',
			}),
			/** Ok button */
			button: MFT.Button.extend({
				elementId: 'wo_popup_denypopup_ok_button',
				text: 'OK',
				actionUp: function(){
					this._super();
					FFW.Backend.set('isFirstStart',false);
					this._parentView._parentView.remove(); // remove View
				}
			
			})
		})
 });
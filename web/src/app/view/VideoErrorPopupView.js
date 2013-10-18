/**
 * @name MFT.VideoErrorPopupView
 * 
 * @desc Video Error Popup visual representation 
 * 
 * @category	view
 * @filesource	app/view/VideoErrorPopupView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
 MFT.VideoErrorPopupView = Em.ContainerView.create({
	/** View Id*/
	elementId: 'video_error_popup',
	/** Class Names*/
	classNames: 'hidden',
	/** View Bindings */
	classNameBindings:	['MFT.VideoPlayerController.model.isError:active_state'],
	/** Child views*/
	childViews: [
		'mask',
		'container'
	],
	
	mask: Em.View.create({
		classNames: 'mask'		
	}),
	
	container:  Em.ContainerView.create({
		
		/** Class Names*/
		classNames: 'wrapper',
		
		/** Child views*/
		childViews: [
			'title',
			'message',
			'button'
		],
	
		/** Title Message */
		title: Em.ContainerView.create({
			 
			classNames: 'title',
			 
			 /** Child views*/
			childViews: [
				'sign',
				'text'
			],	
			
			sign: Em.View.create({
				classNames: 'alert-sign' 
			}),
		
			text: MFT.Label.extend({
				classNames:'error-title',	
				contentBinding: "MFT.locale.label.videoerrorpopupview_no_video_message"
			})
		}),
	
		/** Message */
		message: Em.View.extend({
			
			classNames: 'error-message',
			// Due to short Error message reduce message block width
			classNameBindings: ['this.isErrorCode_11:narrow'],
			// ??? TODO Find OUT how to break lines in Ember String ???
			isErrorCode_11: function(){
				return MFT.VideoPlayerController.model.errorCode === 11;
			}.property('MFT.VideoPlayerController.model.errorCode'),
			
			isThreelines: function(){
				return (!(FLAGS.locale == 'fr'));
			}.property('FLAGS.locale'),
			
		
			/**
			  * Error Code == 10 (No network connection)
			  * Error Code == 11 (If video file is missing when network is available but video still downloading in the background)
			  *	Error Code == 12 (Video File is corrupted)
			  * Error Code == 13 (If network is available, but video is corrupted and maximum download attempt value has been reached)
			 */	
			
			template: Em.Handlebars.compile(
				'<p>{{MFT.VideoPlayerController.errorMessage.line0}}'+
				'{{#if view.isErrorCode_11 }}'+
					'{{#if view.isThreelines}}</p><p> {{/if}}'+
					'{{MFT.VideoPlayerController.errorMessage.line1}}</p>'+					
					'<p>{{MFT.VideoPlayerController.errorMessage.line2}}</p>'+	
				'{{/if}}'
			),
			//contentBinding: 'MFT.VideoPlayerController.errorMessage'
		}),
		/** Ok Button*/
		button: MFT.Button.extend({
			elementId: 'videoerror-ok-button',
			action: 'closeErrorPopup',
			target: 'MFT.VideoPlayerController',
			onDown: false,
			text: 'OK'
		})
	
	})
 });
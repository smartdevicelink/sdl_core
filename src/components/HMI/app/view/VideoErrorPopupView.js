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
 
 MFT.VideoErrorPopupView = MFT.Label.create({
	/** View Id*/
	elementId: 'video_error_popup',
	
	content:   'No Video File',
	/** Error Event listener*/
	onError: function(){
		if(MFT.VideoPlayerController.model.isError){
			// Show popup
			$('#video_error_popup').fadeIn();
			// Hide popup after timeout
			setTimeout(function() {
				$('#video_error_popup').fadeOut();
				MFT.VideoPlayerController.model.set('isError',false);
			},3000);
		}
	}.observes('MFT.VideoPlayerController.model.isError')

 });
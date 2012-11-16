/**
 * @name MFT.FAQController
 * 
 * @desc FAQ Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/FAQController.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.FAQController = Em.Object.create({
	
	playRecord: function(event){
		MFT.VideoPlayerController.start(event.videoName)
	}
});
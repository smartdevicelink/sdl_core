/**
 * @name MFT.seatsMassageView7_1
 * 
 * @desc MCS massage view component 7.1 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MCSConcept7_1_VideoHandler.js
 * @version		2.0
 *
 * @author	Artem Petrosyan	
 */

MFT.MCSConcept7_1_VideoHandler = Em.Object.extend({

	currentPopUp: 0,

	onTime: function(){

			if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 1) && (this.get('currentPopUp') == 0)){

					MFT.MCSController.set('dialog.4', true);
					this.set('currentPopUp', 1);

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 6) && (this.get('currentPopUp') == 1)){

					MFT.MCSController.set('dialog.4', false); 
					this.set('currentPopUp', 2);

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 9) && (this.get('currentPopUp') == 2)){

					MFT.MCSController.set('massageCustom', true);
					MFT.MCSController.set('dialog.0', true);
					MFT.VideoPlayerController.pause();
					this.set('currentPopUp', 3);

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 11.2) && (this.get('currentPopUp') == 3)){

					MFT.MCSController.set('massageCustom', true);
					MFT.MCSController.set('dialog.1', true);
					MFT.VideoPlayerController.pause();
					this.set('currentPopUp', 4);

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 14.5) &&  (this.get('currentPopUp') == 4) ){

					MFT.MCSController.set('massageCustom', true);
					MFT.MCSController.set('dialog.2', true);
					MFT.VideoPlayerController.pause();
					this.set('currentPopUp', 5);

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 17.5) && (this.get('currentPopUp') == 5)){

					MFT.MCSController.set('massageCustom', true);
					MFT.MCSController.set('dialog.3', true);
					MFT.VideoPlayerController.pause();
					this.set('currentPopUp', 6);

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 18) && (this.get('currentPopUp') == 6) ){

					this.set('currentPopUp', 7);
					MFT.MCSController.set('dialog.5', true); 

			}else if((Number(MFT.VideoPlayerController.model.get('currentVideoTime').toFixed(2)) > 27.5) && (this.get('currentPopUp') == 7) ){

					this.set('currentPopUp', 8);
					MFT.MCSController.set('dialog.5', false); 

			}

	}.observes('MFT.VideoPlayerController.currentPlayBackTime')
});
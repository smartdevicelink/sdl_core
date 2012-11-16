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
	  * Timer for Media Clock
	  */
	timer: null,

 	/**
	  * Array of Interaction Choises
	  */
	showInfo: Em.Object.create({
		field1:		'title',
		field2:		'album',
		mediaTrack:	'artist',
		mediaClock:	"0:00",
		appName:	"<App name>",
		deviceName:	"Device name",
		duration:	0,
		cuttTime:	0,
		countUp:	true,
		pause:		true
	}),

	startTimer: function(){
		if(this.showInfo.pause){
			if(MFT.ApplinkModel.showInfo.countUp){
				this.timer = setInterval(function(){
					if(MFT.ApplinkModel.showInfo.cuttTime < MFT.ApplinkModel.showInfo.duration){
						MFT.ApplinkModel.showInfo.cuttTime++;
					}
				}, 1000);
			}else{
				this.timer = setInterval(function(){
					if(MFT.ApplinkModel.showInfo.cuttTime > 0){
						MFT.ApplinkModel.showInfo.cuttTime--;
					}
				}, 1000);
			}
		}else{
			clearInterval(this.timer);
		}
	}.observes('this.showInfo.pause'),

	startTimer: function(){
		MFT.ApplinkModel.showInfo.set('mediaClock', this.showInfo.cuttTime/60 + ":" + this.showInfo.cuttTime % 60);
	}.observes('this.showInfo.cuttTime'),
});
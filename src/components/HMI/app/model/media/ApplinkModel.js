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

	countUp:		true,
	pause:			true,
	maxTimeValue:		68400, // 19 hours

 	/**
	  * Array of Interaction Choises
	  */
	showInfo: Em.Object.create({
		field1:			'<field1>',
		field2:			'<field2>',
		field3:			'<field3>',
		mediaClock:		"<mediaClock>",
		appName:		"<App name>",
		deviceName:		"<Device name>",
		duration:		0,
		currTime:		0,
	}),

	startTimer: function(){
		if(!this.pause){
			this.timer = setInterval(function(){
				this.showInfo.set('currTime', this.showInfo.currTime+1);
			}, 1000);
		}else{
			clearInterval(this.timer);
		}
	}.observes('this.showInfo.pause'),

	setDuration: function() {
		if(this.countUp){
			this.showInfo.set('mediaClock', Math.ceil((this.showInfo.duration + this.showInfo.currTime+1)/60)-1 + ":" + (this.showInfo.duration + this.showInfo.currTime) % 60 );
		}else{
			this.showInfo.set('mediaClock', Math.ceil((this.showInfo.duration - this.showInfo.currTime+1)/60)-1 + ":" + (this.showInfo.duration - this.showInfo.currTime) % 60 );
		}
	}.observes('this.showInfo.currTime'),

	changeDuration: function() {
		clearInterval(this.timer);
		this.startTimer();
	}.observes('this.showInfo.duration')
});
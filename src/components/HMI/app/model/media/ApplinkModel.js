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
	timer:			null,

	countUp:		true,
	pause:			false,
	maxTimeValue:	68400, // 19 hours
	duration:		0,
	currTime:		0,

 	/**
	  * Array of Interaction Choises
	  */
	showInfo: Em.Object.create({
		field1:			'<field1>',
		field2:			'<field2>',
		field3:			'<field3>',
		mediaClock:		'<mediaClock>',
		statusBar:		'<statusBar',
		appName:		'<App name>',
		deviceName:		'<Device name>'
	}),

	startTimer: function(){
		if(!this.pause){
			this.timer = setInterval(function(){
				MFT.ApplinkModel.set('currTime', MFT.ApplinkModel.currTime+1);
			}, 1000);
		}else{
			clearInterval(this.timer);
		}
	}.observes('this.pause'),

	setDuration: function() {
		if(this.countUp){
			this.showInfo.set('mediaClock', Math.ceil((this.duration + this.currTime+1)/60)-1 + ":" + (this.duration + this.currTime) % 60 );
		}else{
			this.showInfo.set('mediaClock', Math.ceil((this.duration - this.currTime+1)/60)-1 + ":" + (this.duration - this.currTime) % 60 );
		}
	}.observes('this.currTime'),

	changeDuration: function() {
		clearInterval(this.timer);
		this.currTime = 0;
		this.startTimer();
	}.observes('this.duration')
});
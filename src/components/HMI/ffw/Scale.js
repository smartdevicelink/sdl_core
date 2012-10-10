/**
 * @name Size
 * 
 * @desc Abstract scale class
 * 
 * @category	model
 * @filesource	app/model/abstract/Scale.js
 * @version	2.0
 *
 * @author 	Igor Zhavoronkin
 */
FFW.Scale = Em.Object.create({
	/** Scale Point*/
	scalePoint: 1,
	
	scale: function(){
		
		 /*ANDROID*/
		 if(FFW.isAndroid){
		 	/** Scale Rate*/
			if(FFW.Backend.isFullScreen){
				/** Calculate Scale Point*/
			   	var scalePointW = (FFW.Backend.windowSize.width/800) / FFW.Backend.windowDensity;
			   	var scalePointH = (FFW.Backend.windowSize.height/480) / FFW.Backend.windowDensity;
				/** Set calculated ScalePoint*/
			    (scalePointW >= scalePointH) ? this.set('scalePoint', scalePointH) : this.set('scalePoint', scalePointW)
			}else{
				this.set('scalePoint', 1 / FFW.Backend.windowDensity) 
			}
			/** Scale Application*/
			$('body').css({'-webkit-transform':'scale('+this.scalePoint+')','-webkit-transform-origin':'0px 0px' });
			
			/** Left Shift*/
			var LeftShiftAppPercents = (FFW.Backend.windowSize.width - 800*this.scalePoint*FFW.Backend.windowDensity) / 2 * this.scalePoint;
			/** Right Shift*/
			var UpShiftAppPercents = (FFW.Backend.windowSize.height - 480*this.scalePoint*FFW.Backend.windowDensity) / 2 * this.scalePoint;
			/** Align Application*/
			$('body').css({'margin-left': LeftShiftAppPercents + 'px', 'margin-top': UpShiftAppPercents + 'px'});
			
			//send response to BE for video resize
			Em.Logger.log("FFW.Backend.resizeVideo = " + FFW.Backend.resizeVideo);
			if(FFW.Backend.resizeVideo)
				FFW.Backend.sendFullScreenResponse($('body').offset().top,
													$('body').offset().left,
													this.scalePoint)
		 }
	}.observes('FFW.Backend.isFullScreen', 'FFW.Backend.windowDensity'),
})
 

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
        //MagicNumber 1500 - minimal allowable screen width value
        if ($(window).width() < 825) {
            this.scalePoint = $(window).width() / 825;
            $('body').css({'-webkit-transform':'scale('+this.scalePoint+')','-webkit-transform-origin':'0px 0px' });
	    }

    }
});
 

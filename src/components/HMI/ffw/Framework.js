/**
 * @name FFW
 * 
 * @desc Setup framework namespace and settings
 * 
 * @category	Framework
 * @filesource	app/Framework.js
 * @version	2.0
 *
 * @author	Sergey Solovey
 */

FFW = Em.Object.create({
 
   	/** Check for mobile devices */
 	isAndroid: 	navigator.userAgent.toLowerCase().indexOf("android") > -1,
	
	isIOS:  	(navigator.userAgent.toLowerCase().indexOf('ipad') != -1 || navigator.userAgent.toLowerCase().indexOf('iphone') != -1),
 	
	init: function() {
		Em.Logger.log('FFW: Framework init');
	}
});
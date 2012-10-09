/**
 * @name OS Info 
 * 
 * @desc Information about application environment
 * 
 * @category	Util
 * @filesource	app/util/OSInfo.js
 * @version	2.0
 *
 * @author 	Sergey Solovey
 */

FFW.OSInfo = Em.Object.extend({	

	/*
	 *  OS name
	 *
	 *  i.e. "Windows" or "Android"
	 */
	OS:				"Android",
	
	/* 
	 * 	OS version
	 *
	 * 	i.e. "2.3.6" or "4.3.1"
	 */
	OSVersion: 		"",
	
	/**
	 * 	true  	if application is runningn in WebView 
	 * 	false 	if application is running in browser
	 */	
	isNative:		false,
})
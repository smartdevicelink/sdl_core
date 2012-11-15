/**
 * @name FFW.iOSLoggingSupport
 *
 * @desc Logging support (e.g., with console.log() and Em.Logger.log() methods)
 * for iOS. The logs are printed to the app's logs.
 *
 * @category	Framework
 * @filesource	ffw/utils/iOSLoggingSupport.js
 * @version		2.0
 *
 * @author		Eugene Nikolsky
 */

/*
 * Turns on/off the logging support.
 */
var iOSLoggingSupportEnabled = false;

var isiOS = (navigator.userAgent.toLowerCase().indexOf('ipad') != -1) || (navigator.userAgent.toLowerCase().indexOf('iphone') != -1);
if (isiOS && iOSLoggingSupportEnabled) {
	// Based on http://stackoverflow.com/questions/6508313/javascript-console-log-in-an-ios-uiwebview/6508343#6508343
	console = new Object();
	console.log = function(log) {
		var iframe = document.createElement("iframe");
		iframe.setAttribute("src", "ios-log:#iOS#" + log);
		document.documentElement.appendChild(iframe);
		iframe.parentNode.removeChild(iframe);
		iframe = null;
	}

	console.debug = console.log;
	console.info = console.log;
	console.warn = console.log;
	console.error = console.log;
	Em.Logger.log = console.log;

	console.log('Hello! This is ' + navigator.userAgent);
}

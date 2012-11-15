/**
 * @name FFW.PreloadImages
 * 
 * @desc Functionality for dynamic image preloading
 * 
 * @category	Framework
 * @filesource	ffw/ImagePreloader.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

FFW.PreloadImages = Em.Object.create({
	// Buffer string
	buffer: '', 
	
	preload: function(){
		//Write all images elements to buffer string
		var i;
		for(i=DataImages.length;i--;) {
			 this.buffer += "<img src='"+DataImages[i]+"'/>";
		}
		//append all images within one refference to DOM
		this.appendToDOM();
	},
	appendToDOM: function(){
		var self = this;
		setTimeout(function(){
			$('#preloader')[0].innerHTML = self.buffer;
		},2000);
	}
});


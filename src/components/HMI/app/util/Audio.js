/**
 * @name MFT.Audio
 * 
 * @desc HTML5 Audio player
 * 
 * @category	utils
 * @filesource	app/util/Audio.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.Audio = {
	
	audio: new Audio(),
	
	play: function(path) {
		this.audio.src = path;
		this.audio.play();
	}
}
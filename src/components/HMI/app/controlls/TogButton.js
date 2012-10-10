/**
 * @name MFT.TogButton
 * 
 * @desc Universal toggle button component for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/TogButton.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.TogButton = MFT.Button.extend({
	classNameBindings: ['content.value:active','pressed','disabled']
});
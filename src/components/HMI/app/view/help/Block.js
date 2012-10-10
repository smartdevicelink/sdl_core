/**
 * @name MFT.HelpDivider
 * 
 * @desc Universal block for using in help mode
 * 
 * @category	View
 * @filesource	app/view/help/HelpBlock.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.HelpBlock = Em.View.extend({
	classNames:			'helpmode_block',
	
	classNameBindings:	['MFT.helpMode:visible']
});
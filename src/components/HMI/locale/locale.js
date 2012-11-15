/**
 * @name MFT.locale
 * 
 * @desc MFT localization object
 * 
 * @category	Localization
 * @filesource	locale/locale.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.locale = Em.Object.create({
	
	label: MFT[MFT.localization],
	
	setLang: function(lang) {
		this.set( 'label', MFT[MFT.localization] );
	}.observes('MFT.localization')		
});
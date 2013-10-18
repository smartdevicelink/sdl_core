/**
 * @name MFT.SettingsModel
 * 
 * @desc Settings model
 * 
 * @category	Model
 * @filesource	app/model/SettingsModel.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.SettingsModel = Em.Object.create({
	
	// temperature measurement units
	// 0 - Fahrenheit
	// 1 - Celsius	
	temperatureType:	0,
	// distance measurement units
	// 0 - Kilometres
	// 1 - Miles
	distanceType: 1,
    // display auto DIM
    // 0 - On
    // 1 - Off
    autoDim: 0,
    brightness: {
        value: 10
    },
    autoDimOffset: {
        value: 0
    },

	
	/**
	 * English version flag 
	 *
 	 * @type {Boolean}
	 */
	isEnglish: FLAGS.locale === 'eng',

    /**
     * Spanish version flag
     *
     * @type {Boolean}
     */
	isSpanish: FLAGS.locale === 'spa'
})
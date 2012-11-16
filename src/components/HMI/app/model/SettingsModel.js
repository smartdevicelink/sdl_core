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
	distanceType: 1
})
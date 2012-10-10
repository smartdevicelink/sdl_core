/**
 * @name MFT.SettingsData5_2
 * 
 * @desc Data model for MCS HMI concept 5.2
 * 
 * @category	Model
 * @filesource	app/model/settings/MCS/SettingsData5_2.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.SettingsData5_2 = Em.Object.extend({

	/** Variables for controls data */
	driverSeat: {
		0: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		1: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		2: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		3: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		4: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		5: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),

		6: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0})
	},

	passengerSeat: {
		0: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		1: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		2: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		3: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		4: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		5: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),

		6: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0})
	}
});
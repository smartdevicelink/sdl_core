/**
 * @name MFT.SettingsData4.1
 * 
 * @desc Data model for MCS HMI concept 4.1
 * 
 * @category	Model
 * @filesource	app/model/settings/MCS/SettingsData4_1.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.SettingsData5_1 = Em.Object.extend({

	/** Variables for controls data */
	driverSeat: {
		0: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		1: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		2: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		3: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		4: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		5: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),

		6: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),

		7: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		8: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		9: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		10: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		11: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		12: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		13: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0})
	},

	passengerSeat: {
		0: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		1: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		2: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		3: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		4: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		5: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),

		6: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),

		7: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		8: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		9: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		10: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		11: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		12: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0}),

		13: MFT.RangedValue.create({range: 2, value:1, cycle: false, minValue: 0})
	}
});
/**
 * @name MFT.SettingsData2.1
 * 
 * @desc Data model for MCS HMI concept 2.1
 * 
 * @category	Model
 * @filesource	app/model/settings/MCS/SettingsData2_1.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.SettingsData2_1 = Em.Object.extend({


	
	/** Preview mode */
	driverSeatPreviewMode: 0,
	
	/** Preview mode */
	passengerPreviewMode: 0,
	
	/** Variables for controls data */
	driverSeat: {
		0: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		1: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		2: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		3: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		4: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		5: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		6: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		7: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		8: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		9: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		// custom adjust
		
		10: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),

		11: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		12: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		13: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		14: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		15: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),
		
		16: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),
		
		// custom adjust preview

		17: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),

		18: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		19: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		20: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		21: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1})
		
	},

	passengerSeat: {
		0: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		1: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		2: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		3: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		4: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		5: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),

		6: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		7: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		8: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		9: MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 0}),
		
		// custom adjust
		
		10: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),

		11: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		12: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		13: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		14: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		15: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),
		
		16: MFT.RangedValue.create({range: 2, value:0, cycle: false, minValue: 0}),
		
		// custom adjust preview

		17: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),

		18: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		19: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		20: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1}),
		
		21: MFT.RangedValue.create({range: 10, value:1, cycle: false, minValue: 1})
	}
});
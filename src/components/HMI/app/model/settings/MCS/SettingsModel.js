/**
 * @name MFT.SettingsModel
 *
 * @desc  Multi contour seat model
 *
 * @category model
 * @filesource app/model/SettingsModel.js
 * @version		2.0
 *
 * @author	Melnik Andriy
 */

MFT.SettingsModel = Em.Object.create({

	/** Concept data models */
	conceptData: {
		1:	 MFT.SettingsData,
		1.1: MFT.SettingsData1_1,
		1.2: MFT.SettingsData1_2,
		1.3: MFT.SettingsData1_3,
		1.4: MFT.SettingsData1_4,
		1.5: MFT.SettingsData1_5,
		2.1: MFT.SettingsData2_1,
		2.2: MFT.SettingsData2_2,
		4.1: MFT.SettingsData4_1,
		5.1: MFT.SettingsData5_1,
		5.2: MFT.SettingsData5_2,
		6.2: MFT.SettingsData6_2,
		6.1: MFT.SettingsData6_1,
		7.1: MFT.SettingsData7_1
	},

	init: function() {
		var data = this.conceptData[FLAGS.MCS_ACTIVE_CONCEPT].create();

		this.set('driverSeat',		data.driverSeat);
		this.set('passengerSeat',	data.passengerSeat);
	},

	/**
	 * Handler for OFF LO HI buttons that switches cushions value
	 */
	changeMassageValue: function(index, value, seat){

		MFT.MCSController.get('activeSeat')[index-1].set('value', value);

	},

	/**
	 * Handler for plus button that increases indicators value
	 */
	onIncrease: function(index, seat){
		MFT.MCSController.get('activeSeat')[index-1].increase(); 
	},

	/**
	 * Handler for minus button that decreases indicators value
	 */ 
	onDecrease: function(index, seat){
		MFT.MCSController.get('activeSeat')[index-1].decrease();
	}
});
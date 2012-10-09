/**
 * @name MFT.ClimateModelCAN
 * 
 * @desc Climate CAN logic model
 * 
 * @category	Model
 * @filesource	app/model/ClimateModelCAN.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateModelCAN = MFT.ClimateData.create({

	/** Increase driver temperature */
	increaseDriverTemp: function() {
	
		MFT.CAN.send('driverTemp',this,this.driverTemp.value+1);
	},
	
	/** Decrease driver temperature */
	decreaseDriverTemp: function() {
		
		MFT.CAN.send('driverTemp',this,this.driverTemp.value-1);
	}
})
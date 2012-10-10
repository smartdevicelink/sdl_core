/**
 * @name MFT.ClimateData
 * 
 * @desc Climate data model
 * 
 * @category	Model
 * @filesource	app/model/ClimateData.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateData = Em.Object.extend({
	
	on:						MFT.RangedValue.create({ range: 1, value: 1 }),
	
	/** drivers values */	
	driverTemp:				MFT.ClimateTemperature.create(),
	myTemp:					MFT.ClimateTemperature.create({value: 75}),
	driverHeatedSeat: 		MFT.RangedValue.create(),
	driverCooledSeat: 		MFT.RangedValue.create(),
	driverHeatedSteer:		MFT.RangedValue.create({ range: 1 }),
	
	/* passenger values */
	passangerTemp:			MFT.ClimateTemperature.create(),
	passangerHeatedSeat: 	MFT.RangedValue.create(),
	passangerCooledSeat: 	MFT.RangedValue.create(),
	
	dualMode:				MFT.RangedValue.create({ range: 1}),
	
	/** airflow values */
	airflow: {
		
		mode: null,
		
		items: {
			defrost:		MFT.ToggleValue.create(),
			panel:			MFT.ToggleValue.create({ value: true }),
			feet:			MFT.ToggleValue.create(),
			both:			MFT.ToggleValue.create()
		}		
	},
	
	/** front */	
	windShield:				MFT.RangedValue.create({ range: 1 }),
	rearDeforost:			MFT.RangedValue.create({ range: 1 }),
	ac:						MFT.RangedValue.create({ range: 1 }),
	maxAc:					MFT.RangedValue.create({ range: 1 }),
	recirculation:			MFT.RangedValue.create({ range: 1 }),
	auto:					MFT.ToggleValue.create(),
	autoFan:				MFT.ToggleValue.create(),
	autoAirflow:			MFT.ToggleValue.create(),
	
	fan:					MFT.RangedValue.create({ range: 7, cycle: false }),
	
	/** Rear controls */
	rearOn:					MFT.RangedValue.create({ range: 1 }),
	rearControl:			MFT.RangedValue.create({ range: 1 }),
	
	rearTemp:				MFT.RangedValue.create({ range: 9, cycle: false }),
	rearFan:				MFT.RangedValue.create({ range: 7, cycle: false })

});
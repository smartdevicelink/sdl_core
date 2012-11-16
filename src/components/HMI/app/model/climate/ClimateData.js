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
			defrost:		MFT.RangedValue.create({ range: 1 }),
			panel:			MFT.RangedValue.create({ range: 1, value: 1 }),
			feet:			MFT.RangedValue.create({ range: 1 }),
			both:			MFT.RangedValue.create({ range: 1 }),
		}		
	},
	
	simpleAirflowPanel:		MFT.RangedValue.create({ range: 1, value: 1 }),
	simpleAirflowFeet:		MFT.RangedValue.create({ range: 1 }),
	
	/** front */	
	windShield:				MFT.RangedValue.create({ range: 1 }),
	maxDefrost:				MFT.RangedValue.create({ range: 1 }),
	rearDeforost:			MFT.RangedValue.create({ range: 1 }),
	ac:						MFT.RangedValue.create({ range: 1 }),
	maxAc:					MFT.RangedValue.create({ range: 1 }),
	recirculation:			MFT.RangedValue.create({ range: 1 }),
	auto:					MFT.RangedValue.create({ range: 1 }),
	autoFan:				MFT.RangedValue.create({ range: 1 }),
	autoAirflow:			MFT.RangedValue.create({ range: 1 }),
	
	fan:					MFT.RangedValue.create({ range: 7, cycle: false }),
	
	/** Rear controls */
	rearOn:					MFT.RangedValue.create({ range: 1 }),
	rearControl:			MFT.RangedValue.create({ range: 1 }),
	
	rearTemp:				MFT.RangedValue.create({ range: 9, cycle: false }),
	rearFan:				MFT.RangedValue.create({ range: 7, cycle: false })
});
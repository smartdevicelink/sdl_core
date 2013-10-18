/**
 * @name MFT.NAvigationModel
 * 
 * @desc Navigation logic model
 * 
 * @category	Model
 * @filesource	app/model/NavigationModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.NavigationModel = Em.Object.create({
	
	typeView: 	MFT.RangedValue.create(),
	
	mapView: 	MFT.RangedValue.create(),
	
	zoomRate: 	MFT.NavigationMapZoom.create(),
	
	mute:  		MFT.RangedValue.create({range:1}),
	
	distance_total: MFT.NavigationDistance.create({value:15}),
	
	distance_left: MFT.NavigationDistance.create({value:3.1}),
	
	distance_to_next_turn: MFT.NavigationDistance.create({value:0.7}),
	
	distance_to_next_street: MFT.NavigationDistance.create({value:0.1}),
	
	distance_to_prev_street: MFT.NavigationDistance.create({value:400, distanceType:0})

})
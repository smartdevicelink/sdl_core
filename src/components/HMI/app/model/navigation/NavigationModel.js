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
	
	init: function(){
		this.zoomRate.set('value', 0.55);
		
		this._super();
	},
	
	typeView: 	MFT.RangedValue.create(),
	
	mapView: 	MFT.RangedValue.create(),
	
	mute:  		MFT.RangedValue.create({range:1}),
	
	zoomRate: 	MFT.NavigationMapZoom.create()
	
})
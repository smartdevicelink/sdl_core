/**
 * @name NavigationMapZoom
 * 
 * @desc NavigationMapZoom class
 * 		 this class is designed to be used for navigation map zoom logic 
 * 
 * @category	Model
 * @filesource	app/model/navigation/NavigationMapZoom.js
 * @version		2.0
 *
 * @author 		Igor zhavoronkin
 */
MFT.NavigationMapZoom = Em.Object.extend({
	
	value: MFT.NavigationDistance.create({value:0.55}),
	
	maxZoom:  MFT.NavigationDistance.create({value:0.25}),
	
	minZoom:  MFT.NavigationDistance.create({value:0.55}),
	
	step:  MFT.NavigationDistance.create({value:0.10.toFixed(2)}),
	
	zoomIn: function(){
		if(this.value.value >this.maxZoom.value)
		{
			
			this.value.set('value' , Number((this.value.value - Number(this.step.value)).toFixed(2)));
			
			MFT.NavigationController.set('zoomMap','zoom_'+(this.value.value * 100).toFixed(0));
		
		}else
		{
			
			return false;
		
		}
	},
	
	zoomOut: function(){
		if(this.value.value < this.minZoom.value)
		{
			this.value.set('value' , Number((this.value.value + Number(this.step.value)).toFixed(2)));
			
			MFT.NavigationController.set('zoomMap','zoom_'+(this.value.value * 100).toFixed(0));
		
		}else
		{
			MFT.NavigationController.set('zoomMap','zoom_55');
		}
	}
});
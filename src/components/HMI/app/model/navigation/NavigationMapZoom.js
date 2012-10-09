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
	
	maxZoom: 0.25,
	
	minZoom: 0.55,
	
	value : 0.55,
	
	zoomIn: function(){
		if(this.value >this.maxZoom){
			this.set('value' , Number((this.value - 0.10).toFixed(2)));
			MFT.NavigationController.set('zoomMap','zoom_'+(this.value * 100));
		}else{
			return false;
		}
	},
	
	zoomOut: function(){
		if(this.value < this.minZoom){
			this.set('value' , Number((this.value + 0.10).toFixed(2)));
			MFT.NavigationController.set('zoomMap','zoom_'+(this.value * 100));
		}else{
			MFT.NavigationController.set('zoomMap','zoom_55');
		}
	}
})
/**
 * @name NavigationDistance
 *
 * @desc Navigation distance
 *       Designed to fomat object value into string and to check for distance value type change
 * 
 * @category	Model
 * @filesource	app/model/navigation/NavigationDistance.js
 * @version		2.0
 * @author 		Igor Zhavoronkin
 */
 
MFT.NavigationDistance = Em.Object.extend({
	
	value: 0, //  distance value
	
	floatPoint: 0, // needs to determine a number of floating digits to be displayed
	
	shortLable: 'mi', // short distance lable string 
	
	longLable: 'Miles', // long distance lable string,
	
	distanceType: 1, // 1 - miles  or kilometres, 0 - foot or metres 
	
	init: function(){
		// set floating point variable
		if(this.value.toString().split('.')[1])
		{
			this.set('floatPoint',  this.value.toString().split('.')[1].length);
		}else
		{
			this.set('floatPoint',  0);
		}
		// If distance type == 0 change lable to ft as default
		if(!this.distanceType){
			
			this.set('shortLable','ft');
		}
		
	},
	
	/** On Distance Type Change observer */
	onDistanceTypeChange: function(){
		
		if(MFT.SettingsModel.distanceType) // Distance value to Miles
		{ 
			if(this.distanceType){
				this.set('value', this.toMiles(this.value,this.floatPoint));
			}else{
				this.set('value', this.toFt(this.value,this.floatPoint));
			}
		}else // Distance value to Miles
		{
			if(this.distanceType){
				this.set('value', this.toKilometres(this.value,this.floatPoint));
			}else{
				this.set('value', this.toMetres(this.value,this.floatPoint));
			}
		}
	
	}.observes('MFT.SettingsModel.distanceType'),
	
	
	/**  Form String output for distance lable */
	toggleLableTypes: function(){
		if(MFT.SettingsModel.distanceType)
		{	
			if(this.distanceType){
				this.set('longLable', 'Miles');
				this.set('shortLable', 'mi');
			}else{
				this.set('shortLable', 'ft');
			}

		}else
		{
			if(this.distanceType){	
				this.set('longLable', MFT.locale.label.view_navigation_kilometres );
				this.set('shortLable', 'km');
			}else{
				this.set('shortLable', 'm');
			}
		}
	
	}.observes('MFT.SettingsModel.distanceType'),
	
	/**
 	 * convert Miles to KM
 	 * @return Number
 	 */
 	toKilometres: function(distance,floatPoint) {
 		distance = ( distance * 1.6 );
 		return Number(distance.toFixed(floatPoint));
 	},
	
	/**
 	 * convert  KM to Miles
 	 * @return Number
 	 */
 	toMiles: function(distance,floatPoint) {
 		distance = ( distance / 1.6 );
 		return Number(distance.toFixed(floatPoint));
 	},
	
	toFt: function(distance,floatPoint){
		distance = ( distance * 3.2 );
 		return Number(distance.toFixed(floatPoint));
	},
	
	toMetres: function(distance,floatPoint){
		distance = ( distance / 3.2 );
 		return Number(distance.toFixed(floatPoint));
	}
	
});
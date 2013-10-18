/**
 * @name MFT.ClimateView
 * 
 * @desc Climate module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/ClimateView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateView = Em.ContainerView.create(MFT.LoadableView,{

	elementId:	'climate',
	
	childViews: [
		'storeTemp'
    ],
    
    /** Store temp popup */
	storeTemp: Em.View.extend({
		elementId:	'climate_storeTemp',
		template: Em.Handlebars.compile('{{MFT.locale.label.view_climate_myTempStored}}')
	})
});
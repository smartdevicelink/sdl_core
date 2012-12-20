/**
 * @name MFT.SliderView
 * 
 * @desc Slider visual representation
 * 
 * @category	View
 * @filesource	app/view/applink/shared/sliderView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.SliderView = MFT.ApplinkAbstractView.create({
	
	elementId: 'slider_view',
	
	childViews: [
		'backButton',
		'captionText'
	]
});

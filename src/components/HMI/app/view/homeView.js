/**
 * @name MFT.HomeView
 * 
 * @desc Home screen visual representation
 * 
 * @category	View
 * @filesource	app/view/home/HomeView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */
 
 MFT.HomeView = Em.ContainerView.create(MFT.LoadableView,{
 
 	elementId:	'home',
 	 	
 	/** Bind class for visual representation */
	classNameBindings: [
		'MFT.helpMode:help'
	],
	
	childViews: [
		'widgetPhone',
		'widgetNavigation',
		'widgetMedia',
		'widgetClimate'
	],
	
	widgetPhone:		MFT.WidgetPhoneView,
	
	widgetNavigation:	MFT.WidgetNavigationView,
	
	widgetMedia:		MFT.WidgetMediaView,
	
	widgetClimate:		MFT.WidgetClimateView,
});


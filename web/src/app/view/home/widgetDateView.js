/**
 * @name MFT.WidgetDateView
 * 
 * @desc Date home screen widget visual representation
 * 
 * @category	View
 * @filesource	app/view/home/WidgetDateView.js
 * @version		2.0
 *
 * @author		Andrii Velychko
 */

MFT.WidgetDateView = Em.ContainerView.extend({
	
	elementId:	'home_widgetDate',
	
    classNameBindings: [
        'MFT.AppController.activeVehicleModel.isClimateDisabled::hidden'
    ],
	
	childViews: [
		'date'
	],

    date: MFT.Label.create({
		elementId:				'home_widgetDate_date',
		contentBinding:			Em.Binding.oneWay('MFT.AppController.getCurrentDate')
	}),

    /**
     * Select settings state
     * should be called when user press home date block
     */
    actionUp: function(){
        if (MFT.helpMode) return false;

        MFT.States.goToState( MFT.SettingsController.get('activeState') );
    }

});
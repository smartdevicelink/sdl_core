/**
 * @name MFT.StatusSettingsView
 * 
 * @desc Settings statusbar visual representation
 * 
 * @category	View
 * @filesource	app/view/home/StatusSettingsView.js
 * @version		2.0
 *
 * @author		Andrii Velychko
 */

MFT.StatusSettingsView = Em.ContainerView.extend({
	
	elementId:	'status_settings',
	
	classNames: ['status_bar'],
	
	/** Bind class for visual representation */
	classNameBindings: [
        'isVisible:visible_display'
    ],

    isVisible: function(){
        return !(MFT.States.settings.active || MFT.StartUpController.isAllStatusHidden) && MFT.AppController.get('activeVehicleModel').isClimateDisabled;
    }.property('MFT.AppController.activeVehicleModel.isClimateDisabled', 'MFT.StartUpController.isAllStatusHidden', 'MFT.States.settings.active'),

	childViews: [
		'statusHome',
        'date'
	],

	/** Label for Home screen */
	statusHome: MFT.Label.create({
		elementId:		'status_settings_label',

		classNameBindings: ['MFT.States.home.active::hidden'],

		contentBinding:	    Ember.Binding.oneWay('MFT.locale.label.view_statusSettings_settings')
	}),

    date: MFT.Label.create({
        elementId:		'status_settings_date',

        classNameBindings: ['MFT.States.home.active:hidden'],

        contentBinding:	    Ember.Binding.oneWay('MFT.AppController.getCurrentDate')
    }),


	/**
	 * Select climate state
	 * should be called when user press climate statusbar
	 */
	actionUp: function(event){
		MFT.States.goToState( MFT.SettingsController.get('activeState') );
	}
});
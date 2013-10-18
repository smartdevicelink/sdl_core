/**
 * @name MFT.StatusInfoView
 * 
 * @desc Component for Information Status bar
 * 
 * @category	view
 * @filesource	app/view/navigation/StatusNavigationView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */
MFT.StatusInfoView = Em.ContainerView.extend({
	/** View Id */
	elementId:	'status_info',
	
	classNames: ['status_bar','hidden_info'],

	classNameBindings: [
		'FFW.Backend.isNavigationEnabled::active_info',
		'MFT.States.info.active:selected'
	],
	
	/** Navigation components */		
	childViews: [
		'statusHome'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.create({
		elementId:		'status_info_label',
        contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_statusInformation_information')
	}),
	
	actionUp: function(event){
		MFT.States.goToState(MFT.InfoController.activeState);
	}
});
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
	
	classNames: ['status_bar','hidden'],

	classNameBindings: [
		'FFW.Backend.isNavigationEnabled::active_state',
		'MFT.States.info.active:selected'
	],
	
	/** Navigation components */		
	childViews: [
		'statusHome'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.create({
		elementId:		'status_info_label',
		content:		'Information'
	}),
	
	actionUp: function(event){
		MFT.States.goToState(MFT.InfoController.activeState);
	}
});
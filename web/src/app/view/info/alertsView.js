/**
 * @name MFT.InfoAlertsView
 * 
 * @desc Info Alerts module visual representation
 * 
 * @category	View
 * @filesource	app/view/sinfo/InfoAlertsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.InfoAlertsView = Em.ContainerView.create(MFT.LoadableView,{

	elementId:		'info_alerts',
	
	childViews: [
		'listCaption',
		'mainlist',
		'rightlist'
	],
	
	listCaption: MFT.LabelPlusToggleButton.extend({
		elementId:			'info_alerts_listCaption',
		labelContentBinding:  'MFT.locale.label.view_info_alerts_sort',
		tButtonValue:		 0,
		tButtonRange: 		 2,
		tButtonLabelsBinding: 'MFT.locale.label.view_info_alerts_dateIcon',
		labelDisabled: 		 true,
		tButtonDisabled:	   true
	}),
	
	mainlist:	MFT.List.extend({
			
			elementId:	  'info_alerts_list',
			
			itemsOnPage:	6,
			
			items:[
				{
					type:		MFT.Label,
			
					params:		{
						disabled: 	true
					}			
				},
				{
					type:		MFT.Button,
					params:		{
						classNames:  ['ember-view ffw-button notpressed list-item disabled active'],
						classNameBindings:  'MFT.SettingsModel.isSpanish:hidden',
						textBinding: 'MFT.locale.label.view_info_alerts_911Assist',
						templateName: 'rightIcon',
						icon:		 'images/info/ico_emergcyAndEnvelope.png',
						righticon:	'images/info/ico_arrowLock.png',
						disabled:	 true
					}							
				}
			]
	}),
	
	rightlist: MFT.List.extend({
			
			elementId:	  'info_alerts_rightList',
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
				{
					type:		MFT.Button,
					params:		{
						className:  'button',
						textBinding: 'MFT.locale.label.view_info_alerts_view',
						disabled:	true
					}							
				},
				{
					type:		MFT.Button,
					params:		{
						className:  'button',
						textBinding: 'MFT.locale.label.view_info_alerts_delete',
						disabled:	true
					}							
				},
				{
					type:		MFT.Button,
					params:		{
						className:  'button',
						textBinding: 'MFT.locale.label.view_info_alerts_deleteAll',
						disabled:	true
					}							
				}
			]			
	})
});
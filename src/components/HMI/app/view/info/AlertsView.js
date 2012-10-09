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
 
MFT.InfoAlertsView = Em.View.create({
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.info.alerts.active:active_state','MFT.helpMode'],
	
	elementId:		'info_alerts',
	
	classNames:	   'hidden',
	
	activeDisabled: function(){
		if (!MFT.helpMode) {
			return true;
		}
				 
	}.property('MFT.helpMode'),
	
	template: Ember.Handlebars.compile(
			'<div class="list-item">'+
				'{{view MFT.LablePlusToggleButton '+
					'labelContentBinding="MFT.locale.label.view_info_alerts_sort" '+
					'tButtonValue=0 '+
					'tButtonRange=2 '+
					'tButtonLabelsBinding="MFT.locale.label.view_info_alerts_dateIcon" '+
					'labelDisabled=true '+
					'tButtonDisabled=true '+
				'}}'+
			
				'{{view MFT.Button '+
					'textBinding="MFT.locale.label.view_info_alerts_911Assist" '+
					'elementId="info_alerts_911Assist_button" '+
					'classNames="button" '+
					'action="" '+
					'target="" '+
					'icon="images/info/ico_emergcyAndEnvelope.png" '+
					'righticon="images/info/ico_arrowLock.png" '+
					'disabled=true '+
				'}} '+	
			'</div>'+
		
		'{{view MFT.ScrollBar '+	
			'currentPage=0 '+
			'pageCount=1 '+
			'listHeight= 301 '+
			'scrollBarIsDisabledBinding="MFT.helpMode" '+
		'}} '+
		
		'<div id="viewControllButtons">'+
			'{{view MFT.Button '+
				'textBinding="MFT.locale.label.view_info_alerts_view" '+
				'elementId="view_info_alerts_view_button" '+
				'classNames="button" '+
				'action="" '+
				'target="" '+
				'disabled=true '+
			'}} '+	
			'{{view MFT.Button '+
				'textBinding="MFT.locale.label.view_info_alerts_delete" '+
				'elementId="info_alerts_delete_button" '+
				'classNames="button" '+
				'action="" '+
				'target="" '+
				'disabled=true '+
			'}} '+	
			'{{view MFT.Button '+
				'textBinding="MFT.locale.label.view_info_alerts_deleteAll" '+
				'elementId="info_alerts_deleteAll_button" '+
				'classNames="button" '+
				'action="" '+
				'target="" '+
				'disabled=true '+
			'}} '+	
		'</div>'
	)
});
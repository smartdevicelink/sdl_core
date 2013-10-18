/**
 * @name MFT.ServicesView
 * 
 * @desc Info Services module visual representation
 * 
 * @category	Info
 * @filesource	app/view/ServicesView.js
 * @version		2.0
 *
 * @author		Gerashchenko Maksym
 */
 
MFT.InfoServicesView= Em.View.create(MFT.LoadableView,{
	
	elementId:	'info_services_view',
	
	/** Define module template */
	template: Ember.Handlebars.compile(
			'<div class="inner-content">'+
				'<div id="info_sync_services">{{MFT.locale.label.view_info_services_syncServices}}</div>'+
				'<div id="info_traff">{{MFT.locale.label.view_info_services_trafficDirectionsAndInformation}}</div>'+
				'<div id="info_traffic_report">'+
					'<p>{{MFT.locale.label.view_info_services_getTheLatestTrafficRreport}}</p>'+
					'<p>{{MFT.locale.label.view_info_services_receive}}</p>'+
					'<p>{{MFT.locale.label.view_info_services_hearPersonalizedInformationMore}}</p>'+
					'<br/>'+
					'<p>{{MFT.locale.label.view_info_services_toActivate}}</p>'+
				'</div>'+
				'<div id="info_connect" class="disabled">{{MFT.locale.label.view_info_services_connecttoServices}}</div>'+
			'</div>'
	)
})
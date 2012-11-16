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
	
	classNameBindings: ['MFT.helpMode'],
	
	/** Define module template */
	template: Ember.Handlebars.compile(
			'<div class="inner-content">'+
				'<div id="info_sync_services" class="info-text">{{MFT.locale.label.view_info_services_syncServices}}</div>'+
				'<div id="info_traff" class="info-text">{{MFT.locale.label.view_info_services_trafficDirectionsAndInformation}}</div>'+
				'<div id="info_traffic_report" class="info-text">{{MFT.locale.label.view_info_services_getTheLatestTrafficRreport}}</div>'+
				'<div id="info_traffic_pts" class="info-text">{{MFT.locale.label.view_info_services_findBusinessesAndDownloadSaved}}<br>{{MFT.locale.label.view_info_services_Pts}}</div>'+
				'<div id="info_traffic_hear" class="info-text">{{MFT.locale.label.view_info_services_hearPersonalizedInformationMore}}</div>'+
				'<div id="info_connect" class="info-text">{{MFT.locale.label.view_info_services_connecttoServices}}</div>'+
				'<div id="info_log" class="info-text">{{MFT.locale.label.view_info_services_toActivate}}</div>'+
			'</div>'
	),
})
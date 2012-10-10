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
	
	stateObj: MFT.States.info.services,
	
	classNameBindings: ['MFT.helpMode'],
		
	classNames: 'hidden',

	/** Define module template */
	template: Ember.Handlebars.compile(
			'<div class="inner-content">'+
				'<div id="info_sync_services" class="info-text">SYNC Services</div>'+
				'<div id="info_traff" class="info-text">TRAFFIC, DIRECTIONS AND INFORMATION</div>'+
				'<div id="info_traffic_report" class="info-text">Get the latest traffic report</div>'+
				'<div id="info_traffic_pts" class="info-text">Find Businesses and Download Saved <br>Pts</div>'+
				'<div id="info_traffic_hear" class="info-text">Hear Personalized Information &amp; More</div>'+
				'<div id="info_connect" class="info-text">Connect to Services</div>'+
				'<div id="info_log" class="info-text">To activate, log onto www.SyncMyRide.com</div>'+
			'</div>'
	),
})
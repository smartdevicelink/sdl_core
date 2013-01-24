/**
 * @name MFT.StatusMediaView
 * 
 * @desc Component for Media Status bar
 * 
 * @category	view
 * @filesource	app/view/media/StatusMediaView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.StatusMediaView =  Em.ContainerView.extend({

	elementId:	'media_status',

	classNameBindings: ['MFT.States.media.active:selected'],

	/** Climate components */		
	childViews: [
		'statusHome',
		'statusInfo'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.extend({
		elementId:	'media_status_homestatus',
		classNameBindings: ['MFT.States.home.active:visible'],		
		contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_mediaStatus_entertainment')
	}),
	
	statusInfo: Em.ContainerView.extend({
		elementId:	'media_status_radio_container',
		
		childViews: [
			'info'
		],
		
		info: Em.View.extend({
			elementId:	'media_status_radio_info',
			controlerBinding: 'MFT.MediaController',
			/** Bind class for visual representation */	
			classNameBindings: ['MFT.States.home.active:hidden_display:visible_display'],
			/** is AVIN active*/
			isAV: function(){return (this.controler.activeState === 'media.avin');}.property('controler.activeState'),
			/** Define module layout */
			template:  Em.Handlebars.compile(
				'<div class="statusInfo">'+
					'<div class="station">'+
						'<span {{bindAttr class="MFT.CDModel.active:visible_display MFT.SDModel.active:visible_display MFT.USBModel.active:visible_display"}}>{{MFT.MediaController.currentModuleData.selectedItem.title}}</span>'+ // title
						'<span {{bindAttr class="MFT.BTModel.active:visible_display"}}>{{MFT.locale.label.view_mediaStatus_bluetooth}}</span>'+  // BLUETOOTH
						'<span {{bindAttr class="view.isAV:visible_display"}}{{MFT.locale.label.view_mediaStatus_avIn}}</span>'+ 		//AV IN
						'<span {{bindAttr class="MFT.SiriusModel.active:visible_display MFT.AmModel.active:visible_display MFT.FmModel.active:visible_display"}}>{{view.controler.currentActiveData.frequency}}</span>'+   // frequency
						'<span {{bindAttr class="MFT.ApplinkMediaController.model.active:visible_display"}}>{{MFT.ApplinkMediaController.model.statusText}}</span>'+ 		//Applink
					'</div>'+
					'<div class="icon  antenaIco"'+
						'{{bindAttr class="MFT.CDModel.active:cdIco"}}'+ // CD icon
						'{{bindAttr class="MFT.SDModel.active:sdIco"}}'+ // SD icon
						'{{bindAttr class="MFT.ApplinkMediaController.model.active:cdIco"}}'+ // Applink icon
						'{{bindAttr class="MFT.USBModel.active:usbIco"}}'+ // USB icon
						'{{bindAttr class="MFT.BTModel.active:btIco"}}'+ // BLUETOOTH icon
						'{{bindAttr class="view.isAV:avIco"}}'+ // AVIN icon
						'{{bindAttr class="view.controler.currentModuleData.selectedItem.isHd:statushdicon"}}'+ // HD icon
						'{{bindAttr class="MFT.SiriusModel.active:siriusIco"}}>'+ // Sirius icon
					'</div>'+
				'</div>'
			)
		})
	}),
	actionUp: function(event){
		MFT.States.goToState(MFT.MediaController.activeState);
	}
});
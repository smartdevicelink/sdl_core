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
		'statusInfo',
		
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.extend({
		elementId:	'media_status_homestatus',
		classNameBindings: ['MFT.States.home.active:visible'],		
		content:			'Entertainment'
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
			/** is BLUETOOTH active*/
			isBT: function(){return (this.controler.activeState === 'media.bluetooth');}.property('controler.activeState'),
			/** is AVIN active*/
			isAV: function(){return (this.controler.activeState === 'media.avin');}.property('controler.activeState'),
			/** Define module layout */
			template:  Em.Handlebars.compile(
				'<div class="statusInfo">'+
					'<div class="station">'+
						'<span {{bindAttr class="MFT.CDModel.active:visible_display MFT.SDModel.active:visible_display MFT.USBModel.active:visible_display"}}>{{MFT.MediaController.currentModuleData.selectedItem.title}}</span>'+ // title
						'<span {{bindAttr class="view.isBT:visible_display"}}>Bluetooth</span>'+  // BLUETOOTH
						'<span {{bindAttr class="view.isAV:visible_display"}}>AV In</span>'+ 		//AV IN
						'<span {{bindAttr class="MFT.SiriusModel.active:visible_display MFT.AmModel.active:visible_display MFT.FmModel.active:visible_display"}}>{{view.controler.currentActiveData.frequency}}</span>'+   // frequency
					'</div>'+
					'<div class="icon  antenaIco"'+
						'{{bindAttr class="MFT.CDModel.active:cdIco"}}'+ // CD icon
						'{{bindAttr class="MFT.SDModel.active:sdIco"}}'+ // SD icon
						'{{bindAttr class="MFT.USBModel.active:usbIco"}}'+ // USB icon
						'{{bindAttr class="view.isBT:btIco"}}'+ // BLUETOOTH icon
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
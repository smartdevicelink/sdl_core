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

	classNameBindings: ['MFT.States.media.active:selected','MFT.StartUpController.isAllStatusHidden:hidden'],

	/** Climate components */		
	childViews: [
		'statusHome',
		'radioPreset',
		'statusInfo'		
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.extend({
		elementId:	'media_status_homestatus',
		classNameBindings: ['MFT.States.home.active:visible','MFT.MediaController.isHomeMediaStatusHidden:hidden'],	
		contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_mediaStatus_entertainment')
	}),
	
	radioPreset: MFT.Label.extend({
		elementId:		'media_status_preset_message',
		classNames:		'bottom_message',
		classNameBindings: ['MFT.MediaController.updatingMessageIsVisible:messageIsVisible'],
		contentBinding:	  Ember.Binding.oneWay('MFT.MediaController.updatingMessage')
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
			classNameBindings: ['MFT.States.home.active:hidden_display:visible_display', 'MFT.MediaController.isHomeMediaStatusInfoHidden:hidden'],
			/** is AVIN active*/
			isAV: function(){return (this.controler.activeState === 'media.avin');}.property('controler.activeState'),
			/** Is Radio is Active*/
			isRadioActive: function(){
				return this.controler.activeState.indexOf('radio') >= 0;
			}.property('controler.activeState'),
			
			isSiriusActive: function(){
				return this.controler.activeState.indexOf('sirius') >= 0;
			}.property('controler.activeState'),
			
			isCDActive: function(){
				return this.controler.activeState.indexOf('cd') >= 0;
			}.property('controler.activeState'),
			
			isSDActive: function(){
				return this.controler.activeState.indexOf('sd') >= 0;
			}.property('controler.activeState'),
			
			isUSBActive: function(){
				return this.controler.activeState.indexOf('usb') >= 0;
			}.property('controler.activeState'),
			
			isPlayerActive: function(){
				return this.controler.activeState.indexOf('player') >= 0 && this.controler.activeState.indexOf('bluetooth') === -1;
			}.property('controler.activeState'),	
			
			isBTActive: function(){
				return this.controler.activeState.indexOf('bluetooth') >= 0;
			}.property('controler.activeState'),
			
			isHdIconActive: function(){
				return this.controler.currentModuleData.items[this.controler.currentModuleData.selectedIndex].isHd && this.get('isRadioActive');
			}.property('this.controler.currentModuleData.selectedIndex','this.isRadioActive'),
			
			/** Define module layout */
			template:  Em.Handlebars.compile(
				'<div class="statusInfo">'+
					'<div class="station">'+
						'<span {{bindAttr class="view.isPlayerActive:visible_display"}}>{{view.controler.currentPlpayerModuleData.selectedItem.title}}</span>'+ // title
						'<span {{bindAttr class="view.isBTActive:visible_display"}}>{{MFT.locale.label.view_mediaStatus_bluetooth}}</span>'+  // BLUETOOTH
						'<span {{bindAttr class="view.isAV:visible_display"}}{{MFT.locale.label.view_mediaStatus_avIn}}</span>'+ 		//AV IN
						'<span {{bindAttr class="view.isRadioActive:visible_display "}}>{{view.controler.currentActiveData.frequency}}' +
                        '{{#if view.controler.currentActiveData.isHd}}-2{{/if}}'+
                        '</span>'+   // frequency
					'</div>'+
					'<div class="icon  antenaIco"'+
						'{{bindAttr class="view.isCDActive:cdIco"}}'+ // CD icon
						'{{bindAttr class="view.isSDActive:sdIco"}}'+ // SD icon
						'{{bindAttr class="view.isUSBActive:usbIco"}}'+ // USB icon
						'{{bindAttr class="view.isBTActive:btIco"}}'+ // BLUETOOTH icon
						'{{bindAttr class="view.isAV:avIco"}}'+ // AVIN icon
						'{{bindAttr class="view.isHdIconActive:statushdicon"}}'+ // HD icon
						'{{bindAttr class="view.isSiriusActive:siriusIco"}}>'+ // Sirius icon
					'</div>'+
				'</div>'
			)
		})
	}),
	
	actionUp: function(event){
		/** 
		 *  Skip Media Options state on Media Module Enter if Media Option was last state on 
		 *  Media Module exit event
		*/
		var path = MFT.MediaController.activeState;
		
		if( path.indexOf('options') >= 0 ){
			MFT.MediaController.set('activeState',path.substring(0, path.indexOf('options')-1 ) );
		}
		
		MFT.States.goToState(MFT.MediaController.activeState);

	}
});
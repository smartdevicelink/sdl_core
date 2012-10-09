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
			classNameBindings: ['isActive:visible_display'],
			
			classNames: 'hidden_display',
			
			/** check for active home module */	
			isActive: function(){return (MFT.States.home.active === false)}.property('MFT.States.currentState'),
			
			/** Is Sirius stations active*/
			isSirius : function(){
				return (this.controler.activeState === 'media.Sirius' ||
						this.controler.activeState === 'media.Sirius.browse' ||
						this.controler.activeState === 'media.Sirius.browse.browseAll' ||
						this.controler.activeState === 'media.Sirius.options'
						);	
			}.property('controler.activeState'),
			
			/** Is Media Player module stations active*/
			isCDsGroup : function(){
				return (this.controler.currentMediaGroup === 'CDs');
			}.property('controler.currentMediaGroup'),

			/** Is Radio (AM/FM/SIRIUS) module stations active*/
			isRadioGroup: function(){
				return (this.controler.currentMediaGroup === 'radio');
			}.property('controler.currentMediaGroup'),
			
			/** is CD active*/
			isCD: function(){
					return (this.controler.activeState.match('cd') != null);
				   }.property('controler.activeState'),
			/** is USB active*/
			isUSB: function(){
					return (this.controler.activeState.match('usb') != null);
				   }.property('controler.activeState'),
			/** is SD active*/
			isSD: function(){
					return (this.controler.activeState.match('sd') != null);
				   }.property('controler.activeState'),
			/** is BLUETOOTH active*/
			isBT: function(){return (this.controler.activeState === 'media.bt' || this.controler.activeState === 'media.bt.options');}.property('controler.activeState'),
			/** is AVIN active*/
			isAV: function(){return (this.controler.activeState === 'media.avin');}.property('controler.activeState'),
			/** Define module layout */
			template:  Em.Handlebars.compile(
				'<div class="statusInfo">'+
					'<div class="station">'+
						'<span {{bindAttr class="view.isCDsGroup:visible_display"}}>{{MFT.MediaController.currentModuleData.selectedItem.title}}</span>'+ // title
						'<span {{bindAttr class="view.isBT:visible_display"}}>Bluetooth</span>'+  // BLUETOOTH
						'<span {{bindAttr class="view.isAV:visible_display"}}>AV In</span>'+ 		//AV IN
						'<span {{bindAttr class="view.isRadioGroup:visible_display"}}>{{view.content.frequency}}</span>'+   // frequency
					'</div>'+
					'<div class="icon  antenaIco"'+
						'{{bindAttr class="view.isCD:cdIco"}}'+ // CD icon
						'{{bindAttr class="view.isSD:sdIco"}}'+ // SD icon
						'{{bindAttr class="view.isUSB:usbIco"}}'+ // USB icon
						'{{bindAttr class="view.isBT:btIco"}}'+ // BLUETOOTH icon
						'{{bindAttr class="view.isAV:avIco"}}'+ // AVIN icon
						'{{bindAttr class="view.content.isHd:statushdicon"}}'+ // HD icon
						'{{bindAttr class="view.isSirius:siriusIco"}}>'+ // Sirius icon
					'</div>'+
				'</div>'
			),
			content: function(){
				if(this.controler.directTuneSelected){
					return this.controler.currentModuleData.get('selectedDirectItem');
				}else{
					return this.controler.currentModuleData.get('selectedItem');
				}
			}.property( 'controler.currentModuleData.selectedItem',
						'controler.currentModuleData.selectedDirectItem',
						'controler.directTuneSelected'
					 ),
		})
	
	}),
	
	actionUp: function(event){
		MFT.States.goToState(MFT.MediaController.activeState);
	}
});
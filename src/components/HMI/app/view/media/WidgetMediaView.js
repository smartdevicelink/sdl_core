/**
 * @name MFT.WidgetMediaView
 * 
 * @desc Component for Media Home Widget
 * 
 * @category	view
 * @filesource	app/view/media/WidgetMediaView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.WidgetMediaView = Em.ContainerView.extend({
	/** Element Id*/
	elementId:	'home_widgetMedia',
	
	/** Media components */		
	childViews: [
		'radio',
		'cd',
		'other',
		'SavePresetPopup'
	],
	/** AM , FM , Sirius modules */
	radio:  Em.ContainerView.extend({
		/** Element Id */
		elementId: 'home_widgetMedia_radio',
		/** Visual representation is current radio group selected*/
		classNameBindings: 	['currentMediaGroup:active_state'],
		classNames:			'hidden',
		/** Check for current media group */
		currentMediaGroup: function(){
			return (MFT.MediaController.currentMediaGroup === 'radio');
		}.property('MFT.MediaController.currentMediaGroup'),
		/** View components*/
		childViews: [
			'homePresets',
			'info'
		],
		/** Info */
		info: Em.View.extend({
			/** Element Id*/
			elementId: 'home_widgetMedia_radio_info',
			
			controlerBinding: 'MFT.MediaController',
			/** Check for Sirius module*/
			isSirius : function(){
				return (this.controler.activeState === 'media.Sirius' ||
						this.controler.activeState === 'media.Sirius.browse' ||
						this.controler.activeState === 'media.Sirius.browse.browseAll' ||
						this.controler.activeState === 'media.Sirius.options'
					   );
			}.property('controler.activeState'),
			
			/** Template */
			template:  Em.Handlebars.compile(
				'{{#with view}}'+
					'<div class="stationInfo">'+
						'<div id="sirius_cont" class="hidden_display" {{bindAttr class="isSirius:visible_display"}}>'+
							'<div id="help_sirius" class="hidden_display" {{bindAttr class="MFT.helpMode:visible_display"}}>'+
								'<div class="siriusico_help"></div>'+
								'<img class="logo hidden_display" {{bindAttr class="controler.isStationLogo:visible_display"}}  src="images/media/help/titanium-logo-home-blur.png"}}>'+
							'</div>'+
							'<div id="sirius_normal" {{bindAttr class="MFT.helpMode:hidden_display"}}>'+
								'<div class="siriusico"></div>'+
								'<img class="logo hidden_display"  {{bindAttr class="controler.isStationLogo:visible_display"}} src="images/media/sirius-logo-h.png">'+
							'</div>'+
						'</div>'+
						'<div id="others_cont" {{bindAttr class="isSirius:hidden_display"}} >'+
					    '<div class="stationimg_help hidden_display" {{bindAttr class="controler.currentActiveData.isHd:homehdicon_blur MFT.helpMode:visible_display"}}></div>'+
						'<div class="stationimg" {{bindAttr class="controler.currentActiveData.isHd:homehdicon MFT.helpMode:hidden_display"}}></div>'+
						'</div>'+	
						'<span class="station" {{bindAttr class="controler.isStationLogo:hidden MFT.helpMode:helpmode_blur_text"}}>{{controler.currentActiveData.frequency}}</span>'+
					'</div>'+
				'{{/with}}'
			 )
		}),
		/** Preset Buttons*/
		homePresets: MFT.HomePresetView.extend({
			classNames: 'home-preset-buttons-wraper',
		})
	}),
	/** CD modules (CD, USB , SD)*/
	cd: Em.View.extend({
		/** Element Id*/
		elementId: 'home_widgetMedia_cd',
		/** Visual representation is current cd group selected*/
		classNameBindings: 	[
			'currentMediaGroup:active_state',
			'MFT.helpMode:helpmode_blur_text'
		],
		controlerBinding: 'MFT.MediaController',
		classNames:			'hidden',
		/** Check for current media group */
		currentMediaGroup: function(){
			return (this.controler.currentMediaGroup === 'CDs');
		}.property('controler.currentMediaGroup'),
		/** Template*/
		template:  Em.Handlebars.compile(
			'{{#with view}}'+
				'<div class="info">'+
					'<div class="ico_holder">'+
						'{{#if MFT.helpMode}}'+
							'<img class="ico_blur" {{bindAttr src="controler.currentModuleData.homeWidgetIcon_blur"}} />'+
						'{{else}}'+
							'<img class="ico" {{bindAttr src="controler.currentModuleData.homeWidgetIcon"}} />'+
						'{{/if}}'+				
					'</div>'+
					'<div class="info_holder">'+
						'<div class="album_title">{{controler.currentModuleData.selectedItem.album}}</div>'+
						'<div class="song_title">{{controler.currentModuleData.selectedItem.title}}</div>'+
					'</div>'+
				'</div>'+
			'{{/with}}'
		)
	
	}),
	/** Other Modules (Bluetooth , AV IN)*/
	other: Em.View.extend({
		/** Element Id*/
		elementId: 'home_widgetMedia_others',
		/** Visual representation is current others group selected*/
		classNameBindings: 	[
			'currentStateOnMediaExit:active_state',
			'MFT.helpMode:helpmode_blur_text'
		],
		
		classNames:			'hidden',
		/* Check for selected module - Bluetooth or AV IN*/
		currentStateOnMediaExit: function(){
				if(MFT.MediaController.activeState == 'media.avin'){
					if(MFT.helpMode){
						this.set('ico', 'images/media/help/line-h-ico_blur.png' );
					}else{
						this.set('ico', 'images/media/line-h-ico.png' );
					}
					this.set('title', 'AV In' );
						
				}else{
					if(MFT.helpMode){
						this.set('ico', 'images/media/help/bt-h-ico_blur.png' );
					}else{
						this.set('ico', 'images/media/bt-h-ico.png' );
					}
					this.set('title', 'Bluetooth' );
				}
				
				return ( MFT.MediaController.activeState === 'media.avin' || 
						 MFT.MediaController.activeState === 'media.bt' || 
						 MFT.MediaController.activeState === 'media.bt.options'
					   );
		}.property('MFT.MediaController.activeState','MFT.helpMode'),
		
		/** Template */
		template:  Em.Handlebars.compile(
			'<div class="info">'+
				'<div class="ico_holder">'+				
					'<img class="ico" {{bindAttr src="view.ico"}} {{bindAttr class="MFT.helpMode:ico_blur"}} />'+
				'</div>'+
				'<div class="info_holder">'+
					'<div class="title">{{view.title}}</div>'+
				'</div>'+
			'</div>'
		)
	}),
	
	/** Save Preset Popup*/
	SavePresetPopup: Em.View.extend({
		elementId: 'home_widgetMedia_presetPopup',
		template:  Em.Handlebars.compile(
			'<div class="preset-stored" style="top:-40px;">Preset saved</div>'
		)
	})
});
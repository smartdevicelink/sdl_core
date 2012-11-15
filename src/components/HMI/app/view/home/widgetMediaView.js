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
		'bluetooth',
		'avin',
		'SavePresetPopup'
	],
	/** AM , FM , Sirius modules */
	radio:  Em.ContainerView.extend({
		/** Element Id */
		elementId: 'home_widgetMedia_radio',
		/** Visual representation is current radio group selected*/
		classNameBindings: 	['MFT.AmModel.active:active_state',
								'MFT.FmModel.active:active_state',
								'MFT.SiriusModel.active:active_state'
								],
		
		classNames:			'hidden',
		
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
			
			/** Template */
			template:  Em.Handlebars.compile(
				'{{#with view}}'+
					'<div class="stationInfo">'+
						'<div id="sirius_cont" class="hidden_display" {{bindAttr class="MFT.SiriusModel.active:visible_display"}}>'+
							'<div id="help_sirius" class="hidden_display" {{bindAttr class="MFT.helpMode:visible_display"}}>'+
								'<div class="siriusico_help"></div>'+
								'<img class="logo hidden_display" {{bindAttr class="controler.isStationLogo:visible_display"}}  src="images/media/help/titanium-logo-home-blur.png"}}>'+
							'</div>'+
							'<div id="sirius_normal" {{bindAttr class="MFT.helpMode:hidden_display"}}>'+
								'<div class="siriusico"></div>'+
								'<img class="logo hidden_display"  {{bindAttr class="controler.isStationLogo:visible_display"}} src="images/media/sirius-logo-h.png">'+
							'</div>'+
						'</div>'+
						'<div id="others_cont" {{bindAttr class="MFT.SiriusModel.active:hidden_display"}} >'+
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
			'MFT.CDModel.active:active_state',
			'MFT.SDModel.active:active_state',
			'MFT.USBModel.active:active_state',
			'MFT.helpMode:helpmode_blur_text'
		],
		controlerBinding: 'MFT.MediaController',
		
		classNames:			'hidden',
		
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
	/**  BLUETOOTH */
	bluetooth: Em.View.extend({
		/** Element Id*/
		elementId: 'home_widgetMedia_bluetooth',
		/** Visual representation is current others group selected*/
		classNameBindings: 	[
			'MFT.BTModel.active:active_state',
			'MFT.helpMode:helpmode_blur_text'
		],
		classNames:			'hidden',
		
		/** Template */
		template:  Em.Handlebars.compile(
			'<div class="info">'+
				'<div class="ico_holder">'+				
					'<div class="bluetooth_ico" {{bindAttr class="MFT.helpMode:ico_blur"}}></div>'+
				'</div>'+
				'<div class="info_holder">'+
					'<div class="title">Bluetooth</div>'+
				'</div>'+
			'</div>'
		)
	
	}),
	
	/**  AV IN */
	avin: Em.View.extend({
		/** Element Id*/
		elementId: 'home_widgetMedia_avin',
		/** Visual representation is current others group selected*/
		classNameBindings: 	[
			'isAvin:active_state',
			'MFT.helpMode:helpmode_blur_text'
		],
		
		classNames:			'hidden',
		/* Check for selected module - Bluetooth or AV IN*/
        isAvin: function(){
			return (MFT.MediaController.activeState === 'media.avin');
        }.property('MFT.MediaController.activeState'),
		
		/** Template */
		template:  Em.Handlebars.compile(
			'<div class="info">'+
				'<div class="ico_holder">'+				
					'<div class="avin_ico" {{bindAttr class="MFT.helpMode:ico_blur"}}></div>'+
				'</div>'+
				'<div class="info_holder">'+
					'<div class="title">AV In</div>'+
				'</div>'+
			'</div>'
		)
	}),
	
	/** Save Preset Popup*/
	SavePresetPopup: Em.View.extend({
		classNameBindings: 'MFT.localization',
		elementId: 'home_widgetMedia_presetPopup',
		template:  Em.Handlebars.compile(
			'<div class="preset-stored" style="top:-40px;">{{MFT.locale.label.view_media_popUp}}</div>'
		)
	})
});
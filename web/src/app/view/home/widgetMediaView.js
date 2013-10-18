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
		'avin'
	],
	/** AM , FM , Sirius modules */
	radio:  Em.ContainerView.extend({
		/** Element Id */
		elementId: 'home_widgetMedia_radio',
		/** Visual representation is current radio group selected*/
		classNameBindings: 	['isRadioActive:active_state'],
		
		classNames:			'hidden',
		
		/** Is Radio is Active*/
		isRadioActive: function(){
			return MFT.MediaController.activeState.indexOf('radio') >= 0;
		}.property('MFT.MediaController.activeState'),
		
		
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
			
			isSiriusSelected: function(){
				return MFT.MediaController.activeState.indexOf('sirius') >= 0;
			}.property('controler.activeState'),
			
			/** Template */
			template:  Em.Handlebars.compile(
				'{{#with view}}'+
					'<div class="stationInfo">'+
						'<div id="sirius_cont" class="hidden_display" {{bindAttr class="view.isSiriusSelected:visible_display"}}>'+
							'<div id="sirius_normal">'+
								'<div class="siriusico"></div>'+
								'<img class="logo hidden_display"  {{bindAttr class="controler.isStationLogo:visible_display"}} src="images/media/sirius-logo-h.png">'+
							'</div>'+
						'</div>'+
						'<div id="others_cont" {{bindAttr class="view.isSiriusSelected:hidden_display"}} >'+
						'<div class="stationimg" {{bindAttr class="controler.currentActiveData.isHd:homehdicon"}}></div>'+
						'</div>'+	
						'<span class="station" {{bindAttr class="controler.isStationLogo:hidden"}}>' +
                            '{{controler.currentActiveData.frequency}}' +
                            '{{#if controler.currentActiveData.isHd}}' +
                                '-2' +
                            '{{/if}}' +
                        '</span>'+
					'</div>'+
				'{{/with}}'
			 )
		}),
		/** Preset Buttons*/
		homePresets: MFT.HomePresetView.extend({
			classNames: 'home-preset-buttons-wraper'
		})
	}),
	/** CD modules (CD, USB , SD)*/
	cd: Em.View.extend({
		/** Element Id*/
		elementId: 'home_widgetMedia_cd',
		/** Visual representation is current cd group selected*/
		classNameBindings: 	[
			'controler.isPlayerActive:active_state'
		],
		controlerBinding: 'MFT.MediaController',
		
		classNames:			'hidden',
		
		/** Template*/
		template:  Em.Handlebars.compile(
				'<div class="info">'+
					'<div class="ico_holder">'+
							'<img class="ico" {{bindAttr src="view.controler.currentPlpayerModuleData.homeWidgetIcon"}} />'+
					'</div>'+
					'<div class="info_holder">'+
						'<div class="album_title">{{view.controler.currentPlpayerModuleData.selectedItem.album}}</div>'+
						'<div class="song_title">{{view.controler.currentPlpayerModuleData.selectedItem.title}}</div>'+
					'</div>'+
				'</div>'
		)
	
	}),
	/**  BLUETOOTH */
	bluetooth: Em.View.extend({
		/** Element Id*/
		elementId: 'home_widgetMedia_bluetooth',
		/** Visual representation is current others group selected*/
		classNameBindings: 	[
			'isBTActive:active_state'
		],
		
		isBTActive: function(){
			return MFT.MediaController.activeState.indexOf('bluetooth') >= 0;
		}.property('MFT.MediaController.activeState'),
		
		classNames:			'hidden',
		
		/** Template */
		template:  Em.Handlebars.compile(
			'<div class="info">'+
				'<div class="ico_holder">'+				
					'<div class="bluetooth_ico"></div>'+
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
			'isAvin:active_state'
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
					'<div class="avin_ico"></div>'+
				'</div>'+
				'<div class="info_holder">'+
					'<div class="title">{{MFT.locale.label.view_media_avi}}</div>'+
				'</div>'+
			'</div>'
		)
	}),

    /**
     * Select media state
     * should be called when user press home media block
     */
    actionUp: function(event){
        if (MFT.helpMode) return false;

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
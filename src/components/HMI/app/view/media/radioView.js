/**
 * @name MFT.AMFMInfoView
 * 
 * @desc Media AM/FM INFO module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/AMFMInfoView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.radioView = Em.ContainerView.create(MFT.LoadableView,{
	
	/** View Id */	
	elementId: 'radio_view',
		
	/** View Components*/
	childViews: [
		'AMFM',
		'Sirius'
	],
	
	AMFM:Em.View.extend({
		
		elementId: 'amfm_view',
		
		/** Bind class for visual representation */
		classNameBindings: [
							 'MFT.FmModel.active:active_state',
							 'MFT.AmModel.active:active_state',
							 'MFT.helpMode:helpmode_blur_text'
							],
		
		/** Class Name*/
		classNames: ['hidden','fm-info'],
		/** Media Controller binding*/
		controlerBinding: 'MFT.MediaController',
		
		/** Define Template*/	
		template:  Em.Handlebars.compile(
				'<div class="info_wrapper" {{bindAttr class="view.controler.directKeypressed:hidden_display"}}>'+
					'<div class="hdInfo hidden_display" {{bindAttr class="view.controler.currentActiveData.isHd:visible_display"}}>'+
						'<div class="fmInfoHdIco" {{bindAttr class="MFT.helpMode:fmInfoHdIco_helpmode"}}></div>'+
						'<div class="hdtitle">{{view.controler.currentActiveData.title}}</div>'+
						'<div class="hdartist">{{view.controler.currentActiveData.artist}}</div>'+
					'</div>'+
					'{{view MFT.Button target="MFT.MediaController"'+ 
						'action="turnTagHelpVideoOn" classNames="btn_tag not-visible"'+
						'classNameBindings="MFT.helpMode:btn_tag_help view.controler.currentActiveData.isHd:visible" onDown=false}}'+
					'<div class="nonHDsonginfo not-visible" {{bindAttr class="MFT.FmModel.active:visible view.controler.currentActiveData.isHd:hidden"}}>{{view.songInfo}}</div>'+	
					'<span class="STAName not-visible" {{bindAttr class="MFT.FmModel.active:visible view.controler.currentActiveData.isHd:hidden"}}>STA-{{view.STAName}}</span>'+
					'<div class="fmgenre">{{view.controler.currentActiveData.genre}}</div>'+
				'</div>'+	
				'<div class="divider_o"></div>'+	
				
				'<div id="band">'+
					'{{#if view.controler.directKeypressed}}'+
						'{{view.controler.directTuneToString}}'+
					'{{else}}'+
						'{{view.controler.currentActiveData.frequency}}'+
					'{{/if}}'+
				'</div>'
				
				
		),
			
			/** Formate Station name according to HD or non HD State of Station*/	
			STAName: function(){
				if(this.controler.get('currentActiveData'))	
					return this.controler.get('currentActiveData').frequency.toString().replace('.','');
			}.property('controler.currentActiveData'),
		
			/** Formate Song name according to HD or non HD State of Station*/
			songInfo: function(){
				if(this.controler.get('currentActiveData'))
					return  this.controler.get('currentActiveData').title + ' - ' +  this.controler.get('currentActiveData').artist;
			}.property('controler.currentActiveData')
	}),
	
	Sirius : Em.View.create({
		
			elementId: 'sirius_view',
			/** Bind class for visual representation */
			classNameBindings: 	['MFT.SiriusModel.active:active_state','MFT.helpMode:helpmode_blur_text'],
			/** Class Name*/
			classNames: ['hidden','media_content','sirius-info'],
			/** controler binding*/
			controlerBinding: 'MFT.MediaController',
			
			/** Define Template*/	
			template:  Em.Handlebars.compile(
					'<div class="siriusband" {{bindAttr class="view.controler.isStationLogo:hidden_display"}}>'+
						'{{view.controler.currentActiveData.frequency}}'+
					'</div>'+
					'<div>'+
						'<div class="channel" {{bindAttr class="view.controler.directKeypressed:hidden_display"}}>{{view.controler.currentActiveData.channel}}</div>'+
						'<div class="channel hidden_display"{{bindAttr class="view.controler.directKeypressed:visible_display"}}>'+
							'Channel {{view.controler.directTuneToString}}'+
						'</div>'+
						
						
						'<div class="siriusgenre">{{view.controler.currentActiveData.genre}}</div>'+
						'<div class="titanium" {{bindAttr class="view.controler.isStationLogo:visible_display"}} {{bindAttr class="MFT.helpMode:logo_blur"}} {{bindAttr src="MFT.SiriusModel.sir1.selectedItem.logo"}}></div>'+
						'<div class="siriustitle">{{view.controler.currentActiveData.title}}</div>'+
						'<div class="siriusartist">{{view.controler.currentActiveData.artist}}</div>'+
						'{{view MFT.Button '+
							'target="MFT.MediaController"'+
							'action="turnAlertHelpVideoOn" '+
							'classNameBindings="MFT.helpMode:help" '+
							'classNames="button btn_alert" '+
							'onDown=false '+
							'textBinding="MFT.locale.label.view_media_sirius_alert" '+
							/* Binnding to helpMode invert function */
							'disabledBinding="MFT.RightMenuView.replayButton.disabled" '+							
						'}}'+ 
						'{{view MFT.Button '+
								'elementId="media_sirius_alert_help_yellow_border" '+
								'classNameBindings="MFT.helpMode:active_state" '+
								'classNames="hidden" '+
								'onDown=false '+
						'}} '+
					'</div>'	
			)
	})
	
});
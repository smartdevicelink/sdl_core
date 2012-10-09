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
 
MFT.AMFMInfoView = Em.View.create({
	
		/** View Id */	
		elementId: 'media_am-fm_info_view',
		
		/** Bind class for visual representation */
		classNameBindings: [
							 'MFT.States.media.Fm.active:active_state',
							 'MFT.States.media.Am.active:active_state',
							 'MFT.helpMode:helpmode_blur_text'
							],
		
		/** Class Name*/
		classNames: ['hidden','fm-info'],
		/** Media Controller binding*/
		controlerBinding: 'MFT.MediaController',
		
		/** Define Template*/	
		template:  Em.Handlebars.compile(
			'{{#with view}}'+
				'<div class="info_wrapper" {{bindAttr class="controler.directKeypressed:hidden_display"}}>'+
					'<div class="hdInfo hidden_display" {{bindAttr class="controler.currentActiveData.isHd:visible_display"}}>'+
						'<div class="fmInfoHdIco" {{bindAttr class="MFT.helpMode:fmInfoHdIco_helpmode"}}></div>'+
						'<div class="hdtitle">{{controler.currentActiveData.title}}</div>'+
						'<div class="hdartist">{{controler.currentActiveData.artist}}</div>'+
					'</div>'+
					'{{view MFT.Button target="MFT.MediaController"'+ 
						'action="turnTagHelpVideoOn" classNames="btn_tag not-visible"'+
						'classNameBindings="MFT.helpMode:btn_tag_help controler.currentActiveData.isHd:visible" onDown=false}}'+
					'<div class="nonHDsonginfo not-visible" {{bindAttr class="MFT.States.media.Fm.active:visible controler.currentActiveData.isHd:hidden"}}>{{songInfo}}</div>'+	
					'<span class="STAName not-visible" {{bindAttr class="MFT.States.media.Fm.active:visible controler.currentActiveData.isHd:hidden"}}>STA-{{STAName}}</span>'+
					'<div class="fmgenre">{{controler.currentActiveData.genre}}</div>'+
				'</div>'+	
				'<div class="divider_o"></div>'+	
				'<div id="band" {{bindAttr class="controler.directTuneHide:visible_display"}}>{{controler.currentActiveData.frequency}}</div>'+
				'<div id="band_dir" {{bindAttr class="controler.directTuneHide:hidden_display"}}>{{controler.directTuneToString}}</div>'+
			'{{/with}}'
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
});
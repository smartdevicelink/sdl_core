/**
 * @name MFT.SiriusView
 * 
 * @desc Media Sirius module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/SiriusView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
MFT.SiriusView = Em.ContainerView.create({

	elementId: 'media_sirius_info',
	/** Bind class for visual representation */
	classNameBindings: 	['MFT.States.media.Sirius.active:active_state','MFT.helpMode:helpmode_blur_text'],
	/** Class Name*/
	classNames: ['hidden','media_content','sirius-info'],
	/** controller binding*/
	controllerBinding: 'MFT.MediaController',
	/** Define Template*/	

	childViews: [
		'content'
	],

	content: Em.View.create({

		template:  Em.Handlebars.compile(
			'{{#with view}}'+
				'<div class="siriusband" {{bindAttr class="controller.isStationLogo:hidden_display controller.directKeypressed:hidden"}}>'+
					'{{controller.currentActiveData.frequency}}'+
				'</div>'+
				'<div class="directune_band" {{bindAttr class="controller.directKeypressed:visible_display"}}>'+
					'{{controller.directTuneToString}}'+
				'</div>'+
				'<div {{bindAttr class="controller.directKeypressed:hidden_display"}} >'+
					'<div class="channel">{{controller.currentActiveData.channel}}</div>'+
					'<div class="siriusgenre">{{controller.currentActiveData.genre}}</div>'+
					'<div class="titanium" {{bindAttr class="controller.isStationLogo:visible_display"}} {{bindAttr class="MFT.helpMode:logo_blur"}} {{bindAttr src="MFT.SiriusModel.sir1.selectedItem.logo"}}></div>'+
					'<div class="siriustitle">{{controller.currentActiveData.title}}</div>'+
					'<div class="siriusartist">{{controller.currentActiveData.artist}}</div>'+
					'{{view MFT.Button target="MFT.MediaController"'+
					'action="turnAlertHelpVideoOn" classNames="btn_alert" classNameBindings="MFT.helpMode:btn_alert_help" onDown=false}}'+
				'</div>'+
			'{{/with}}'	
		),
	})
});
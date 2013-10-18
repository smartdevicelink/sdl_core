/**
 * @name MFT.CDOptionsView
 * 
 * @desc Media FM Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/cd/optionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.CDOptionsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_cd_options_view',
	
	classNames: ['cd','media_options_view'],
	
	customInsertElementBinding: 'MFT.MediaView',

    childViews: [
        'backButton',
        'topTitile',
        'list'
    ],

    topTitile: MFT.Label.extend({
		elementId:		'media_option_cd_title_header',
		classNames:		  ['block-header-title','media_option_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_cd_options_title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_cd_options_list',
			
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click','list'],
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_bt_options_soundSettings',
						templateName:		  'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LabelPlusToggleButton,
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_cd_options_compression',
						tButtonValue: 		 1,
						tButtonRange:		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_cd_options_onOff'
					}		
				}
			]
 
	}),
	
	backButton: MFT.Button.extend({
		elementId: 			'media_options_cd_back_btn',
		classNames:		  ['backButton','button','media_options_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	})
});
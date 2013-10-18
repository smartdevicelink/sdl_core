/**
 * @name MFT.PTYCategoryView
 * 
 * @desc Media FM Options Set PTY module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/sirius/options/PTYCategoryView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.FMPTYCategoryView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_fm_options_PTYCategory_view',
	
	classNames: ['media_options_PTYCategory_view', 'fm'],
		
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'listTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'media_fm_options_PTYCategory_back_btn',
		classNames:		   ['backButton','button','media_options_PTYCategory_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',
		icon:				 'images/media/ico_back.png'
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		'media_options_fm_PTYCategory_title_header',
		classNames:		  ['block-header-title','media_options_PTYCategory_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_amFm_options_PTYCategory_title')
	}),	
	
	listTitile: MFT.Label.extend({
		elementId:		'media_options_fm_PTYCategory_listTitile',
		classNames:		  ['block-header-title','media_options_PTYCategory_listTitile'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_amFm_options_PTYCategory_listLabel')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_options_fm_PTYCategory_list',
			
			classNames: 'media_options_PTYCategory_list',
			
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click'],
			
			itemsOnPage:	5,
			
			items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_noProgramType',
						classNames: ['ffw-button','notpressed','list-item','active-category'],
						disabled: true				
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_information',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_sports',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_talk',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_rock',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_classicRock',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_adultRock',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_softRock',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_top40',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_country',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_oldies',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_soft',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_nostalgia',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_jazz',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_classical',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_RnB',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_softRnB',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_language',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_religiousMusic',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_religiousTalk',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_personality',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_public',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_college',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_weather',
						disabled: true
					}							
				}
	
			]
 
	})
});
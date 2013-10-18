/**
 * @name MFT.PTYCategoryView
 * 
 * @desc Media Sirius Options Set PTY module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/sirius/options/PTYCategoryView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.PTYCategoryView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_sirius_options_PTYCategory_view',
	
	classNames: 'media_options_PTYCategory_view',
		
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'listTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'media_sirius_options_PTYCategory_back_btn',
		classNames:		   ['backButton','button','media_options_PTYCategory_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',
		icon:				 'images/media/ico_back.png'
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		'media_options_sirius_PTYCategory_title_header',
		classNames:		  ['block-header-title','media_options_PTYCategory_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_sirius_options_setCategory_title')
	}),	
	
	listTitile: MFT.Label.extend({
		elementId:		'media_options_sirius_PTYCategory_listTitile',
		classNames:		  ['block-header-title','media_options_PTYCategory_listTitile'],
		contentBinding:	  Ember.Binding.oneWay('MFT.MediaController.currentPTYCategoryData.listTtitle')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_options_sirius_PTYCategory_list',
			
			classNames: 'media_options_PTYCategory_list',
			
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click'],
			
			itemsOnPage:	5,
			
			items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_allChanels',
						classNames: ['ffw-button','notpressed','list-item','active-category'],
						disabled: true				
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_pop',
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
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_hipHopRnB',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_danceElectronic',
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
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_christian',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_jazzStandards',
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
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_familyHealth',
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
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_comedy',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_howardStern',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_entertainment',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_newsPublicRadio',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_politics',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_religion',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_trafficWeather',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_more',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_latinWorld',
						disabled: true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_PTYCategory_canadian',
						disabled: true
					}							
				}	
			]
 
	})
});
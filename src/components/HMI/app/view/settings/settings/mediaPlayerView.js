/**
 * @name MFT.SettingsSettingsMediaPlayerView
 * 
 * @desc Settings Settings Media Player module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsSettingsMediaPlayerView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsSettingsMediaPlayerView = Em.ContainerView.create(MFT.LoadableView,{
	elementId:			'settings_settings_mediaPlayer',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],						  
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'back',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer',
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_mediaPlayer_list',
			
			classNameBindings: ['MFT.helpMode: hidden'],
			
			itemsOnPage:	5,
			
			items:[
			{
								
				type:		 MFT.Label,

				params:		{
					contentBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer_autoplay',
					icon:		'images/settings/ico_on-off-wide.png',
					templateName: 'icon',
					disabled:    true
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
					textBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices',
					templateName: 'arrow',
					goToState:   'bluetoothDevices',
					action:	  'onChildState',
					target:	  'MFT.SettingsController',
					helpMode: true,
				}							
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer_indexBluetooth',
					icon:	   'images/settings/ico_on-off-wide.png',
					templateName: 'icon',
					disabled:    true
				}
			},
				{
				type:		MFT.Button,
				params:		{
					textBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer_gracenote',
					disabled: true
				}
			},
			{
				type:		MFT.Label,
				params:		{
					contentBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer_gracenoteMgmt',
					icon:	   'images/settings/ico_on-off-wide.png',
					templateName: 'icon',
					disabled:    true
				}
			},
			{
				
				type:		MFT.LabelPlusToggleButton,
				params:		{
					labelContentBinding:  'MFT.locale.label.view_settings_settings_mediaPlayer_coverArtPriority',
					tButtonValue: 		 0,
					tButtonRange:		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_mediaPlayer_coverArtPriorityTriger',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}		
								
			}
		]
	})	
});
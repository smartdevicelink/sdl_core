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
    classNames: 'settings-list-block',

	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],						  
	backButton: MFT.Button.extend({
	elementId: 			'settings_settings_mediaPlayer_backButton',	
	classNames:		  ['backButton','button'],		
	action:			  'goToPreviousState',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'settings_settings_mediaPlayer_title',
		classNames:		  ['block-header-title'],
		contentBinding:      Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_mediaPlayer'),
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'settings_settings_mediaPlayer_list',
			
			itemsOnPage:	5,

            disableScrollbar: true,
			
			items:[
			{
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_mediaPlayer_autoplay',
					tButtonValue:		 0,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_mediaPlayer_onOff',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}			
					
			},
			{
				type:		MFT.Button,
				params:		{
					textBinding:      'MFT.locale.label.view_settings_settings_mediaPlayer_bluetoothDevices',
					templateName: 'arrow',
					stateName:   'phoneSettings.bluetoothDevices',
					action:	  'subState',
					target:	  'MFT.PhoneController',
					helpMode: true
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
								
				type:		MFT.LabelPlusToggleButton,
				params:		{
					classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
					labelContentBinding:  'MFT.locale.label.view_settings_settings_mediaPlayer_gracenoteMgmt',
					tButtonValue:		 1,
					tButtonRange: 		 2,
					tButtonLabelsBinding: 'MFT.locale.label.view_settings_settings_mediaPlayer_onOff',
					labelDisabled: 		 true,
					tButtonDisabled:	   true
				}			
					
			},
			{
				
				type:		MFT.LabelPlusToggleButton,
				params:		{
                    classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
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
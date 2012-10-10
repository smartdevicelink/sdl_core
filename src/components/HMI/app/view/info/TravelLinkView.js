/**
 * @name MFT.InfoTravelLinkView
 * 
 * @desc Info Travel Link Help module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/InfoTravelLinkView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.InfoTravelLinkView = Em.ContainerView.create(MFT.LoadableView,{
	
	classNameBindings: ['MFT.States.info.travelLink.active:active_state','MFT.helpMode'],
	
	elementId:	'info_travelLink',
		
	classNames: 'hidden',
	
	stateObj: MFT.States.info.travelLink,
	
	childViews: [
		'imageLogo',
		'listLeft',
		'listRight',
	],
	
	imageLogo: MFT.Label.extend({
		elementId:	'info_travelLink_logo',	
		icon: 		 'images/info/info_travelLink_sirius_logo.png'
	}),
	
	listLeft: MFT.List.extend({
		elementId:		   'info_travelLink_listLeft',
		
		disableScrollbar:	true,
		
		itemsOnPage:	5,
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_trafficOnRoute',
					arrow:			true,
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_trafficNearby',
					arrow:			true,
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_fuelPrices',
					arrow:			true,
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_movieListings',
					arrow:			true,
					disabled:         true,
				}			
					
			},
		],
	}),
	
	listRight: MFT.List.extend({
		elementId:		   'info_travelLink_listRight',
		
		disableScrollbar:	true,
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
				
				params:		{
					
					textBinding: 	  'MFT.locale.label.view_info_travelLink_subscriptionInfo',
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_weather',
					arrow:			true,
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_sportsInfo',
					arrow:			true,
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_skiConditions',
					arrow:			true,
					disabled:         true,
				}			
					
			},
		]
	})
});
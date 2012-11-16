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
	
	classNameBindings: ['MFT.helpMode','MFT.localization'],
	
	elementId:	'info_travelLink',
		
	childViews: [
		'imageLogo',
		'listLeft',
		'listRight',
	],
	
	imageLogo: MFT.Label.extend({
		elementId:	'info_travelLink_logo',	
		icon: 		 'images/info/info_travelLink_sirius_logo.png',
		templateName: 'icon',
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
					templateName: 'arrow',
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_trafficNearby',
					templateName: 'arrow',
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_fuelPrices',
					templateName: 'arrow',
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_movieListings',
					templateName: 'arrow',
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
					templateName: 'arrow',
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_sportsInfo',
					templateName: 'arrow',
					disabled:         true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					textBinding: 	  'MFT.locale.label.view_info_travelLink_skiConditions',
					templateName: 'arrow',
					disabled:         true,
				}			
					
			}
		]
	})
});
/**
 * @name MFT.SettingsHelpSoftwareLicensesView
 * 
 * @desc Settings Help Software Licenses module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsHelpSoftwareLicensesView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.SettingsHelpSoftwareLicensesView = Em.ContainerView.create(MFT.LoadableView,{
	classNameBindings:	['MFT.States.settings.help.softwareLicenses.active:active_state'],
	classNames:		   ['hidden'],
	dataListBinding: 	  'MFT.SettingsData.clocklist',
	elementId:			'settings_help_softwareLicenses_view',
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'installUpdatesButton',
							  'list'
						  ],	
						  			
	stateObj: MFT.States.settings.help.softwareLicenses,
										  
	backButton: MFT.Button.extend({
	classNames:		  ['backButton','button'],		
	action:			  'back',
	target:			  'MFT.States',	
	icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		content:			 'Software Licenses'
	}),
	
	installUpdatesButton: MFT.Button.extend({
		text:				'Install Updates',
		classNames:		  [,'button'],
		elementId:	       'installUpdatesButton',
		/*goToState:	       'settings.settings.system.installApplications',*/
		action:			  'onState',
		target:			  'MFT.SettingsController',
	}),
					  
	
	list:	MFT.List.extend({
			
			elementId:	  'settingsHelpSoftWareLicensesList',
			
			itemsOnPage:	 5,
			
			disableScrollbar:	false,
				
			items:[
			{
								
				type:		 MFT.Label,
				params:		{

					content:	    'Navigation North Amer',
				}			
					
			}
			]
	})	

		
});
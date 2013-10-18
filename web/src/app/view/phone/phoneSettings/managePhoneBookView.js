/**
 * @name MFT.PhoneSettingsManagePhoneBookView
 * 
 * @desc Phone Settings Manage Phone Book module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/options/ManagePhoneBookView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.PhoneSettingsManagePhoneBookView = Em.ContainerView.create( MFT.LoadableView, {
		
	//classNameBindings:	['MFT.helpMode','FLAGS.locale'],
	elementId:			'phone_settings_phoneBook',
	
	classNames:			['phone_view'],
	
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'phone_settings_phoneBook_backButton',
		classNames:		   ['backButton','button'],		
		action:			   'onBackState',
		target:			   'MFT.PhoneController',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'phone_phoneSettings_managePhoneBook_title',
		classNames:		  ['block-header-title'],
		contentBinding:	  'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_title'
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'phone_settings_phoneBook_list',
			
			itemsOnPage:	5,
						
			items:[
				
				{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
						labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_auto',
						tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_onOf',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				},	
				
				{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						classNames:		   ['lableToggleButton', 'list-item', 'ember-view', 'wideDoubleToggle'],
						labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_display',
						tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_onOf',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				},	
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_reDownload',
						buttonTexBindingt:	'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_download',
						labelDisabled:		true,
						buttonDisabled: 	   true,
						buttonClasses: 		['single']
					}							
				},

				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_addContacts',
						buttonTexBindingt:	'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_add',
						labelDisabled:		true,
						buttonDisabled: 	   true,
						buttonClasses: 		['single']
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_deletePhonebook',
						buttonTexBindingt:	'MFT.locale.label.view_phone_phoneSettings_managePhoneBook_delete',
						labelDisabled:		true,
						buttonDisabled: 	   true,
						buttonClasses: 		['single']
					}							
				},
				
			]
	})
});
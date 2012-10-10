/**
 * @name MFT.PhonebookPhoneView
 * 
 * @desc Phone Phonebook module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/PhonebookView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.PhonebookPhoneView = Em.ContainerView.create( MFT.LoadableView, {
		
	elementId:	'phone_phonebook',
		
	classNames: ['hidden'],
	
	stateObj: MFT.States.phone.phonebook,
	
	childViews: [
		'help',
		'content'
	],
	
	help: Em.View.extend({
		elementId:		'phone_phonebook_help',
		
		classNames:			'help',
		
		classNameBindings: ['MFT.helpMode:visible']		
	}),
	
	content: Em.ContainerView.extend({
		
		elementId:	'phone_phonebook_content',
		
		childViews: [
			'filters',
			'list'
		],
		
		filters: Em.View.extend({
			elementId:			'phone_phonebook_filters',
			
			classNames:			'disabled',
			
			template: Em.Handlebars.compile(
				'<ul>'+
					'<li>A - F</li>'+
					'<li>G - L</li>'+
					'<li>M - S</li>'+
					'<li>T - Z</li>'+
					'<li>Other</li>'+
				'</ul>'
			)
			
		}),
		
		list: MFT.List.extend({
			elementId:		   'phone_phonebook_list',
			
			/** Items */
			items: [
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Micky Marmaremmy',
						disabled:	true,
						arrow:		true
						
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Billy McTuff',
						disabled:	true,
						arrow:		true
					}	
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Hope Doferson',
						disabled:	true,
						arrow:		true
					}			
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Larry Pitterputz',
						disabled:	true,
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Suzie McCozo',
						disabled:	true,
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Suzie McCozo',
						disabled:	true,
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Larry Pitterputz',
						disabled:	true,
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Micky Marmaremmy',
						disabled:	true,
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Billy McTuff',
						disabled:	true,
						arrow:		true
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'arrow',
						text:		'Hope Doferson',
						disabled:	true,
						arrow:		true
					}				
				}
			],
			
			itemsOnPage:	5
		})
	})
});
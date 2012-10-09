/**
 * @name MFT.HistoryPhoneView
 * 
 * @desc Phone History module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/HistoryView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.HistoryPhoneView = Em.ContainerView.create({
	
	elementId:	'phone_history',
	
	classNameBindings: ['MFT.States.phone.history.active:active_state'],
	
	classNames: ['hidden'],
	
	childViews: [
		'help',
		'content'
	],
	
	
	help: Em.View.extend({
		elementId:			'phone_history_help',
		
		classNames:			'help',
		
		classNameBindings: ['MFT.helpMode:visible']
	}),
	
	
	content: Em.ContainerView.extend({
		
		elementId:	'phone_history_content',
		
		childViews: [
			'filters',
			'historyList',
			'controls'
		],
		
		filters: Em.View.extend({
			elementId:			'phone_history_filters',
			
			classNames:			'disabled',
			
			template: Em.Handlebars.compile(
				'<ul>'+
					'<li>All</li>'+
					'<li>Outgoing</li>'+
					'<li>Incoming</li>'+
					'<li>Missed</li>'+
				'</ul>'
			)
		}),
		
		historyList: MFT.List.extend({
		
			elementId:		'phone_history_list',
			
			items:	[
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Micky Marmaremmy',
						rightText:	'1:28 pm',
						disabled:	true,
						icon:		'images/phone/missed_call_ico.png'
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Billy McTuff',
						rightText:	'12:03 pm',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'		
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Hope Doferson',
						rightText:	'11:35 am',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'				
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Larry Pitterputz',
						rightText:	'05/25/2011',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'				
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Suzie McCozo',
						rightText:	'05/26/2011',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Suzie McCozo',
						rightText:	'05/26/2011',
						disabled:	true,
						icon:		'images/phone/missed_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Larry Pitterputz',
						rightText:	'05/25/2011',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Micky Marmaremmy',
						rightText:	'1:28 pm',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'				
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Billy McTuff',
						rightText:	'12:03 pm',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						text:		'Hope Doferson',
						rightText:	'11:35 am',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'			
					}				
				}
			],
			
			itemsOnPage:	5
		}),
		
		controls: Em.ContainerView.extend({
			elementId:		'phone_history_controls',
			
			childViews: [
				'dialButton',
				'saveDialButton'
			],
			
			dialButton: MFT.Button.extend({
				elementId:	'phone_history_dial',
				
				disabled:	true,
				text:		'Dial',
				icon:		'images/phone/dial.png'
			}),
			
			saveDialButton: MFT.Button.extend({
				elementId:	'phone_history_savedial',
				
				disabled:	true,
				text:		'Save dial',
				arrow:		true,
			})
		})
	})
});
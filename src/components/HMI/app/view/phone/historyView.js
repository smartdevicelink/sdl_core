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

MFT.HistoryPhoneView = Em.ContainerView.create( MFT.LoadableView, {
		
	elementId:	'phone_history',
			
	childViews: [
		'help',
		'content'
	],
	
	help: Em.View.extend({
		elementId:			'phone_history_help',
		
		classNames:			['blured_background','shifted']
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
					'<li><span>{{MFT.locale.label.view_phone_history_all}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_history_outgoing}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_history_incoming}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_history_missed}}</span></li>'+
				'</ul>'
			)
		}),
		
		historyList: MFT.List.extend({
		
			elementId:		'phone_history_list',
			
			items:	[
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	 'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item1',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item1_rightText',
						disabled:		 true,
						icon:			 'images/phone/missed_call_ico.png'
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item2',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item2_rightText',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'		
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item3',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item3_rightText',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'				
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item4',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item4_rightText',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'				
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item5',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item5_rightText',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item6',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item6_rightText',
						disabled:	true,
						icon:		'images/phone/missed_call_ico.png'		
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item7',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item7_rightText',
						disabled:	true,
						icon:		'images/phone/incoming_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item8',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item8_rightText',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'					
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item9',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item9_rightText',
						disabled:	true,
						icon:		'images/phone/outgoing_call_ico.png'			
					}				
				},
				{
					type:		MFT.Button,
					
					params:		{
						templateName:	'rightText',
						textBinding: 'MFT.locale.label.view_phone_history_item10',
						rightTextBinding: 'MFT.locale.label.view_phone_history_item10_rightText',
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
				textBinding: Ember.Binding.oneWay('MFT.locale.label.view_phone_history_dial'),
				icon:		'images/phone/dial.png'
			}),
			
			saveDialButton: MFT.Button.extend({
				elementId:	'phone_history_savedial',
				
				classNames:	['large'],
				
				disabled:	true,
				textBinding: Ember.Binding.oneWay('MFT.locale.label.view_phone_history_saveDial'),
				templateName:	'arrow'
			})
		})
	})
});
/**
 * @name MFT.MessagingPhoneView
 * 
 * @desc Phone Messaging module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/MessagingView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.MessagingPhoneView = Em.ContainerView.create( MFT.LoadableView, {
		
	elementId:	'phone_messaging',
			
	childViews: [
		'help',
		'content'
	],
	
	help: Em.View.extend({
		elementId:			'phone_messaging_help',
		
		classNames:			['blured_background','shifted']
	}),
	
	content: Em.ContainerView.extend({
		
		elementId:	'phone_messaging_content',
		
		childViews: [
			'sendButton',
			'controls',
			'listMessages'
		],
	
		sendButton: MFT.Button.extend({
			elementId:	'phone_messaging_send',
			
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_phone_messaging_send'),
			templateName:	'arrow',
			disabled:	true
		}),
		
		controls: Em.View.extend({
			elementId:	'phone_messaging_controls',
			
			classNames:	'disabled',
			
			template:	Em.Handlebars.compile(
				'<ul>'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_downloadNew}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_deleteAll}}</span></li>'+
				'</ul>'
			)
		}),
		
		listMessages: Em.View.extend({
			elementId:	'phone_messaging_list',
			
			classNames:	'disabled',
			
			template:	Em.Handlebars.compile(
				'<ul class="list">'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_item1}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_item2}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_item3}}</span></li>'+
				'</ul>'+
				'<ul class="del">'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_delete}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_delete}}</span></li>'+
					'<li><span>{{MFT.locale.label.view_phone_messaging_delete}}</span></li>'+
				'</ul>'
			)
		})
	})
});
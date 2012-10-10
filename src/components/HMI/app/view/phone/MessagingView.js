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
		
	classNames: ['hidden'],
	
	stateObj: MFT.States.phone.messaging,
	
	childViews: [
		'help',
		'content'
	],
	
	help: Em.View.extend({
		elementId:			'phone_messaging_help',
		
		classNames:			'help',
		
		classNameBindings: ['MFT.helpMode:visible']	
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
			
			text:		'Send',
			arrow:		true,
			disabled:	true
		}),
		
		controls: Em.View.extend({
			elementId:	'phone_messaging_controls',
			
			classNames:	'disabled',
			
			template:	Em.Handlebars.compile(
				'<ul>'+
					'<li>Download New</li>'+
					'<li>Delete All</li>'+
				'</ul>'
			)
		}),
		
		listMessages: Em.View.extend({
			elementId:	'phone_messaging_list',
			
			classNames:	'disabled',
			
			template:	Em.Handlebars.compile(
				'<ul class="list">'+
					'<li>Suzie McCozo</li>'+
					'<li>Micky Marmaremmy</li>'+
					'<li>Billy McTuff</li>'+
				'</ul>'+
				'<ul class="del">'+
					'<li>Delete</li>'+
					'<li>Delete</li>'+
					'<li>Delete</li>'+
				'</ul>'
			)
		})
	})
});
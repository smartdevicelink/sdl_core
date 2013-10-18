/**
 * @name MFT.RateMFTGuideDialogueView
 * 
 * @desc Rate MFTGuide Dialogue module visual representation
 * 
 * @category	View
 * @filesource	app/view/RateMFTGuideDialogueView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.RateMFTGuideDialogueView = Em.ContainerView.create({
	
	elementId: 'rateMFTGuideDialogue_view',
	
	classNames: ['rateDialogue'],
	
	classNameBindings: ['MFT.RateController.rateMFTGuideDialogueIsVisible:popUpIsVisible'],
	
	childViews: [
		'wrapper',
		'content'
	],
	
	wrapper: Em.View.extend({
		elementId: 'rateMFTGuideDialogue_view_content_wrapper',
		classNames: ['wrapper']
	}),
	
	content:  Em.ContainerView.extend({
		
		elementId: 'rateMFTGuideDialogue_view_content',
		
		classNames: ['content'],
		
		
		childViews: [
			'title',
			'separator',
			'text',
			'rateButton',
			'cancelButton'
		],
		
		title: MFT.Label.extend({
			elementId:  'rateMFTGuideDialogue_view_content_title',
			classNames: 'title',
			contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_rateMFTGuideDialogue_title')
		}),
		
		separator:Em.View.extend({
			elementId: 'rateMFTGuideDialogue_view_content_separator',
			classNames: 'separator',
		}),
		
		text: Em.View.extend({			
			elementId:	'rateMFTGuideDialogue_view_content_message',
			classNames:   'text',
			template: Em.Handlebars.compile(
				'<p>{{MFT.locale.label.view_rateMFTGuideDialogue_text1}}</p>'+
				'<p>{{MFT.locale.label.view_rateMFTGuideDialogue_text2}}</p>'+
				'<p>{{MFT.locale.label.view_rateMFTGuideDialogue_text3}}</p>'
			)
		}),
		
		rateButton: MFT.Button.extend({
			elementId: 			 'rateMFTGuideDialogue_view_content_rateButton',
			classNames:			'rateDialogue_button',
			textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_rateMFTGuideDialogue_rateApp'),
			action:			    'rateApp',
			target:			    'MFT.RateController'
		}),
		
		cancelButton: MFT.Button.extend({
			elementId: 			 'rateMFTGuideDialogue_view_content_cancelButton',
			classNames:			'rateDialogue_button',
			textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_rateMFTGuideDialogue_noThankes'),
			action:			    'cancelRating',
			target:			    'MFT.RateController'
		}),
	})
})
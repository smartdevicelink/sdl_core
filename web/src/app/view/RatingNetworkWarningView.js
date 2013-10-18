/**
 * @name MFT.RatingNetworkWarningView
 * 
 * @desc Rating Network Warning module visual representation
 * 
 * @category	View
 * @filesource	app/view/RatingNetworkWarningView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.RatingNetworkWarningView = Em.ContainerView.create({
	
	elementId: 'ratingNetworkWarning_view',
	
	classNames: ['rateDialogue'],
	
	classNameBindings: ['MFT.RateController.ratingNetworkWarningIsVisible:popUpIsVisible'],
	
	isVisible: true,
	
	childViews: [
		'wrapper',
		'content'
	],
	
	wrapper: Em.View.extend({
		elementId: 'ratingNetworkWarning_view_content_wrapper',
		classNames: ['wrapper']
	}),
	
	content:  Em.ContainerView.extend({
		
		elementId: 'ratingNetworkWarning_view_content',
		
		classNames: ['content'],
		
		
		childViews: [
			'icon',
			'title',
			'separator',
			'text',
			'okButton'
		],
		
		icon: Em.View.extend({
			elementId:  'ratingNetworkWarning_view_content_icon',
			classNames: 'icon'
		}),
		
		title: MFT.Label.extend({
			elementId:  'ratingNetworkWarning_view_content_title',
			classNames: 'title',
			contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_ratingNetworkWarning_title')
		}),
		
		separator: Em.View.extend({
			elementId: 'ratingNetworkWarning_view_content_separator',
			classNames: 'separator',
		}),
		
		text: Em.View.extend({			
			elementId:	'ratingNetworkWarning_view_content_message',
			classNames:   'text',
			template: Em.Handlebars.compile(
				'<p>{{MFT.locale.label.view_ratingNetworkWarning_text1}}</p>'+
				'<p>{{MFT.locale.label.view_ratingNetworkWarning_text2}}</p>'
			)
		})
		,
		
		okButton: MFT.Button.extend({
			elementId: 			 'ratingNetworkWarning_view_content_okButton',
			classNames:			'rateDialogue_button okButton',
			textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_ratingNetworkWarning_ok'),
			action:			    'hideRatingNetworkWarningView',
			target:			    'MFT.RateController'
		})
	})
})
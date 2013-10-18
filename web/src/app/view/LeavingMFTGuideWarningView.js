/**
 * @name MFT.LeavingMFTGuideWarningView
 * 
 * @desc Leaving MFT Guide Warning module visual representation
 * 
 * @category	View
 * @filesource	app/view/LeavingMFTGuideWarningView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.LeavingMFTGuideWarningView = Em.ContainerView.create({
	
	elementId: 'leavingMFTGuideWarning_view',
	
	classNames: ['rateDialogue'],
	
	classNameBindings: ['MFT.RateController.leavingMFTGuideWarningIsVisible:popUpIsVisible'],
	isVisible: true,
	
	childViews: [
		'wrapper',
		'content'
	],
	
	wrapper: Em.View.extend({
		elementId: 'leavingMFTGuideWarning_view_content_wrapper',
		classNames: ['wrapper']
	}),
	
	content:  Em.ContainerView.extend({
		
		elementId: 'leavingMFTGuideWarning_view_content',
		
		classNames: ['content'],
		
		
		childViews: [
			'icon',
			'title',
			'separator',
			'text',
		],
		
		icon: Em.View.extend({
			elementId:  'leavingMFTGuideWarning_view_content_icon',
			classNames: 'icon'
		}),
		
		title: MFT.Label.extend({
			elementId:  'leavingMFTGuideWarning_view_content_title',
			classNames: 'title',
			contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_leavingMFTGuideWarning_title')
		}),
		
		separator: Em.View.extend({
			elementId: 'leavingMFTGuideWarning_view_content_separator',
			classNames: 'separator',
		}),
		
		text: Em.View.extend({			
			elementId:	'leavingMFTGuideWarning_view_content_message',
			classNames:   'text',
			template: Em.Handlebars.compile(
				'<p>{{MFT.locale.label.view_leavingMFTGuideWarning_text1}}</p>'+
				'<p>{{MFT.locale.label.view_leavingMFTGuideWarning_text2}}</p>'
			)
		})
	})
})
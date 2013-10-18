/**
 * @name MFT.RatingNoRespondWarningView
 * 
 * @desc Rating Network Warning module visual representation
 * 
 * @category	View
 * @filesource	app/view/RatingNoRespondWarningView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.RatingNoRespondWarningView = Em.ContainerView.create({
	
	elementId: 'ratingNoRespondWarning_view',
	
	classNames: ['rateDialogue'],
	
	classNameBindings: ['MFT.RateController.ratingNoRespondWarningIsVisible:popUpIsVisible'],
	
	isVisible: true,
	
	childViews: [
		'wrapper',
		'content'
	],
	
	wrapper: Em.View.extend({
		elementId: 'ratingNoRespondWarning_view_content_wrapper',
		classNames: ['wrapper']
	}),
	
	content:  Em.ContainerView.extend({
		
		elementId: 'ratingNoRespondWarning_view_content',
		
		classNames: ['content'],
		
		
		childViews: [
			'icon',
			'title',
			'separator',
			'text',
			'okButton'
		],
		
		icon: Em.View.extend({
			elementId:  'ratingNoRespondWarning_view_content_icon',
			classNames: 'icon'
		}),
		
		title: MFT.Label.extend({
			elementId:  'ratingNoRespondWarning_view_content_title',
			classNames: 'title',
			contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_ratingNetworkWarning_title')
		}),
		
		separator: Em.View.extend({
			elementId: 'ratingNoRespondWarning_view_content_separator',
			classNames: 'separator',
		}),
		
		text: Em.View.extend({			
			elementId:	'ratingNoRespondWarning_view_content_message',
			classNames:   'text',
            /*
             * Set Warning text according to OS
             */
            warningText1: function(){
                if (FFW.isAndroid) {
                    return MFT.locale.label.view_ratingNoRespondWarning_text1_android;
                } else if (FFW.isIOS){
                    return MFT.locale.label.view_ratingNoRespondWarning_text1_ios;
                } else {
                    return MFT.locale.label.view_ratingNoRespondWarning_text1_desktop;
                }
            }.property('MFT.locale'),
			template: Em.Handlebars.compile(
				'<p>{{view.warningText1}}</p>'+
				'<p>{{MFT.locale.label.view_ratingNoRespondWarning_text2}}</p>'
			)
		}),
		
		okButton: MFT.Button.extend({
			elementId: 			 'ratingNoRespondWarning_view_content_okButton',
			classNames:			'rateDialogue_button okButton',
			textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_ratingNetworkWarning_ok'),
			action:			    'hideRatingNoRespondWarningView',
			target:			    'MFT.RateController'
		})
	})
})
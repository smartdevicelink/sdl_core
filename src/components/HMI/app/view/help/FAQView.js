/**
 * @name MFT.FAQView
 * 
 * @desc FAQ module visual representation
 * 
 * @category	View
 * @filesource	app/view/help/FAQView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.FAQView = Em.ContainerView.create({
	
	elementId:	'faq_view',
 	
 	classNames:	'hidden',
	
	/** Define state*/
	classNameBindings: ['MFT.States.faq.active:active_state'],
	
	/** Element ID*/
	elementId: 'fag_view',
	
	/** Child views*/
	childViews: [
		'faq_list',
		'faq_bottom',
	],

	/** List on view*/
	faq_list: MFT.List.create({
		dataListBinding: 	 'MFT.FAQModel.list',
		scrollBarIsDisabled: false,
		itemsOnPage: 5,
		itemAction: function(event){
			MFT.VideoPlayerController.start({videoPath:event.videoPath});
		}
	}),
	
	faq_list: MFT.List.extend({
		elementId:		   'faq_view_list',
		
		disableScrollbar:	false,
		
		/** Items */
		items: [
			{								
				type:		MFT.Button,
				
				params:		{
					text:		'MFT Guide Welcome Orientation',
					videoName:   'FAQ_Welcome_orientation',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}
					
			},
			{
				type:		MFT.Button,

				params:		{
					text:		'MyFord Touch 4 Corner Menu System',
					videoName:   'FAQ_4_corners',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}							
			},
			{
				type:		MFT.Button,

				params:		{
					text:		'Customize Your MyFord Touch System',
					videoName:   'FAQ_Customization',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					text:		'Use Voice Commands',
					videoName:   'FAQ_Voice_commands',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}
			},
			{
				type:		MFT.Button,

				params:		{
					text:		'MyFord Touch Nav System',
					videoName:   'FAQ_Nav_system',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					text:		'Pair Your Phone and Make Hands-Free Calls',
					videoName:   'FAQ_Pair_phones',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			}
		],
		
		itemsOnPage:	5
	}),
	
	/** Bottom text*/
	faq_bottom: Em.View.create({
		elementId: ['faq_bottom'] ,
		template: Em.Handlebars.compile(
			'<div > For further assistance about your in-vehicle MyFord Touch system, call 1-800-392-3673. </div>'+
			'<div class="second-line"> Select option 3 for an In-Vehicle Technology Team representative. </div>'
		)
	})
});
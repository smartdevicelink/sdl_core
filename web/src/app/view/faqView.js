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

MFT.FAQView = Em.ContainerView.create(MFT.LoadableView, {

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
	
	isNav: function(){
		return 	!FFW.Backend.isNavigationEnabled;
	}.property('FFW.Backend.isNavigationEnabled'),
	
	faq_list: MFT.List.extend({
		elementId:		   'faq_view_list',
		
		disableScrollbar:	false,
		
		/** Items */
		items: [
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_bt_audio',
					videoName:   'ent_BT_audio',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_customize',
					videoName:   'FAQ_Customization',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_multipleConnected',
					videoName:   'FAQ_Multi_phone',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{								
				type:		MFT.Button,
				
				params:		{
					textBinding: 'MFT.locale.label.view_faq_mftGuide',
					videoName:   'FAQ_Welcome_orientation',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}
					
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_myFordTouch4',
					videoName:   'FAQ_4_corners',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}							
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_myFordTouchN',
					videoName:   'FAQ_Nav_system',
					hiddenBinding:	'MFT.FAQView.isNav',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_pairYourP',
					videoName:   'FAQ_Pair_phones',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}			
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_useVoiceC',
					videoName:   'FAQ_Voice_commands',
					action:	  'playRecord',
					target:	  'MFT.FAQController',
					onDown: false
				}
			},
			{
				type:		MFT.Button,

				params:		{
					textBinding: 'MFT.locale.label.view_faq_wallpaper',
					videoName:   'FAQ_Wallpaper',
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
			'<div > {{MFT.locale.label.view_faq_forFurther}} </div>'+
			'<div class="second-line"> {{MFT.locale.label.view_faq_selectOption}} </div>'
		)
	})
});
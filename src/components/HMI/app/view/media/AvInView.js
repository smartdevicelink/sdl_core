/**
 * @name MFT.AvInView
 * 
 * @desc Media AVIN module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/AvInView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.avinView = Em.View.create(MFT.LoadableView,{
		/** Bind class for visual representation */
		classNameBindings: 	['MFT.States.media.avin.active:active_state'],
		/** Class Name */
		classNames: 'hidden',
		/** View ID*/
		elementId:	'media_avin',
		
		stateObj: MFT.States.media.avin,
		
		/*Template*/
		template: Ember.Handlebars.compile(
				'<div class="avin-info">'+
					'<div id="line_in_blur" {{bindAttr class="MFT.helpMode:visible_display"}}></div>'+
					'<div id="avin-line" {{bindAttr class="MFT.helpMode:hidden_display"}}>'+
						'<img src="images/media/av-in_details.png">'+
					'</div>'+
				'</div>'
			
		)
});
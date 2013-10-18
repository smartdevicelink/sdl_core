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

		/** View ID*/
		elementId:	'media_avin',
		
		/*Template*/
		template: Ember.Handlebars.compile(
			'<div class="avin-info">'+
				'<div id="avin-line">'+
					'<img src="images/media/av-in_details.png">'+
				'</div>'+
			'</div>'			
		)
});
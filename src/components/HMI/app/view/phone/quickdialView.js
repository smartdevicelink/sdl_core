/**
 * @name MFT.QuickdialPhoneView
 * 
 * @desc Phone Quickdial module visual representation
 * 
 * @category	View
 * @filesource	app/view/phone/QuickdialView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.QuickdialPhoneView = Em.ContainerView.create( MFT.LoadableView, {
		
	elementId:	'phone_quickdial',
			
	childViews: [
		'help',
		'content'
	],
	
	help: Em.View.extend({
		elementId:		'phone_quickdial_help',
		
		classNames:			['blured_background','shifted']	
	}),
	
	content: Em.ContainerView.extend({
		
		elementId:		'phone_quickdial_content',
		
		childViews: [
			'leftList',
			'rightList'
		],
		
		leftList: Em.View.extend({
			
			elementId:	'phone_quickdial_leftList',
			
			classNames:	['l-block', 'disabled'],
			
			tagName:	'ul',
			
			template:	Ember.Handlebars.compile(
				'<li><span class="number">1</span><span>{{MFT.locale.label.view_phone_quickDial_item1}}<span></li>'+
				'<li><span class="number">2</span><span>{{MFT.locale.label.view_phone_quickDial_item2}}</span></li>'+
				'<li><span class="number">3</span><span>{{MFT.locale.label.view_phone_quickDial_item3}}</span></li>'+
				'<li><span class="number">4</span><span>{{MFT.locale.label.view_phone_quickDial_item4}}</span></li>'+
				'<li><span class="number">5</span><span>{{MFT.locale.label.view_phone_quickDial_item5}}</span></li>'
			)
		}),
		
		rightList: Em.View.extend({
			elementId:	'phone_quickdial_rightList',
			
			classNames:	['r-block', 'disabled'],
			
			tagName:	'ul',
			
			template:	Ember.Handlebars.compile(
				'<li><span class="number">6</span><span>{{MFT.locale.label.view_phone_quickDial_item6}}</span></li>'+
				'<li><span class="number">7</span><span>{{MFT.locale.label.view_phone_quickDial_item7}}</span></li>'+
				'<li><span class="number">8</span><span>{{MFT.locale.label.view_phone_quickDial_item8}}</span></li>'+
				'<li><span class="number">9</span><span>{{MFT.locale.label.view_phone_quickDial_item9}}</span></li>'+
				'<li><span class="number">10</span><span>{{MFT.locale.label.view_phone_quickDial_item10}}</span></li>'
			)
		})
	})
});
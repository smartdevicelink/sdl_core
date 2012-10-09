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

MFT.QuickdialPhoneView = Em.ContainerView.create({
	
	elementId:	'phone_quickdial',
	
	classNameBindings: ['MFT.States.phone.quickdial.active:active_state'],
	
	classNames: ['hidden'],
	
	childViews: [
		'help',
		'content'
	],
	
	help: Em.View.extend({
		elementId:		'phone_quickdial_help',
		
		classNames:			'help',
		
		classNameBindings: ['MFT.helpMode:visible']		
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
				'<li><div>1</div>Voice Mail</li>'+
				'<li><div>2</div>Micky Marmaremmy</li>'+
				'<li><div>3</div>Billy McTuff</li>'+
				'<li><div>4</div>Hope Doferson</li>'+
				'<li><div>5</div>Larry Pitterputz</li>'
			)
		}),
		
		rightList: Em.View.extend({
			elementId:	'phone_quickdial_rightList',
			
			classNames:	['r-block', 'disabled'],
			
			tagName:	'ul',
			
			template:	Ember.Handlebars.compile(
				'<li><div>6</div>James Murly</li>'+
				'<li><div>7</div>Wally Schultz</li>'+
				'<li><div>8</div>Suzie McCozo</li>'+
				'<li><div>9</div>Empty</li>'+
				'<li><div>10</div>Empty</li>'
			)
		})
	})
});
/**
 * @name MFT.ScrollBar
 * 
 * @desc ScrollBar component for List component
 * 
 * @category	Controlls
 * @filesource	app/controlls/ScrollBar.js
 * @version		4.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.ScrollBar = Em.ContainerView.extend({
		
	/** Define enable/disable scrollbar*/
	classNameBindings: ['scrollBarIsDisabled:is-disabled'],
	
	/** Componet class*/
	classNames: 'scrollbar',
		
	/** Childs views*/
	childViews: [
		'upButton',
		'bar',
		'downButton'
	],	
	
	/** current page */
	currentPage: 	1,
	
	/**Pages count*/
	pageCount: 		0,
	
	listHeight:       250,
	
	/** On/OF scrollbar */
	scrollBarIsDisabled: 	false,
	
	/** Define bar height*/	
	sbHeight: function(){
		/** Max bar height*/
		this.maxHeight = this.listHeight - 102;
		if ( this.pageCount <= 1 ){
			return this.maxHeight + 1;
		} else {
			return (this.maxHeight/this.pageCount);
		}		
	}.property('pageCount'),
	
	/** Position of bar*/
	sbTop: function(){
		if (this.get('currentPage') == 0) {
			return 0;
		} else {
			return (this.maxHeight-this.get('sbHeight'))/(this.get('pageCount')-1)*this.get('currentPage')+1;
		}
	}.property('currentPage','pageCount'),
	
	/** Support function*/
	scrollbarBodyStyleAttributes: function(){
		return 'height: '+(this.get('listHeight')-1)+'px;';
	}.property('listHeight'),
	
	/** Support function*/
	sbBodyStyleAttributes: function(){
		return 'height: '+(this.get('listHeight')-100-1)+'px;';
	}.property('listHeight'),
	
	sbStyleAttributes: function(){
		return 'height: '+this.get('sbHeight')+'px; '+'top: '+this.get('sbTop')+'px';
	}.property('currentPage','pageCount'),
	
	/** Define scroll up button "disable" status */
	sbUpButtonIsDisabled: function(){
		if( this.get('currentPage') < 1 ) {
			return true;
		} else {
			return false;
		}
	}.property('currentPage','pageCount'),
	
	/** Define scroll down button "disable" status */
	sbDownButtonIsDisabled:function(){
		if ( (this.pageCount-1) > this.get('currentPage') ) {
			return false;
		} else {
			return true;
		}
	}.property('currentPage','pageCount'),
	
	attributeBindings: ['scrollbarBodyStyleAttributes:style'],
		
	/** Bottom for scroll up*/
	upButton: MFT.Button.extend({
		classNames: 			['sb-top','button'],
		action:					'sbUp',
		target:					'parentView.parentView',
		disabledBinding:		'parentView.sbUpButtonIsDisabled',
		icon:					'images/list/scrollbar/button-up-active.png',
		timer:					 200
	}),
	
	/** Bottom for scroll down*/
	downButton:MFT.Button.extend({
		classNames: 			['sb-bottom','button'],
		action:					'sbDown',
		target:					'parentView.parentView',
		disabledBinding:		'parentView.sbDownButtonIsDisabled',
		icon:					'images/list/scrollbar/button-down-active.png',
		timer:					 200
	}),
	
	/** Scrollbar track */
	bar: Em.View.extend({
		barBodyStyleBinding:	'parentView.sbBodyStyleAttributes',
		barStyleBinding:		'parentView.sbStyleAttributes',
		cancelAnimationBinding: 'parentView.parentView.cancelAnimation',
		
		layout: Em.Handlebars.compile(
			'<div class="sb-body" {{bindAttr style="view.barBodyStyle"}}>'+
				'{{yield}}'+
			'</div>'
		),
		
		template: Em.Handlebars.compile(
			'<div class="sb-bar" '+
				'{{bindAttr style="view.barStyle"}}'+
				'{{bindAttr class="view.cancelAnimation:cancelBarAnimation"}}>'+
			'</div>'
		)
	})
});
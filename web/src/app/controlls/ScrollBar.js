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
	
	/**Pages count*/
	pageCount: 		0,
	
	listHeight:       250,
	
	barposition: 0,
	
	upactive: true,
	
	bottomactive: false,
	
	/** On/OF scrollbar */
	scrollBarIsDisabled: 	false,
	
	
	didInsertElement: function() {
		this._super();
		//init scroll bar height an position style
		this.set('sbstyle', 'height: '+this.get('sbHeight')+'px; '+'top: '+this.barposition+'px');
		// init bottom scrollbar button active state
		if ( (this.pageCount-1) > this.currentPage ) {
			this.set('bottomactive', false);
		} else {
			this.set('bottomactive', true);
		}
	}.observes('pageCount'),
	
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
		if (this.currentPage == 0) {
			this.set('barposition', 0);
		} else {
			this.set('barposition', (this.maxHeight-this.get('sbHeight'))/(this.pageCount-1)*this.currentPage+1);
		}
	}.observes('currentPage','pageCount'),
	
	/** Support function*/
	scrollbarBodyStyleAttributes: function(){
		return 'height: '+(this.listHeight-1)+'px;';
	}.property('listHeight'),
	
	/** Support function*/
	sbBodyStyleAttributes: function(){
		return 'height: '+(this.listHeight-100-1)+'px;';
	}.property('listHeight'),
	
	sbStyleAttributes: function(){
		this.set('sbstyle', 'height: '+this.get('sbHeight')+'px; '+'top: '+this.barposition+'px');
	}.observes('currentPage','pageCount'),
	
	/** Define scroll up button "disable" status */
	sbUpButtonIsDisabled: function(){
		if( this.currentPage < 1 ) {
			this.set('upactive', true);
		} else {
			this.set('upactive', false);
		}
	}.observes('currentPage'),
	
	/** Define scroll down button "disable" status */
	sbDownButtonIsDisabled:function(){
		if ( (this.pageCount-1) > this.currentPage ) {
			this.set('bottomactive', false);
		} else {
			this.set('bottomactive', true);
		}
	}.observes('currentPage'),
	
	attributeBindings: ['scrollbarBodyStyleAttributes:style'],
		
	/** Bottom for scroll up*/
	upButton: MFT.Button.extend({
		classNames: 			['sb-top','button'],
		action:					'sbUp',
		target:					'parentView.parentView',
		disabledBinding:		'parentView.upactive',
		icon:					'images/list/scrollbar/button-up-active.png'
	}),
	
	/** Bottom for scroll down*/
	downButton:MFT.Button.extend({
		classNames: 			['sb-bottom','button'],
		action:					'sbDown',
		target:					'parentView.parentView',
		disabledBinding:		'parentView.bottomactive',
		icon:					'images/list/scrollbar/button-down-active.png'
	}),
	
	/** Scrollbar track */
	bar: Em.View.extend({
		barBodyStyleBinding:	'parentView.sbBodyStyleAttributes',
		barStyleBinding:		'parentView.sbstyle',
		cancelAnimationBinding: 'parentView.parentView.cancelAnimation',
		
		template: Em.Handlebars.compile(
			'<div class="sb-body" {{bindAttr style="view.barBodyStyle"}}>'+
				'<div class="sb-bar" '+
					'{{bindAttr style="view.barStyle"}}'+
					'{{bindAttr class="view.cancelAnimation:cancelBarAnimation"}}>'+
				'</div>'+
			'</div>'
		)
	})
});
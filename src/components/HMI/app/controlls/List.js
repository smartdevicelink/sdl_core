/**
 * @name MFT.List
 * 
 * @desc 		General list component for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/List.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */


MFT.List = Em.ContainerView.extend({
	
	classNames: 'list',
	
	/** flag for scrollbar*/
	disableScrollbar:	false,
		
	/** Set count of items at one time */
	itemsOnPage: 	0,
	
	/** Speed of scrolling in milliseconds*/
	scrollBarSpeed: 200,
	
	/** Number of columns on page*/
	columnsNumber: 1,
	
	itemheight: 50,
	
	/** Current scroll page*/
	currentPage: 	0,
	
	/** Css style of list*/
	listScrollingAttributes: '',
	
	/** Count of items in menu*/
	listCount: function(){
		if( this.items ) {
			return this.items.length;
		}
	}.property('items.@each.type'),
	
	/**Pages count*/
	pageCount: function(){
		return Math.ceil(this.get('listCount')/this.get('columnsNumber')/this.get('itemsOnPage'));
	}.property('listCount','itemsOnPage'),
		
	listHeight: function(){
		return this.itemsOnPage*this.itemheight;
	}.property('itemsOnPage'),
	
	/** Action of element "sb-top" which show previous list page */
	sbUp: function(){
		if (this.get('currentPage') > 0){
			this.set('currentPage', this.get('currentPage') - 1 );
		}
	},
	
	/** Action of element "sb-bottom" which show previous list page */
	sbDown: function(){
		if (this.get('currentPage') < this.get('pageCount')-1) {
			this.set('currentPage', this.get('currentPage') + 1 );
		}		
	},
	
	/** Scroll content according to current page */
	onCurrentPageChange: function(){
		this.set('listScrollingAttributes', 'margin-top: '+(this.get('currentPage')* this.itemsOnPage*(-50))+'px');
	}.observes('currentPage'),

	/** Method for delete certain item from list */
	deleteItem: function(id){
		this.items.splice(id, 1);
		this.list.refresh();
	},
	
	
	/** List components */
	childViews: [
		'list',
		'scrollbar'
	],
	
	
	/** List view */
	list: Em.ContainerView.extend({
				
		classNames: 'list-content',
		
		listStyleBinding: 'parentView.listScrollingAttributes',
		
		attributeBindings: ['listStyle:style'],
		
		refresh: function() {
			this.rerender();
		}.observes('_parentView.items.@each.type'),
		
		afterRender: function() {		
			var items = this._parentView.items,
				element, i, key, binding;
						
			for( i = 0; i < items.length; i++ ) {
		
				element = items[i].type.create({
					// element id
					elementId:	this._parentView.elementId+'_item'+i,
					
					// list item css class
					classNames: 'list-item',
					
					classNameBindings: ['this.voiceOver'],
					
					// Dynamic property set
					init: function() {
						for( key in items[i].params ) {
							if(key.match('Binding') != null){			
								binding = Ember.Binding.from(items[i].params[key]).to(key.replace('Binding',''));
								binding.connect(this);
								//Set one way binding
								binding.oneWay();
							}else{
								this.set(key, items[i].params[key]);
							}
						}	
						this._super();
						// synchronize bindings
						Ember.run.sync();
					}
				})
				
				// Push element to list
				this.get('childViews').pushObject(element);
			}
		}
	}),
	
	/** Scrollbar view */
	scrollbar: MFT.ScrollBar.extend({	
		currentPageBinding:			'parentView.currentPage',
		pageCountBinding:			'parentView.pageCount',
		listHeightBinding:			'parentView.listHeight',
		scrollBarIsDisabledBinding:	'parentView.disableScrollbar'
	})
});
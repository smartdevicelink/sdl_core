/**
 * @name MFT.ScrollableText
 * 
 * @desc        General ScrollableText component
 * 
 * @category    Controlls
 * @filesource  app/controlls/scrollableText.js
 * @version     2.0
 *
 * @author      Andriy Melnyk
 */


MFT.ScrollableText = Em.ContainerView.extend({
    
    classNames: 'scrollableText',
    
    /** flag for scrollbar*/
    disableScrollbar:   false,
        
    /** Set count of items at one time */
    itemsOnPage:    0,
    
    /** Speed of scrolling in milliseconds*/
    scrollBarSpeed: 200,

    /** Number of columns on page*/
    columnsNumber: 1,

    itemheight: 50,

    /** Current scroll page*/
    currentPage:    0,

    /** Css style of scrollableText*/
    scrollableTextScrollingAttributes: '',

    /** Count of items in menu*/
    /*
    scrollableTextCount: function(){
        if( this.items ) {
            return this.items.length;
        }
    }.property('items.@each.type'),
    */
    scrollableTextCount: function(){
        //console.log(this.get('this.scrollableText.childViews.length'));
            return this.get('this.scrollableText.childViews.length');
    }.property('this.scrollableText.childViews.length'),

    /**Pages count*/
    pageCount: function(){
        return Math.ceil(this.get('scrollableTextCount')/this.get('columnsNumber')/this.get('itemsOnPage'));
    }.property('scrollableTextCount','itemsOnPage'),

    scrollableTextHeight: function(){
        return this.itemsOnPage*this.itemheight;
    }.property('itemsOnPage'),

    /** Action of element "sb-top" which show previous scrollableText page */
    sbUp: function(){
        if (this.get('currentPage') > 0){
            this.set('currentPage', this.get('currentPage') - 1 );
        }
    },

    /** Action of element "sb-bottom" which show previous scrollableText page */
    sbDown: function(){
        if (this.get('currentPage') < this.get('pageCount')-1) {
            this.set('currentPage', this.get('currentPage') + 1 );
        }       
    },

    /** Scroll content according to current page */
    onCurrentPageChange: function(){
        this.set('scrollableTextScrollingAttributes', 'margin-top: '+(this.get('currentPage')* this.itemsOnPage*(-50))+'px');
    }.observes('currentPage'),

    /** Method for delete certain item from scrollableText */
    deleteItem: function(id){
        this.items.splice(id, 1);
        this.scrollableText.refresh();
    },

    /** scrollableText components */
    childViews: [
        'scrollableText',
        'scrollbar'
    ],

    /** scrollableText view */
    scrollableText: Em.View.extend({

        classNames: 'scrollableText-content',

        scrollableTextStyleBinding: 'parentView.scrollableTextScrollingAttributes',

        attributeBindings: ['scrollableTextStyle:style'],

        contentBinding:     'this.parentView.items',
        
        defaultTemplate: Em.Handlebars.compile(
            '<textarea class="scrollableTextArea">{{view.content}}</textarea>'
        ),

        refresh: function() {
            this.rerender();
        }.observes('_parentView.items'),

    }),

    /** Scrollbar view */
    scrollbar: MFT.ScrollBar.extend({   
        currentPageBinding:         'parentView.currentPage',
        pageCountBinding:           'parentView.pageCount',
        scrollableTextHeightBinding:          'parentView.scrollableTextHeight',
        scrollBarIsDisabledBinding: 'parentView.disableScrollbar'
    })
});
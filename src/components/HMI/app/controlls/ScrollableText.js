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

    /** Css style of line-height of rows*/
    linesHeght: 23,

    linesCount: 1,

    scrollHeight: null,

    /** Count of items in menu*/
    /*
    scrollableTextCount: function(){
        if( this.items ) {
            return this.items.length;
        }
    }.property('items.@each.type'),
    */

    scrollableTextCount: function(){
        if( $('#' + this.get('childViews')[1].elementId) ){
            $('#' + this.get('childViews')[1].elementId).removeAttr('style');
            this.set('scrollHeight', $('#' + this.get('childViews')[1].elementId)[0].scrollHeight);
            $('#' + this.get('childViews')[1].elementId).height(this.scrollHeight);
            this.set('linesCount', $('#' + this.get('childViews')[1].elementId)[0].scrollHeight / this.linesHeght);
            console.log('linesCount     ' + this.linesCount);
        }
    },

    /**Pages count*/
    pageCount: function(){
                return Math.ceil(this.get('linesCount')/this.get('itemsOnPage'));
    }.property('linesCount','itemsOnPage'),

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
        this.set('scrollableTextScrollingAttributes', 'height: '+ this.scrollHeight +'px; top: '+(this.get('currentPage')* this.itemsOnPage*(-23))+'px');
    }.observes('currentPage'),

    /** Method for delete certain item from scrollableText */
    deleteItem: function(id){
        this.items.splice(id, 1);
        this.scrollableText.refresh();
    },

    /** scrollableText components */
    childViews: [
        'scrollbar',
       // 'scrollableText'
    ],

    refreshTextArea: function(){
        if( this.get('childViews')[1] ){
            this.get('childViews').removeObject(this.get('childViews')[1]);
        }

        $('#scrollableTextArea').height(23);
        this.set('scrollHeight', 23);
        this.set('linesCount', 1);
        this.set('currentPage', 0);

        this.get('childViews').pushObject(
            Ember.TextArea.create({

                classNames: 'scrollableTextArea',

                elementId:  'scrollableTextArea',

                scrollableTextStyleBinding: 'parentView.scrollableTextScrollingAttributes',

                attributeBindings: ['scrollableTextStyle:style'],

                valueBinding:     'this.parentView.items',

                actionDown: function(){
                    return false;
                },

                didInsertElement: function(){
                    this.get('parentView').scrollableTextCount();
                },
/*
                refresh: function() {
                 //   this.rerender();
                }.observes('items'),
*/
            })
        );
    }.observes('items'),

    /** scrollableText view */
    scrollableText: Ember.TextArea.extend({

        classNames: 'scrollableTextArea',

        elementId:  'scrollableTextArea',

        scrollableTextStyleBinding: 'parentView.scrollableTextScrollingAttributes',

        attributeBindings: ['scrollableTextStyle:style'],

        valueBinding:     'this.parentView.items',

        actionDown: function(){
            return false;
        },

        didInsertElement: function(){
            this._parentView.scrollableTextCount();
        },

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
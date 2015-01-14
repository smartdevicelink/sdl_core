/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: ·
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. · Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. · Neither the name of the Ford Motor Company nor the
 * names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @name SDL.List
 * @desc General list component for SDL application
 * @category Controlls
 * @filesource app/controlls/List.js
 * @version 1.0
 */

SDL.List = Em.ContainerView.extend( {

    classNames: 'list',

    /** flag for scrollbar */
    disableScrollbar: false,

    /** Set count of items at one time */
    itemsOnPage: 0,

    /** Speed of scrolling in milliseconds */
    scrollBarSpeed: 200,

    /** Number of columns on page */
    columnsNumber: 1,

    itemheight: 50,

    /** Current scroll page */
    currentPage: 0,

    /** Css style of list */
    listScrollingAttributes: '',

    /** Count of items in menu */
    /*
     * listCount: function(){ if( this.items ) { return this.items.length; }
     * }.property('items.@each.type'),
     */
    listCount: function() {

        if ( (this.get('this.list.childViews.length') / this.get('columnsNumber') / this.get('itemsOnPage') ) < this.get('currentPage')) {
            this.set('currentPage', 0);
        }
        // console.log(this.get('this.list.childViews.length'));
        return this.get('this.list.childViews.length');
    }.property('this.list.childViews.length'),

    /** Pages count */
    pageCount: function() {

        return Math.ceil(this.get('listCount') / this.get('columnsNumber')
            / this.get('itemsOnPage'));
    }.property('listCount', 'itemsOnPage'),

    listHeight: function() {

        return this.itemsOnPage * this.itemheight;
    }.property('itemsOnPage'),

    /** Action of element "sb-top" which show previous list page */
    sbUp: function() {

        if (this.get('currentPage') > 0) {
            this.set('currentPage', this.get('currentPage') - 1);
        }
    },

    /** Action of element "sb-bottom" which show previous list page */
    sbDown: function() {

        if (this.get('currentPage') < this.get('pageCount') - 1) {
            this.set('currentPage', this.get('currentPage') + 1);
        }
    },

    /** Scroll content according to current page */
    onCurrentPageChange: function() {

        this.set('listScrollingAttributes', 'margin-top: '
            + (this.get('currentPage') * this.itemsOnPage * (-50)) + 'px');
    }.observes('currentPage'),

    /** Method for delete certain item from list */
    deleteItem: function(id) {

        this.items.splice(id, 1);
        this.list.refresh();
    },

    /** List components */
    childViews: [
        'list', 'scrollbar'
    ],

    /** List view */
    list: Em.ContainerView.extend( {

        classNames: 'list-content',

        listStyleBinding: 'parentView.listScrollingAttributes',

        attributeBindings: [
            'listStyle:style'
        ],

        refresh: function() {

            this.rerender();
        }.observes('_parentView.items.@each.type'),

        afterRender: function() {

            var items = this._parentView.items, element, i, key, binding;

            for (i = 0; i < items.length; i++) {

                element = items[i].type.create( {
                    // element id
                    elementId: this._parentView.elementId + '_item' + i,

                    // list item css class
                    classNames: 'list-item',

                    classNameBindings: [
                        'this.voiceOver'
                    ],

                    // Dynamic property set
                    init: function() {

                        for (key in items[i].params) {
                            if (key.match('Binding') != null) {
                                binding = Ember.Binding
                                    .from(items[i].params[key]).to(key
                                        .replace('Binding', ''));
                                binding.connect(this);
                                // Set one way binding
                                binding.oneWay();
                            } else {
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
    scrollbar: SDL.ScrollBar.extend( {
        currentPageBinding: 'parentView.currentPage',
        pageCountBinding: 'parentView.pageCount',
        listHeightBinding: 'parentView.listHeight',
        scrollBarIsDisabledBinding: 'parentView.disableScrollbar'
    })
});
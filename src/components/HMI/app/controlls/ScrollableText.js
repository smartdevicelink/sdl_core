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
 * @name SDL.ScrollableText
 * @desc General ScrollableText component
 * @category Controlls
 * @filesource app/controlls/scrollableText.js
 * @version 1.0
 */

SDL.ScrollableText = Em.ContainerView
    .extend( {

        classNames: 'scrollableText',

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

        /** Css style of scrollableText */
        scrollableTextScrollingAttributes: '',

        /** Css style of line-height of rows */
        linesHeght: 23,

        linesCount: 1,

        scrollHeight: null,

        scrollableTextCount: function() {

            if ($('#' + this.get('childViews')[1].elementId)) {
                $('#' + this.get('childViews')[1].elementId)
                    .removeAttr('style');
                this.set('scrollHeight', $('#'
                    + this.get('childViews')[1].elementId)[0].scrollHeight);
                $('#' + this.get('childViews')[1].elementId)
                    .height(this.scrollHeight);
                this.set('linesCount', $('#'
                    + this.get('childViews')[1].elementId)[0].scrollHeight
                    / this.linesHeght);
            }
        },

        /** Pages count */
        pageCount: function() {

            return Math.ceil(this.get('linesCount') / this.get('itemsOnPage'));
        }.property('linesCount', 'itemsOnPage'),

        scrollableTextHeight: function() {

            return this.itemsOnPage * this.itemheight;
        }.property('itemsOnPage'),

        /** Action of element "sb-top" which show previous scrollableText page */
        sbUp: function() {

            if (this.get('currentPage') > 0) {
                this.set('currentPage', this.get('currentPage') - 1);
            }
        },

        /** Action of element "sb-bottom" which show previous scrollableText page */
        sbDown: function() {

            if (this.get('currentPage') < this.get('pageCount') - 1) {
                this.set('currentPage', this.get('currentPage') + 1);
            }
        },

        /** Scroll content according to current page */
        onCurrentPageChange: function() {

            this.set('scrollableTextScrollingAttributes', 'height: '
                + this.scrollHeight + 'px; top: '
                + (this.get('currentPage') * this.itemsOnPage * (-23)) + 'px');
        }.observes('currentPage'),

        /** Method for delete certain item from scrollableText */
        deleteItem: function(id) {

            this.items.splice(id, 1);
            this.scrollableText.refresh();
        },

        /** scrollableText components */
        childViews: [
            'scrollbar',
        // 'scrollableText'
        ],

        refreshTextArea: function() {

            if (this.get('childViews')[1]) {
                this.get('childViews').removeObject(this.get('childViews')[1]);
            }

            $('#scrollableTextArea').height(23);
            this.set('scrollHeight', 23);
            this.set('linesCount', 1);
            this.set('currentPage', 0);

            this
                .get('childViews')
                .pushObject(Ember.TextArea
                    .create( {

                        classNames: 'scrollableTextArea',

                        elementId: 'scrollableTextArea',

                        scrollableTextStyleBinding: 'parentView.scrollableTextScrollingAttributes',

                        attributeBindings: [
                            'scrollableTextStyle:style'
                        ],

                        valueBinding: 'this.parentView.items',

                        actionDown: function() {

                            return false;
                        },

                        didInsertElement: function() {

                            this.get('parentView').scrollableTextCount();
                        }
                    }));
        }.observes('items'),

        /** scrollableText view */
        scrollableText: Ember.TextArea
            .extend( {

                classNames: 'scrollableTextArea',

                elementId: 'scrollableTextArea',

                scrollableTextStyleBinding: 'parentView.scrollableTextScrollingAttributes',

                attributeBindings: [
                    'scrollableTextStyle:style'
                ],

                valueBinding: 'this.parentView.items',

                actionDown: function() {

                    return false;
                },

                didInsertElement: function() {

                    this._parentView.scrollableTextCount();
                },

                refresh: function() {

                    this.rerender();
                }.observes('_parentView.items')

            }),

        /** Scrollbar view */
        scrollbar: SDL.ScrollBar.extend( {
            currentPageBinding: 'parentView.currentPage',
            pageCountBinding: 'parentView.pageCount',
            scrollableTextHeightBinding: 'parentView.scrollableTextHeight',
            scrollBarIsDisabledBinding: 'parentView.disableScrollbar'
        })
    });
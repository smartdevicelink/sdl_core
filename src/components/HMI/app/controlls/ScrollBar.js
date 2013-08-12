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
 * @name SDL.ScrollBar
 * @desc ScrollBar component for List component
 * @category Controlls
 * @filesource app/controlls/ScrollBar.js
 * @version 1.0
 */

SDL.ScrollBar = Em.ContainerView
    .extend( {

        /** Define enable/disable scrollbar */
        classNameBindings: [
            'scrollBarIsDisabled:is-disabled'
        ],

        /** Componet class */
        classNames: 'scrollbar',

        /** Childs views */
        childViews: [
            'upButton', 'bar', 'downButton'
        ],

        /** current page */
        currentPage: 1,

        /** Pages count */
        pageCount: 0,

        listHeight: 250,

        /** On/OF scrollbar */
        scrollBarIsDisabled: false,

        /** Define bar height */
        sbHeight: function() {

            /** Max bar height */
            this.maxHeight = this.listHeight - 102;
            if (this.pageCount <= 1) {
                return this.maxHeight + 1;
            } else {
                return (this.maxHeight / this.pageCount);
            }
        }.property('pageCount'),

        /** Position of bar */
        sbTop: function() {

            if (this.get('currentPage') == 0) {
                return 0;
            } else {
                return (this.maxHeight - this.get('sbHeight'))
                    / (this.get('pageCount') - 1) * this.get('currentPage') + 1;
            }
        }.property('currentPage', 'pageCount'),

        /** Support function */
        scrollbarBodyStyleAttributes: function() {

            return 'height: ' + (this.get('listHeight') - 1) + 'px;';
        }.property('listHeight'),

        /** Support function */
        sbBodyStyleAttributes: function() {

            return 'height: ' + (this.get('listHeight') - 100 - 1) + 'px;';
        }.property('listHeight'),

        sbStyleAttributes: function() {

            return 'height: ' + this.get('sbHeight') + 'px; ' + 'top: '
                + this.get('sbTop') + 'px';
        }.property('currentPage', 'pageCount'),

        /** Define scroll up button "disable" status */
        sbUpButtonIsDisabled: function() {

            if (this.get('currentPage') < 1) {
                return true;
            } else {
                return false;
            }
        }.property('currentPage', 'pageCount'),

        /** Define scroll down button "disable" status */
        sbDownButtonIsDisabled: function() {

            if ((this.pageCount - 1) > this.get('currentPage')) {
                return false;
            } else {
                return true;
            }
        }.property('currentPage', 'pageCount'),

        attributeBindings: [
            'scrollbarBodyStyleAttributes:style'
        ],

        /** Bottom for scroll up */
        upButton: SDL.Button.extend( {
            classNames: [
                'sb-top', 'button'
            ],
            action: 'sbUp',
            target: 'parentView.parentView',
            disabledBinding: 'parentView.sbUpButtonIsDisabled',
            icon: 'images/list/scrollbar/button-up-active.png',
            timer: 200
        }),

        /** Bottom for scroll down */
        downButton: SDL.Button.extend( {
            classNames: [
                'sb-bottom', 'button'
            ],
            action: 'sbDown',
            target: 'parentView.parentView',
            disabledBinding: 'parentView.sbDownButtonIsDisabled',
            icon: 'images/list/scrollbar/button-down-active.png',
            timer: 200
        }),

        /** Scrollbar track */
        bar: Em.View
            .extend( {
                barBodyStyleBinding: 'parentView.sbBodyStyleAttributes',
                barStyleBinding: 'parentView.sbStyleAttributes',
                cancelAnimationBinding: 'parentView.parentView.cancelAnimation',

                layout: Em.Handlebars
                    .compile('<div class="sb-body" {{bindAttr style="view.barBodyStyle"}}>'
                        + '{{yield}}' + '</div>'),

                template: Em.Handlebars
                    .compile('<div class="sb-bar" '
                        + '{{bindAttr style="view.barStyle"}}'
                        + '{{bindAttr class="view.cancelAnimation:cancelBarAnimation"}}>'
                        + '</div>')
            })
    });
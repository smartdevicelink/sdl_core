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
 * @name SDL.MenuList
 * @desc
 * @category Controlls
 * @filesource app/controlls/MenuList.js
 * @version 1.0
 */

SDL.MenuList = Em.ContainerView
    .extend( {

        /**
         * Add new item to container
         * 
         * @param buttons:
         *            SoftButton[]
         */
        addItems: function(buttons, appID) {

            this.deleteItems();

            if (buttons) {
                for ( var i = 0; i < buttons.length; i++) {
                    this.get('content.childViews').pushObject(SDL.Button
                        .create(SDL.PresetEventsCustom, {
                            text: buttons[i].text,
                            icon: buttons[i].image ? buttons[i].image.value : "",
                            templateName: buttons[i].image ? 'rightText' : 'text',
                            groupName: "AlertPopUp",
                            classNameBindings: ['isHighlighted:isHighlighted'],
                            isHighlighted: buttons[i].isHighlighted ? true : false,
                            softButtonID: buttons[i].softButtonID,
                            systemAction: buttons[i].systemAction,
                            groupName: this.groupName,
                            classNames: 'softButton',
                            appID: appID
                        }));
                }
            }
        },

        /**
         * Delete existing Soft Buttons from container
         */
        deleteItems: function() {

            this.get('content.childViews').removeObjects(this.get('content.childViews').filterProperty('softButtonID'));
            this.get('content.childViews').removeObjects(this.get('content.childViews').filterProperty('softButtonID', 0));
        },

        classNames: [
            'ffw_list_menu'
        ],

        attributeBindings: [
            'elementHeight:style'
        ],

        // Position of current page
        page: 0,

        // Items per one page
        itemsOnPage: 5,

        // Height of one item
        ITEM_HEIGHT: 50,

        pageHeight: function() {

            return this.itemsOnPage * this.ITEM_HEIGHT;
        }.property('this.itemsOnPage'),

        elementHeight: function() {

            return 'height:'
                + String(this.get('pageHeight') + this.ITEM_HEIGHT - 2) + 'px;';
        }.property('this.pageHeight'),

        // Position of content block
        contentPositon: function() {

            return 'top:'
                + String(-(this.get('page') * this.get('pageHeight')))
                + 'px; height:'
                + String(this.get('pageHeight')
                    - (Boolean(this.get('onLastPage') && this.get('page')) * this.ITEM_HEIGHT))
                + 'px;';
        }.property('onLastPage'),

        // Property for show or hide 'down' button
        onLastPage: function() {

            return ((this.get('page') * this.itemsOnPage) < (this
                .get('content.childViews.length') - this.itemsOnPage));
        }.property('page', 'content.childViews.length'),

        // Handeler to go previous page
        pageUp: function() {

            if (this.get('page')) {
                this.set('page', (this.get('page') - 1));
            }
        },

        // Handeler to go next page
        pageDown: function() {

            if (this.get('onLastPage')) {
                this.set('page', (this.get('page') + 1));
            }
        },

        childViews: [
            'upButton', 'content', 'downButton'
        ],

        upButton: SDL.Button.extend( {

            classNames: [
                'control', 'up_button'
            ],

            hidden: function() {

                if (this.get('parentView.page')) {
                    return false;
                } else {
                    return true;
                }
            }.property('parentView.page'),

            templateName: 'icon',

            icon: 'images/media/ico_arrow_up.png',

            action: 'pageUp',

            onDown: false,

            target: 'parentView'
        }),

        downButton: SDL.Button.extend( {

            classNames: [
                'control', 'down_button'
            ],

            hidden: function() {

                if (this.get('parentView.onLastPage')) {
                    return false;
                } else {
                    return true;
                }
            }.property('parentView.onLastPage'),

            templateName: 'icon',

            icon: 'images/media/ico_arrow_down.png',

            action: 'pageDown',

            onDown: false,

            target: 'parentView'
        }),

        content: Em.ContainerView.extend( {

            classNames: [
                'content'
            ],

            attributeBindings: [
                'parentView.contentPositon:style'
            ]
        })
    });
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
 * @name SDL.TBTTurnList
 * @desc TBT Turn List visual representation
 * @category View
 * @filesource app/view/sdl/shared/tbtTurnList.js
 * @version 1.0
 */

SDL.TBTTurnList = SDL.SDLAbstractView.create({

        elementId: 'tbtTurnListView',

        childViews: [
            'backButton',
            'captionText',
            'tbtTurnListList'
        ],

        /**
         * Activate window
         */
        activate: function (appID) {

            this.set('active', true);
            this.updateList(appID);
        },

        /**
         * Deactivate window if TurnByTurnView window was closed
         */
        closeTurnList: function () {

            if (!SDL.TurnByTurnView.get('activeTBT')) {
                this.set('active', false);
            }
        }.observes('SDL.TurnByTurnView.activeTBT'),

        /**
         * Title Labes of window
         */
        captionText: SDL.Label.extend({
            classNames: 'caption-text',

            content: 'Turn List'
        }),

        /**
         * Turn List
         */
        tbtTurnListList: SDL.List.extend({
            elementId  : 'tbtTurnListList',
            itemsOnPage: 5,
            items      : []
        }),

        /**
         * Function updates content of Turn List with data from application's
         * model
         *
         * @type {Number}
         */
        updateList: function (appID) {

            this.get('tbtTurnListList.list').removeAllChildren();

            this.tbtTurnListList.list.refresh();

            var turnListArray = SDL.SDLController.getApplicationModel(appID).turnList,
                length = turnListArray.length;
            for (var i = 0; i < length; i++) {
                this.get('tbtTurnListList.list.childViews').pushObject(SDL.Label.create({
                        icon        : turnListArray[i].turnIcon ? turnListArray[i].turnIcon.value : "",
                        content     : turnListArray[i].navigationText ? turnListArray[i].navigationText.fieldText : "",
                        classNames  : 'list-item',
                        templateName: turnListArray[i].turnIcon ? 'icon' : ''
                    }));
            }

            turnListArray = SDL.SDLController.getApplicationModel(appID).turnListSoftButtons;
            length = turnListArray.length;
            
            for (var i = 0; i < length; i++) {
                this.get('tbtTurnListList.list.childViews').pushObject(SDL.Button.create(SDL.PresetEventsCustom, {
                        systemAction     : turnListArray[i].systemAction,
                        groupName        : "TBTList",
                        classNameBindings: [
                            'isHighlighted:isHighlighted'
                        ],
                        isHighlighted    : turnListArray[i].isHighlighted ? true : false,
                        softButtonID     : turnListArray[i].softButtonID,
                        icon             : turnListArray[i].image ? turnListArray[i].image.value : "",
                        text             : turnListArray[i].text,
                        classNames       : 'list-item softButton',
                        elementId        : 'softButton' + i,
                        templateName     : turnListArray[i].image ? 'rightIcon' : 'text',
                        appID            : appID
                    }));
            }
        }
    });

/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  · Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *  · Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *  · Neither the name of the Ford Motor Company nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 * @name SDL.OptionsView
 * @desc Options visual representation
 * @category View
 * @filesource app/view/sdl/shared/optionsView.js
 * @version 1.0
 */

SDL.OptionsView = SDL.SDLAbstractView.create({

    elementId: 'sdl_options',

    childViews:
        [
            'backButton',
            'captionText',
            'commands'
        ],

    // Menu caption text
    captionBinding: 'SDL.SDLAppController.model.currentSubMenuLabel',

    commands: SDL.List.extend({

        elementId: 'info_nonMedia_options_list',

        itemsOnPage: 5,

        items: [],

        /*
         * itemsDefault: [ { type: SDL.Button,
         * 
         * params: { templateName: 'text', text: 'Exit', target:
         * 'this.parentView.parentView.parentView', action: 'deactivate',
         * onDown: false } }, { type: SDL.Button,
         * 
         * params: { templateName: 'arrow', text: 'Device Information', } } ],
         */

        refreshItems: function() {

            if (SDL.SDLAppController.model) {

                var commands = SDL.SDLAppController.model.get('currentCommandsList'),
                    i,
                    len,
                    template;

                this.items = [];

                len = commands.length;

                for(i = 0; i < len; i++){

                    if(commands[i].menuID >= 0){
                        template = 'arrow';
                    }else{
                        template = commands[i].icon ? 'rightText' : 'text';
                    }

                    this.items.push({
                        type: SDL.Button,
                        params: {
                            templateName: template,
                            text: commands[i].name,
                            commandID: commands[i].commandID,
                            menuID: commands[i].menuID,
                            icon: commands[i].icon,
                            target: 'SDL.SDLAppController',
                            action: 'onCommand',
                            onDown: false
                        }
                    })
                }

                this.list.refresh();
            }

        }.observes('SDL.SDLAppController.model.currentSubMenuId', 'SDL.SDLAppController.model.currentCommandsList.@each')
    }),

    activate: function(text) {
        this._super();

        SDL.SDLAppController.buttonsSort('top', SDL.SDLAppController.model.appID);
        SDL.OptionsView.commands.refreshItems();

        SDL.SDLController.onSystemContextChange();
    },

    // Extend deactivate window
    deactivate: function() {

        if (SDL.SDLAppController.model) {

            if (SDL.SDLAppController.model.get('currentSubMenuId') >= 0) {
                SDL.SDLAppController.onSubMenu('top');
            }else{
                this._super();
                SDL.SDLController.onSystemContextChange();
            }
        }

    }
});

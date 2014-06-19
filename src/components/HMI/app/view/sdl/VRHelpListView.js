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
 * @name SDL.InteractionChoicesView
 * @desc Interaction Choices visual representation
 * @category View
 * @filesource app/view/sdl/shared/interactionChoicesView.js
 * @version 1.0
 */

SDL.VRHelpListView = SDL.SDLAbstractView.create( {

    elementId: 'vr_help_list_view',

    childViews: [
        'backButton', 'captionText', 'helpList'
    ],

    helpList: SDL.List.extend( {
        elementId: 'vr_help_list_view_list',
        itemsOnPage: 5,
        items: []
    }),

    /**
     * Deactivate window
     */
    deactivate: function() {

        this._super();

        if (SDL.SDLAppController.model && SDL.SDLModel.vrActiveRequests.vrPerformInteraction) {
            SDL.SDLController.vrInteractionResponse(SDL.SDLModel.resultCode['ABORTED']);
        }

        SDL.SDLController.VRMove();
    },

    /**
     * Clean choices caption and list before new proform
     */
    clean: function() {

        this.set('captionText.content', 'vrHelp');
        this.helpList.items = [];
        this.helpList.list.refresh();
    },

    /**
     * Update choises list with actual set id
     * 
     * @param data:
     *            Array
     */
    showVRHelp: function(vrHelpTitle, vrHelp) {

        this.clean();

        this.set('active', true);
        
        SDL.SDLController.VRMove();

        if (vrHelpTitle) {
            this.captionText.set('content', vrHelpTitle);
        }

        if (vrHelp) {
            for (i = 0; i < vrHelp.length; i++) {
                this.helpList.items.push( {
                    type: SDL.Label,
                    params: {
                        content: vrHelp[i].text,
                        icon: vrHelp[i].image
                            ? vrHelp[i].image.value : null,
                        templateName: vrHelp[i].image ? "icon" : null
                    }
                });
            }
        }

        this.helpList.list.refresh();
    }
});

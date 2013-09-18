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
 * @name SDL.PhoneModel
 * @desc Phone model
 * @category Model
 * @filesource app/model/PhoneModel.js
 * @version 1.0
 */

SDL.PhoneModel = Em.Object.create( {

    /** Dial number */
    dialpadNumber: '',

    readyForCall: true,

    onCall: false,

    /** Text for phone status bar */
    statusModeText: null,

    /** Start call */
    dialCall: function() {

        if (this.dialpadNumber.length) {
            this.set('onCall', true);
            this.set('readyForCall', false);
        }
    },

    /** End call */
    endCall: function() {

        this.set('onCall', false);
        this.set('readyForCall', true);
        this.set('dialpadNumber', '');

        this.set('statusModeText', null);

        if (SDL.SDLModel.phoneCall) {
            SDL.SDLModel.set('phoneCall', false);
            SDL.SDLController
                .getApplicationModel(SDL.SDLAppController.model.appID)
                .turnOnSDL();
        }
    },

    /** Add symbol to dial pad number */
    setDialpadNumber: function(key) {

        if (this.dialpadNumber.length < 14) {
            key = this.dialpadNumber + key;
            this.set('dialpadNumber', key);
        }
    },

    /** Delete last number symbol */
    deleteDialpadNumber: function() {

        if (this.dialpadNumber.length) {
            this.set('dialpadNumber', this.dialpadNumber.slice(0, -1));
        }
    }
})
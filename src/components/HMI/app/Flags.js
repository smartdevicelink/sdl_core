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
 * @name SDL
 * @desc flags for application flags used for diffeerent configurations of
 *       application As the same code base is used fro Production and RnD work
 *       pakackages, it is necessary to configare application for different
 *       needs/releases It is NOT recommended to extend this object with new
 *       flags. Each modification should be discussed with PM in advance
 * @category Application
 * @filesource app/AppFlags.js
 * @version 1.0
 */

FLAGS = Em.Object.create( {
    /**
     * Set language for localization
     */
    SET_LOCALIZATION: 'eng',

    /**
     * Emulate CAN network for Climate true for SDL Guide production build
     */
    EMULATE_CAN_CLIMATE: true,

    /**
     * Emulate Phone true for SDL Guide production build
     */
    EMULATE_PHONE: true,

    /**
     * Emulate WebSocket connection false for SDL Guide production build
     */
    EMULATE_WS: true,

    /**
     * Emulate Android environment it is necessary to debug some interfaces
     * under browser false for SDL Guide production build
     */
    EMULATE_ANDROID: false,

    /**
     * Extended logging only for debug and development !!! MUST BE false for
     * release build !!!
     */
    DEBUG_MODE: false,

    /** hide mouse cursor from screen */
    APP_HIDE_MOUSE_CURSOR: false,

    /**
     * show MCS screen in settings -> vehicle menu to be disabled until v2.1, in
     * version v2.1 this feature will be released to end user
     */
    MCS_ENABLED: false,

    /**
     * show MCS screen in settings -> vehicle menu to be disabled until v2.1, in
     * version v2.1 this feature will be released to end user
     */
    HELP_MODE: false,

    /**
     * Emulate CAN network for Multi Contour Seat false for SDL Guide production
     * build
     */
    MCS_EMULATE_CAN: true,

    /** Sync bladders activity in MCS */
    MCS_BLADDERS_SYNC: true,

    /**
     * Active concept for MCS massage feature
     */

    MCS_ACTIVE_CONCEPT: 1
} );
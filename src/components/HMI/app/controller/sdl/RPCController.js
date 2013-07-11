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
 * @name SDL.ValidateMessage
 * @desc RPC Controller
 * @category Controller
 * @filesource app/controller/sdl/RPCController.js
 * @version 1.0
 */

SDL.ValidateMessage = Em.Object.create({

    
    /**
     * Object that contains check methods that
     * returns true if mandatory fields is successful
     * and returns false if not
     */
    Navigation: Em.Object.create({

        /**
         * Validate method for request Navigation.ShowConstantTBT
         * 
         * @param {Object} params
         */
        ShowConstantTBT: function(params) {

            if (!params.navigationTexts || params.navigationTexts.length < 0) {
                return false;
            }
            if (!params.turnIcon || typeof params.turnIcon != 'string') {
                return false;
            }
            if (!params.distanceToManeuver || typeof params.distanceToManeuver != 'number') {
                return false;
            }
            if (!params.distanceToManeuver || typeof params.distanceToManeuverScale != 'number') {
                return false;
            }
            if (!params.appId || typeof params.appId != 'number') {
                return false;
            }
            
            return true;
        },
        
        /**
         * Validate method for request Navigation.UpdateTurnList
         * 
         * @param {Object} params
         */
        UpdateTurnList: function(params) {

            if (!params.turnList || params.turnList.length < 1) {
                return false;
            }
            
            return true;
        }

    })
});

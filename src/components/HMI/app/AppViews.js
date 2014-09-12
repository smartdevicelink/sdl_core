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
 * @name SDL
 * @desc Setup application container for all views
 * @category Application
 * @filesource app/AppViews.js
 * @version 1.0
 */

/** Appending views */
SDL.AppViews = Em.ContainerView.extend( {

    elementId: 'app',

    childViews: [
        SDL.HomeView,
        SDL.MediaView,
        SDL.NavigationAppView,
        SDL.InfoView,
        SDL.PhoneView,
        SDL.ClimateView,
        SDL.NavigationView,
        SDL.ControlButtons,
        SDL.SettingsView,
        SDL.TurnByTurnView,
        SDL.TBTTurnList,
        SDL.OptionsView,
        SDL.InteractionChoicesView,
        SDL.Keyboard,
        SDL.VRHelpListView,
        SDL.ScrollableMessage,
        SDL.SliderView,
        SDL.StatusClimateView,
        SDL.StatusMediaView,
        SDL.StatusNavigationView,
        SDL.StatusInfoView,
        SDL.StatusPhoneView,
        SDL.TopControls,
        SDL.BottomControls,
        SDL.TTSPopUp,
        SDL.AlertPopUp,
        SDL.AlertManeuverPopUp,
        SDL.AudioPassThruPopUp,
        SDL.VRPopUp,
        SDL.VehicleInfo,
        SDL.TBTClientStateView,
        SDL.DriverDistraction,
        SDL.ExitApp,
        SDL.SystemRequest
    ],

    /*
     * This method is called when the app is fully rendered and ready to be
     * displayed. We notify the backend to hide the splash and load internal
     * view modules
     */
    didInsertElement: function() {

        this._super();

        SDL.set('appReady', true);

        $(window).bind("beforeunload", function(e) {

            FFW.BasicCommunication.OnIgnitionCycleOver();

            FFW.BasicCommunication.disconnect();
            FFW.UI.disconnect();
            FFW.VR.disconnect();
            FFW.VehicleInfo.disconnect();
            FFW.TTS.disconnect();
            FFW.Buttons.disconnect();
            FFW.Navigation.disconnect();

            if(confirm('The "ignition off" emulation executed!')){
                return 'OK, Good Bye then';
            }
            else {
                e = e || event;
                if (e.preventDefault) {
                    e.preventDefault();
                }
                e.returnValue = false;
                return 'The "ignition off" emulation executed!';
            }
        })

    }
});

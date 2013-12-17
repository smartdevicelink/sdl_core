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
 * @name SDL.ServicesView
 * @desc Info Services module visual representation
 * @category Info
 * @filesource app/view/ServicesView.js
 * @version 1.0
 */

SDL.InfoServicesView = Em.View
    .create( {

        elementId: 'info_services_view',

        classNameBindings: [
            'SDL.States.info.services.active:active_state:inactive_state'
        ],

        /** Define module template */
        template: Ember.Handlebars
            .compile('<div class="inner-content">'
                + '<div id="info_sync_services" class="info-text">{{SDL.locale.label.view_info_services_syncServices}}</div>'
                + '<div id="info_traff" class="info-text">{{SDL.locale.label.view_info_services_trafficDirectionsAndInformation}}</div>'
                + '<div id="info_traffic_report" class="info-text">{{SDL.locale.label.view_info_services_getTheLatestTrafficRreport}}</div>'
                + '<div id="info_traffic_pts" class="info-text">{{SDL.locale.label.view_info_services_findBusinessesAndDownloadSaved}}<br>{{SDL.locale.label.view_info_services_Pts}}</div>'
                + '<div id="info_traffic_hear" class="info-text">{{SDL.locale.label.view_info_services_hearPersonalizedInformationMore}}</div>'
                + '<div id="info_connect" class="info-text">{{SDL.locale.label.view_info_services_connecttoServices}}</div>'
                + '<div id="info_log" class="info-text">{{SDL.locale.label.view_info_services_toActivate}}</div>'
                + '</div>')
    });
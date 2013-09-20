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
 * @name SDL.InfoAlertsView
 * @desc Info Alerts module visual representation
 * @category View
 * @filesource app/view/sinfo/InfoAlertsView.js
 * @version 1.0
 */

SDL.InfoAlertsView = Em.ContainerView.create( {
    /** Bind class for visual representation */
    classNameBindings: [
        'SDL.States.info.alerts.active:active_state:inactive_state'
    ],

    elementId: 'info_alerts',

    childViews: [
        'listCaption', 'mainlist', 'rightlist'
    ],

    listCaption: SDL.LabelPlusToggleButton.extend( {
        elementId: 'info_alerts_listCaption',
        labelContentBinding: 'SDL.locale.label.view_info_alerts_sort',
        tButtonValue: 0,
        tButtonRange: 2,
        tButtonLabelsBinding: 'SDL.locale.label.view_info_alerts_dateIcon',
        labelDisabled: true,
        tButtonDisabled: true
    }),

    mainlist: SDL.List.extend( {

        elementId: 'info_alerts_list',

        itemsOnPage: 6,

        items: [
            {
                type: SDL.Label,

                params: {
                    disabled: true
                }
            }, {
                type: SDL.Button,
                params: {
                    classNames: [
                        'ember-view ffw-button notpressed list-item disabled active'
                    ],
                    textBinding: 'SDL.locale.label.view_info_alerts_911Assist',
                    templateName: 'rightIcon',
                    icon: 'images/info/ico_emergcyAndEnvelope.png',
                    righticon: 'images/info/ico_arrowLock.png',
                    disabled: true
                }
            }
        ]
    }),

    rightlist: SDL.List.extend( {

        elementId: 'info_alerts_rightList',

        itemsOnPage: 5,

        disableScrollbar: true,

        items: [
            {
                type: SDL.Button,
                params: {
                    className: 'button',
                    textBinding: 'SDL.locale.label.view_info_alerts_view',
                    disabled: true
                }
            }, {
                type: SDL.Button,
                params: {
                    className: 'button',
                    textBinding: 'SDL.locale.label.view_info_alerts_delete',
                    disabled: true
                }
            }, {
                type: SDL.Button,
                params: {
                    className: 'button',
                    textBinding: 'SDL.locale.label.view_info_alerts_deleteAll',
                    disabled: true
                }
            }
        ]
    })
});
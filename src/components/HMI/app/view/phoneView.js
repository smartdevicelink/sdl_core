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
 * @name SDL.PhoneView
 * @desc Phone module visual representation
 * @category View
 * @filesource app/view/phone/PhoneView.js
 * @version 1.0
 */

SDL.PhoneView = Em.ContainerView.create( {

    elementId: 'phone',

    /** Bind class for visual representation */
    classNameBindings: [
        'SDL.States.phone.active:active_state:inactive_state'
    ],

    /** Initial phone components */
    childViews: [
        'menu', SDL.DialpadPhoneView
    ],

    /** Left menu */
    menu: Em.ContainerView.extend( {
        elementId: 'phone_menu',

        // classNameBindings: ['SDL.PhoneController.hideMenu:hide'],

        childViews: [
            'border', 'items'
        ],

        border: Em.View.extend( {
            classNames: 'ls_border',

            template: Ember.Handlebars
                .compile('<img class="bg" src="images/common/ls_border.png">')
        }),

        items: Em.ContainerView.extend( {
            classNames: 'ls-items',

            childViews: [
                'dialpadButton'
            ],

            dialpadButton: SDL.Button.extend( {
                elementId: 'phone_menu_dialpadButton',

                classNames: 'ls-item lsp1_p active_in_helpmode',
                classNameBindings: [
                    'SDL.States.phone.dialpad.active:phone_active'
                ],
                textBinding: Em.Binding
                    .oneWay('SDL.locale.label.view_phone_phone'),
                icon: 'images/phone/ico_phone.png',
                action: 'subState',
                target: 'SDL.PhoneController',

                stateName: 'dialpad'
            })
        })
    }),

    /** End call message window */
    endCallMessage: SDL.Label.extend( {
        elementId: 'phone_endCallMessage',

        contentBinding: Em.Binding
            .oneWay('SDL.locale.label.view_phone_popUp_callEnded')
    })
});
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
 * @name SDL.DialpadPhoneView
 * @desc Phone Dialpad module visual representation
 * @category View
 * @filesource app/view/phone/DialpadView.js
 * @version 1.0
 */

SDL.DialpadPhoneView = Em.ContainerView.create( {

    elementId: 'phone_dialpad',

    classNameBindings: 'SDL.States.phone.dialpad.active:active_state:inactive_state',

    childViews:
        [
            'number',
            'keypad',
            'deleteButton',
            'dialButton',
            'endButton'
        ],

    /** Number area */
    number: SDL.Label.extend( {
        elementId: 'phone_dialpad_number',

        contentBinding: Em.Binding.oneWay( 'SDL.PhoneController.model.dialpadNumber' )
    } ),

    /** Keypad block */
    keypad: Em.ContainerView.extend( {
        elementId: 'phone_dialpad_keypad',

        childViews:
            [
                'key1',
                'key2',
                'key3',
                'key4',
                'key5',
                'key6',
                'key7',
                'key8',
                'key9',
                'key0',
                'keyStar',
                'keyDiez'
            ],

        key1: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key1',

            classNames: 'left',

            text: '1',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key2: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key2',

            classNames: 'center',

            text: '2',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key3: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key3',

            classNames: 'right',

            text: '3',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key4: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key4',

            classNames: 'left',

            text: '4',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key5: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key5',

            classNames: 'center',

            text: '5',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key6: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key6',

            classNames: 'right',

            text: '6',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key7: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key7',

            classNames: 'left',

            text: '7',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key8: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key8',

            classNames: 'center',

            text: '8',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key9: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key9',

            classNames: 'right',

            text: '9',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        key0: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_key0',

            classNames: 'center',

            text: '0',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        keyStar: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_keyStar',

            classNames: 'left',

            text: '*',

            action: "keyPress",

            target: "SDL.PhoneController"
        } ),

        keyDiez: SDL.Button.extend( {
            elementId: 'phone_dialpad_keypad_keyDiez',

            classNames: 'right',

            text: '#',

            action: "keyPress",

            target: "SDL.PhoneController"
        } )
    } ),

    deleteButton: SDL.Button.extend( {
        elementId: 'phone_dialpad_deleteButton',

        classNameBindings:
            [
                'SDL.PhoneController.model.dialpadNumber.length:show'
            ],

        icon: 'images/phone/del.png',

        action: 'onDelete',
        target: 'SDL.PhoneController',

        disabledBinding: Em.Binding.oneWay( 'SDL.PhoneController.model.onCall' )
    } ),

    dialButton: SDL.Button.extend( {
        elementId: 'phone_dialpad_dialButton',

        textBinding: Em.Binding.oneWay( 'SDL.locale.label.view_phone_dial' ),
        icon: 'images/phone/dial.png',

        action: 'onDialCall',
        target: 'SDL.PhoneController',

        disabledBinding: Em.Binding.oneWay( 'SDL.PhoneController.model.onCall' )
    } ),

    endButton: SDL.Button.extend( {
        elementId: 'phone_dialpad_endButton',

        textBinding: Em.Binding.oneWay( 'SDL.locale.label.view_phone_end' ),
        icon: 'images/phone/end.png',

        action: 'onEndCall',
        target: 'SDL.PhoneController',

        disabledBinding: Em.Binding.oneWay( 'SDL.PhoneController.model.readyForCall' )
    } )
} );
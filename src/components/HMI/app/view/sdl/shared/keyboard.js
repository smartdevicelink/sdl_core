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
 * @name SDL.Keyboard
 * @desc Slider visual representation
 * @category View
 * @filesource app/view/sdl/shared/keyboard.js
 * @version 1.0
 */

SDL.Keyboard = SDL.SDLAbstractView.create({

    elementId: 'keyboard_view',

    childViews: [
        'backButton',
        'microphone',
        'searchBar',
        'controlls',
        'buttonsArea'
    ],

    /**
     * Extend deactivate method send SUCCESS response on deactivate with current
     * slider value
     */
    deactivate: function () {

        this._super();
        //FFW.UI.keyboardClose();
    },

    microphone: SDL.Button.extend({
        classNames: 'microphone'
    }),

    searchBar: Em.ContainerView.extend({

        classNames: 'searchBar',

        childViews: [
            'input',
            'clearBtn',
            'serchIcon'
        ],

        clearBtn: SDL.Button.extend({
            classNames: 'clearBtn',
            target    : 'SDL.SDLController',
            action    : 'clearBtn'
        }),

        serchIcon: Em.View.extend({
            classNames: 'serchIcon'
        }),

        input: Ember.View.extend({
            tagName: 'input',
            attributeBindings: ['disabled'],
            disabled: true
        })

    }),

    controlls: Em.ContainerView.extend({

        classNames: 'controlls',

        childViews: [
            'leftBtn',
            'numericBtn',
            'symbolBtn',
            'spaceBtn',
            'caseSwitchBtn',
            'localisationBtn',
            'searchBtn'
        ],

        leftBtn: SDL.Button.extend({
            classNames: 'leftBtn',
            text      : 'Left Button',
            target    : 'SDL.SDLController',
            action    : 'leftBtn'
        }),

        numericBtn: SDL.Button.extend({
            classNames: 'numericBtn',
            text      : '123',
            target    : 'SDL.SDLController',
            action    : 'numericBtn'
        }),

        symbolBtn: SDL.Button.extend({
            classNames: 'symbolBtn',
            text      : '!@#',
            target    : 'SDL.SDLController',
            action    : 'symbolBtn'
        }),

        spaceBtn: SDL.Button.extend({
            classNames: 'spaceBtn',
            text      : 'Space',
            target    : 'SDL.SDLController',
            action    : 'spaceBtn'
        }),

        caseSwitchBtn: SDL.Button.extend({
            classNames: 'caseSwitchBtn',
            text      : 'ABC',
            target    : 'SDL.SDLController',
            action    : 'caseSwitchBtn'
        }),

        localisationBtn: SDL.Button.extend({
            classNames: 'localisationBtn',
            icon      : 'icon',
            target    : 'SDL.SDLController',
            action    : 'localisationBtn'
        }),

        searchBtn: SDL.Button.extend({
            classNames: 'searchBtn',
            text      : 'Search',
            target    : 'SDL.SDLController',
            action    : 'searchBtn'
        })
    }),

    buttonsArea: SDL.SDLModel.keyboardLayout['QWERTY']

});

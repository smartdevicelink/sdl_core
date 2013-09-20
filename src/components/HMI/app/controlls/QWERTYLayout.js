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
 * @name SDL.QWERTYLayout
 * @desc General list component for SDL application
 * @category Controlls
 * @filesource app/controlls/QWERTYLayout.js
 * @version 1.0
 */

SDL.QWERTYLayout = Em.ContainerView.extend( {

    classNames: 'QWERTYLayout',

    childViews: [
        'q',
        'w',
        'e',
        'r',
        't',
        'y',
        'u',
        'i',
        'o',
        'p',
        'a',
        's',
        'd',
        'f',
        'g',
        'h',
        'j',
        'k',
        'l',
        'z',
        'x',
        'c',
        'v',
        'b',
        'n',
        'm',
        'dash',
        'ampersand'
    ],

    q: SDL.Button.extend({
        elementId : 'q',
        classNames: 'q k0',
        text      : 'q',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    w: SDL.Button.extend({
        elementId : 'w',
        classNames: 'w k1',
        text      : 'w',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    e: SDL.Button.extend({
        elementId : 'e',
        classNames: 'e k2',
        text      : 'e',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    r: SDL.Button.extend({
        elementId : 'r',
        classNames: 'r k3',
        text      : 'r',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    t: SDL.Button.extend({
        elementId : 't',
        classNames: 't k4',
        text      : 't',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    y: SDL.Button.extend({
        elementId : 'y',
        classNames: 'y k5',
        text      : 'y',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    u: SDL.Button.extend({
        elementId : 'u',
        classNames: 'u k6',
        text      : 'u',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    i: SDL.Button.extend({
        elementId : 'i',
        classNames: 'i k7',
        text      : 'i',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    o: SDL.Button.extend({
        elementId : 'o',
        classNames: 'o k8',
        text      : 'o',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    p: SDL.Button.extend({
        elementId : 'p',
        classNames: 'p k9',
        text      : 'p',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    a: SDL.Button.extend({
        elementId : 'a',
        classNames: 'a k10',
        text      : 'a',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    s: SDL.Button.extend({
        elementId : 's',
        classNames: 's k11',
        text      : 's',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    d: SDL.Button.extend({
        elementId : 'd',
        classNames: 'd k12',
        text      : 'd',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    f: SDL.Button.extend({
        elementId : 'f',
        classNames: 'f k13',
        text      : 'f',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    g: SDL.Button.extend({
        elementId : 'g',
        classNames: 'g k14',
        text      : 'g',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    h: SDL.Button.extend({
        elementId : 'h',
        classNames: 'h k15',
        text      : 'h',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    j: SDL.Button.extend({
        elementId : 'j',
        classNames: 'j k16',
        text      : 'j',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    k: SDL.Button.extend({
        elementId : 'k',
        classNames: 'k k17',
        text      : 'k',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    l: SDL.Button.extend({
        elementId : 'l',
        classNames: 'l k18',
        text      : 'l',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    z: SDL.Button.extend({
        elementId : 'z',
        classNames: 'z k20',
        text      : 'z',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    x: SDL.Button.extend({
        elementId : 'x',
        classNames: 'x k21',
        text      : 'x',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    c: SDL.Button.extend({
        elementId : 'c',
        classNames: 'c k22',
        text      : 'c',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    v: SDL.Button.extend({
        elementId : 'v',
        classNames: 'v k23',
        text      : 'v',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    b: SDL.Button.extend({
        elementId : 'b',
        classNames: 'b k24',
        text      : 'b',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    n: SDL.Button.extend({
        elementId : 'n',
        classNames: 'n k25',
        text      : 'n',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    m: SDL.Button.extend({
        elementId : 'm',
        classNames: 'm k26',
        text      : 'm',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    dash: SDL.Button.extend({
        elementId : 'dash',
        classNames: 'dash k27',
        text      : '-',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    }),

    ampersand: SDL.Button.extend({
        elementId : 'ampersand',
        classNames: 'ampersand k28',
        text      : '&',
        target    : 'SDL.SDLController',
        action    : 'keyboardPress'
    })
});
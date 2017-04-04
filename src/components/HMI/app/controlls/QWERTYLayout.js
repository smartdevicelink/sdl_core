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

    classNames: 'keyboardLayout QWERTYLayout',

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
        classNames: 'q k0',
        text      : 'q',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    w: SDL.Button.extend({
        classNames: 'w k1',
        text      : 'w',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    e: SDL.Button.extend({
        classNames: 'e k2',
        text      : 'e',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    r: SDL.Button.extend({
        classNames: 'r k3',
        text      : 'r',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    t: SDL.Button.extend({
        classNames: 't k4',
        text      : 't',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    y: SDL.Button.extend({
        classNames: 'y k5',
        text      : 'y',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    u: SDL.Button.extend({
        classNames: 'u k6',
        text      : 'u',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    i: SDL.Button.extend({
        classNames: 'i k7',
        text      : 'i',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    o: SDL.Button.extend({
        classNames: 'o k8',
        text      : 'o',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    p: SDL.Button.extend({
        classNames: 'p k9',
        text      : 'p',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    a: SDL.Button.extend({
        classNames: 'a k10',
        text      : 'a',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    s: SDL.Button.extend({
        classNames: 's k11',
        text      : 's',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    d: SDL.Button.extend({
        classNames: 'd k12',
        text      : 'd',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    f: SDL.Button.extend({
        classNames: 'f k13',
        text      : 'f',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    g: SDL.Button.extend({
        classNames: 'g k14',
        text      : 'g',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    h: SDL.Button.extend({
        classNames: 'h k15',
        text      : 'h',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    j: SDL.Button.extend({
        classNames: 'j k16',
        text      : 'j',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    k: SDL.Button.extend({
        classNames: 'k k17',
        text      : 'k',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    l: SDL.Button.extend({
        classNames: 'l k18',
        text      : 'l',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    z: SDL.Button.extend({
        classNames: 'z k21',
        text      : 'z',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    x: SDL.Button.extend({
        classNames: 'x k22',
        text      : 'x',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    c: SDL.Button.extend({
        classNames: 'c k23',
        text      : 'c',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    v: SDL.Button.extend({
        classNames: 'v k24',
        text      : 'v',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    b: SDL.Button.extend({
        classNames: 'b k25',
        text      : 'b',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    n: SDL.Button.extend({
        classNames: 'n k26',
        text      : 'n',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    m: SDL.Button.extend({
        classNames: 'm k27',
        text      : 'm',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    dash: SDL.Button.extend({
        classNames: 'dash k28',
        text      : '-',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    }),

    ampersand: SDL.Button.extend({
        classNames: 'ampersand k29',
        text      : '&',
        target    : 'parentView.parentView',
        action    : 'inputChanges'
    })
});
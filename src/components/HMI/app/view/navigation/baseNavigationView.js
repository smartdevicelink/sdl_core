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
 * @name SDL.BaseNavigationView
 * @desc Navigation component base screen
 * @category View
 * @filesource app/view/navigation/baseNavigationView.js
 * @version 1.0
 */
SDL.BaseNavigationView = Em.ContainerView.create({
    /** View Id */
    elementId: 'baseNavigation',

    classNameBindings: [
        'SDL.States.navigation.baseNavigation.active:active_state:inactive_state'
    ],

    childViews: [
        'mainField1',
        'mainField2',
        'mainField3',
        'mainField4',
        'mainField5',
        'mainField6',
        'softButton1',
        'softButton2',
        'softButton3',
        'softButton4'
    ],

    mainField1: SDL.Label.extend({

        classNames: 'mainField1 mainField',

        content: 'mainField1'
    }),

    mainField2: SDL.Label.extend({

        classNames: 'mainField2 mainField',

        content: 'mainField2'
    }),

    mainField3: SDL.Label.extend({

        classNames: 'mainField3 mainField',

        content: 'mainField3'
    }),

    mainField4: SDL.Label.extend({

        classNames: 'mainField4 mainField',

        content: 'mainField4'
    }),

    mainField5: SDL.Label.extend({

        classNames: 'mainField5 mainField',

        content: 'mainField5'
    }),

    mainField6: SDL.Label.extend({

        classNames: 'mainField6 mainField',

        content: 'mainField6'
    }),

    softButton1: SDL.Button.extend(SDL.PresetEventsCustom, {
        systemAction     : null,
        groupName        : "NaviBase",
        classNameBindings: ['isHighlighted:isHighlighted'],
        isHighlighted    : false,
        softButtonID     : null,
        icon             : "",
        text             : this.elementId,
        classNames       : 'list-item softButton softButton1',
        elementId        : 'softButton1',
        templateName     : 'text',
        appID            : null
    }),

    softButton2: SDL.Button.extend(SDL.PresetEventsCustom, {
        systemAction     : null,
        groupName        : "NaviBase",
        classNameBindings: ['isHighlighted:isHighlighted'],
        isHighlighted    : false,
        softButtonID     : null,
        icon             : "",
        text             : this.elementId,
        classNames       : 'list-item softButton softButton2',
        elementId        : 'softButton2',
        templateName     : 'text',
        appID            : null
    }),

    softButton3: SDL.Button.extend(SDL.PresetEventsCustom, {
        systemAction     : null,
        groupName        : "NaviBase",
        classNameBindings: ['isHighlighted:isHighlighted'],
        isHighlighted    : false,
        softButtonID     : null,
        icon             : "",
        text             : this.elementId,
        classNames       : 'list-item softButton softButton3',
        elementId        : 'softButton3',
        templateName     : 'text',
        appID            : null
    }),

    softButton4: SDL.Button.extend(SDL.PresetEventsCustom, {
        systemAction     : null,
        groupName        : "NaviBase",
        classNameBindings: ['isHighlighted:isHighlighted'],
        isHighlighted    : false,
        softButtonID     : null,
        icon             : "",
        text             : this.elementId,
        classNames       : 'list-item softButton softButton4',
        elementId        : 'softButton4',
        templateName     : 'text',
        appID            : null
    })

});

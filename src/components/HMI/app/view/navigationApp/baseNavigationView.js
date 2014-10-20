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
 * @filesource app/view/navigationApp/baseNavigationView.js
 * @version 1.0
 */
SDL.BaseNavigationView = Em.ContainerView.create({
    /** View Id */
    elementId: 'baseNavigation',

    classNameBindings: [
        'SDL.States.navigationApp.baseNavigation.active:active_state:inactive_state'
    ],

    childViews: [
        'mainField1',
        'mainField2',
        'mainField3',
        'mainField4',
        'mainField5',
        'mainField6',
        'optionsBtn'
    ],

    mainField1: null,
    mainField2: null,
    mainField3: null,
    mainField4: null,
    mainField5: null,
    mainField6: null,

    update: function(){

        var naviParams = SDL.SDLModel.constantTBTParams;

        if (naviParams) {

            for ( var i = 0; i < naviParams.navigationTexts.length; i++) {
                switch (naviParams.navigationTexts[i].fieldName) {
                    case 'navigationText1': {
                        this.set('mainField1',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'navigationText2': {
                        this.set('mainField2',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'ETA': {
                        this.set('mainField4', naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'totalDistance': {
                        this.set('mainField5',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'navigationText': {
                        this.set('mainField6',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'timeToDestination': {
                        this.set('mainField3',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    default :{
                        break;
                    }
                }
            }

            if (naviParams.softButtons) {
                for ( var i = 0; i < naviParams.softButtons.length; i++) {
                    this.get('childViews').pushObject(SDL.Button.create(SDL.PresetEventsCustom, {
                            text: naviParams.softButtons[i].text,
                            icon: naviParams.softButtons[i].image ? naviParams.softButtons[i].image.value : "",
                            templateName: naviParams.softButtons[i].image ? 'rightText' : 'text',
                            groupName: "NaviBase",
                            classNameBindings: ['isHighlighted:isHighlighted'],
                            isHighlighted: naviParams.softButtons[i].isHighlighted ? true : false,
                            softButtonID: naviParams.softButtons[i].softButtonID,
                            systemAction: naviParams.softButtons[i].systemAction,
                            classNames: 'navButton softButton softButton'  + (i + 1),
                            appID: SDL.SDLAppController.model.appID
                        }));
                }
            }
        }
    },

    mainField1: SDL.Label.extend({

        classNames: 'mainField1 mainField',

        contentBinding: 'SDL.SDLAppController.model.appInfo.field1'
    }),

    mainField2: SDL.Label.extend({

        classNames: 'mainField2 mainField',

        contentBinding: 'SDL.SDLAppController.model.appInfo.field2'
    }),

    mainField3: SDL.Label.extend({

        classNames: 'mainField3 mainField',

        contentBinding: 'SDL.SDLAppController.model.appInfo.field3'
    }),

    mainField4: SDL.Label.extend({

        classNames: 'mainField4 mainField',

        contentBinding: 'SDL.SDLAppController.model.appInfo.field4'
    }),

    mainField5: SDL.Label.extend({

        classNames: 'mainField5 mainField',

        contentBinding: 'parentView.mainField5'
    }),

    mainField6: SDL.Label.extend({

        classNames: 'mainField6 mainField',

        contentBinding: 'parentView.mainField6'
    }),

    optionsBtn: SDL.Button.extend({
        classNames: 'naviOptionsBtn navButton',
        text : "Options",
        action: 'openCommandsList',
        target: 'SDL.SDLAppController'
    })

});

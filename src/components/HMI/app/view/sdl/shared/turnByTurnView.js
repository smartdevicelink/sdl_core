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
 * @name SDL.TurnByTurnView
 * @desc TurnByTurnView module visual representation
 * @category View
 * @filesource app/view/sdl/shared/turnByTurnView.js
 * @version 1.0
 */

SDL.TurnByTurnView = SDL.SDLAbstractView.create( {

    elementId: 'TurnByTurnView',

    classNames: 'TurnByTurnView',

    classNameBindings: [
        'activeTBT:active_state:inactive_state'
    ],

    active: false,

    activeTBT: false,

    childViews: [
        'captionText',
        'softButtons',
        'totalDistanceLabel',
        'etaLabel',
        'turnList',
        'homeScreen',
        'navigationText2',
        'turnIconImage',
        'nextTurnIconImage',
        'distanceToManeuver',
        'distanceToManeuverScale',
        'timeToDestination'
    ],

    appID: -1,
    navigationText2: null,
    eta: null,
    totalDistance: null,
    turnIcon: null,
    nextturnIcon: null,
    distanceToManeuver: null,
    distanceToManeuverScale: null,
    timeToDestination: null,
    maneuverComplete: null,

    activate: function(appID) {

        var naviParams = SDL.SDLController.getApplicationModel(appID).constantTBTParams;

        if (naviParams) {

            for ( var i = 0; i < naviParams.navigationTexts.length; i++) {
                switch (naviParams.navigationTexts[i].fieldName) {
                    case 'navigationText1': {
                        this.set('captionText.content',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'navigationText2': {
                        this.set('navigationText2',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'ETA': {
                        this.set('eta', naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                    case 'totalDistance': {
                        this.set('totalDistance',
                            naviParams.navigationTexts[i].fieldText);
                        break;
                    }
                }
            }

            this.softButtons.addItems(naviParams.softButtons, appID);

            if (naviParams.maneuverComplete) {
                this.set('maneuverComplete', naviParams.maneuverComplete);
            }

            this.set('appID', naviParams.appID);

            if (naviParams.turnIcon) {

                this.set('turnIcon', naviParams.turnIcon.value);
            }

            if (naviParams.nextTurnIcon) {

                this.set('nextTurnIcon', naviParams.nextTurnIcon.value);
            }
            this.set('distanceToManeuver', naviParams.distanceToManeuver);
            this.set('distanceToManeuverScale', naviParams.distanceToManeuverScale);
            this.set('timeToDestination', naviParams.timeToDestination);

            this.set('activeTBT', true);
        }
    },

    /**
     * Deactivate View
     */
    deactivate: function() {

        this.set('activeTBT', false);
    },

    totalDistanceLabel: SDL.Label.extend( {
        classNames: 'totalDistanceLabel',
        contentBinding: 'parentView.totalDistance'
    }),

    distanceToManeuverScale: SDL.Label.extend( {
        classNames: 'distanceToManeuverScale',
        contentBinding: 'parentView.distanceToManeuverScale'
    }),

    distanceToManeuver: SDL.Label.extend( {
        classNames: 'distanceToManeuver',
        contentBinding: 'parentView.distanceToManeuver'
    }),

    timeToDestination: SDL.Label.extend( {
        classNames: 'timeToDestination',
        contentBinding: 'parentView.timeToDestination'
    }),

    etaLabel: SDL.Label.extend( {
        classNames: 'etaLabel',
        contentBinding: 'parentView.eta'
    }),

    turnList: SDL.Button.create( {
        elementID: 'turnList',
        classNames: 'turnList btn',
        text: 'Turn List',
        action: function() {

            SDL.SDLController.tbtTurnList(this._parentView.appID);
        },
        target: '',
        onDown: false,
        templateName: 'arrow'
    }),

    turnIconImage: Em.View.create( {
        classNames: 'turnIcon btn',
        attributeBindings: [
            'style'
        ],
        style: function() {

            if (this._parentView.turnIcon) {
                return 'background-image: URL(' + this._parentView.turnIcon
                    + ');';
            } else {
                return '';
            }
        }.property('this.parentView.turnIcon')
    }),

    nextTurnIconImage: Em.View.create( {
        classNames: 'nextTurnIcon btn',
        attributeBindings: [
            'style'
        ],
        style: function() {

            if (this._parentView.nextTurnIcon) {
                return 'background-image: URL(' + this._parentView.nextTurnIcon
                    + ');';
            } else {
                return '';
            }
        }.property('this.parentView.nextTurnIcon')
    }),

    navigationText2: SDL.Label.extend( {
        classNames: 'navigationText2',
        contentBinding: 'parentView.navigationText2'
    }),

    homeScreen: SDL.Button.create( {
        elementId: 'homeScreen',
        classNames: 'homeScreen btn',
        text: 'Home Screen',
        iconBinding: 'SDL.SDLAppController.model.appIcon',
        target: 'this.parentView',
        action: 'deactivate',
        onDown: false
    }),

    softButtons: SDL.MenuList.extend( {

        itemsOnPage: 3,

        groupName: "TurnByTurnView",

        content: Em.ContainerView.extend( {

            classNames: [
                'content'
            ]

        })
    })
});
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
 * @name SDL.TurnByTurnView
 * @desc TurnByTurnView module visual representation
 * @category View
 * @filesource app/view/sdl/shared/turnByTurnView.js
 * @version 1.0
 */

SDL.TurnByTurnView = SDL.SDLAbstractView.create( {

    elementId: 'TurnByTurnView',

    classNames: 'TurnByTurnView',

    classNameBindings:
        [
            'active:active'
        ],

    active: false,

    childViews:
        [
            'captionText',
            'softButtons',
            'totalDistanceLabel',
            'etaLabel',
            'turnList',
            'homeScreen',
            'navigationText2',
            'turnIconImage'
        ],

    appID: -1,
    navigationText2: null,
    eta: null,
    totalDistance: null,
    turnIcon: null,
    distanceToManeuver: null,
    distanceToManeuverScale: null,
    maneuverComplete: null,

    activate: function( params ) {
        if( params ){
            
            for (var i = 0; i < params.navigationTexts.length; i++) {
                switch (params.navigationTexts[i].fieldName) {
                    case 'navigationText1': {
                        this.set( 'captionText.content', params.navigationTexts[i].fieldText );
                        break;
                    }
                    case 'navigationText2': {
                        this.set( 'navigationText2', params.navigationTexts[i].fieldText );
                        break;
                    }
                    case 'ETA': {
                        this.set( 'eta', params.navigationTexts[i].fieldText );
                        break;
                    }
                    case 'totalDistance': {
                        this.set( 'totalDistance', params.navigationTexts[i].fieldText );
                        break;
                    }
                }
            }
            
            if (params.softButtons) {
                this.softButtons.addItems( params.softButtons );
            }
            if (params.maneuverComplete){
                this.set( 'maneuverComplete', params.maneuverComplete );
            }
            this.set( 'appID', params.appID );
            this.set( 'turnIcon', params.turnIcon );
            this.set( 'distanceToManeuver', params.distanceToManeuver );
            this.set( 'distanceToManeuverScale', params.distanceToManeuverScale );

            this.set( 'active', true );
        }
    },

    /**
     * Deactivate View
     */
    deactivate: function() {
        this.set( 'active', false );
    },

    totalDistanceLabel: SDL.Label.extend( {
        classNames: 'totalDistanceLabel',
        contentBinding: 'parentView.totalDistance'
    } ),

    etaLabel: SDL.Label.extend( {
        classNames: 'etaLabel',
        contentBinding: 'parentView.eta'
    } ),

    turnList: SDL.Button.create( {
        elementID: 'turnList',
        classNames: 'turnList btn',
        text: 'Turn List',
        action: function() {
            SDL.SDLController.tbtTurnList( this._parentView.appID );
        },
        target: '',
        onDown: false,
        templateName: 'arrow'
    } ),

    turnIconImage: Em.View.create( {
        classNames: 'turnIcon btn',
        attributeBindings:
            [
                'style'
            ],
        style: function() {
            if( this._parentView.turnIcon ){
                return 'background-image: URL(' + this._parentView.turnIcon + ');';
            }else{
                return '';
            }
        }.property( 'this.parentView.turnIcon' )
    } ),

    navigationText2: SDL.Label.extend( {
        classNames: 'navigationText2',
        contentBinding: 'parentView.navigationText2'
    } ),

    homeScreen: SDL.Button.create( {
        elementId: 'homeScreen',
        classNames: 'homeScreen btn',
        text: 'Home Screen',
        iconBinding: 'SDL.SDLAppController.model.appIcon',
        target: 'this.parentView',
        action: 'deactivate',
        onDown: false
    } ),

    softButtons: SDL.MenuList.extend( {

        itemsOnPage: 3,

        groupName: "TurnByTurnView",

        content: Em.ContainerView.extend( {

            classNames:
                [
                    'content'
                ]

        } )
    } )
} );
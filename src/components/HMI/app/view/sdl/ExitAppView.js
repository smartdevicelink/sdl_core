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
 * @name SDL.ExitApp
 * @desc Exit All Applications reason select visual representation
 * @category View
 * @filesource app/view/sdl/ExitAppView.js
 * @version 1.0
 */

SDL.ExitApp = Em.ContainerView.create( {

	elementId: 'exitAppView',

    classNames: 'exitAppView',

    classNameBindings:
        [
            'active'
        ],

    childViews:
        [
            'exitAppViewLabel',
            'exitAppViewTitle',
            'exitAppViewSelect'
        ],

    /**
     * Title of VehicleInfo PopUp view
     */
    exitAppViewLabel: SDL.Label.extend( {

        elementId: 'exitAppViewLabel',

        classNames: 'exitAppViewLabel',

        content: 'Exit Application'
    } ),

    /**
     * Property indicates the activity state of TBTClientStateView
     */
    active: false,

    /**
     * Title of tbtClientState group of parameters
     */
    exitAppViewTitle: SDL.Label.extend( {

        elementId: 'exitAppViewTitle',

        classNames: 'exitAppViewTitle',

        content: 'Exit Application reason'
    } ),

    /**
     * HMI element Select with parameters of TBTClientStates
     */
    exitAppViewSelect: Em.Select.extend( {

        elementId: 'exitAppViewSelect',

        classNames: 'exitAppViewSelect',

        contentBinding: 'SDL.SDLModel.exitAppState',

        optionValuePath: 'content.id',

        optionLabelPath: 'content.name',

        /**
         * Selected data sent on model for further processing
         */
        click: function() {

            SDL.SDLController.exitAppViewSelected( this.selection.name );

        }
    } ),

    /**
     * Trigger function that activates and deactivates tbtClientStateView
     */
    toggleActivity: function() {
        this.toggleProperty( 'active' );
    }
} );
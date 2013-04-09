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
 * @name SDL.LablePlusButton
 * @desc Universal label component with button for SDL application
 * @category Controlls
 * @filesource app/controlls/LablePlusButton.js
 * @version 1.0
 */

SDL.LablePlusButton = Em.ContainerView.extend( {
    classNames:
        [
            'lablePlusButton'
        ],

    /** Label text */
    labelContent: '',

    /** Label ico */
    labelIcon: null,

    /** Class of button * */
    buttonClasses: [],

    /** Labels of button */
    buttonText: '',

    /** Action of buttno click* */
    buttonAction: null,

    /** Target of button action * */
    buttonTarget: null,

    /** Button right text * */
    buttonRightText: null,

    /** Enable of button arrow* */
    templateName: 'text',

    /** Button icon* */
    buttonIcon: null,

    /** Button time delay* */
    buttonTimer: null,

    /** Touch leave event flag */
    buttonTouchleave: false,

    /** Disable lable */
    labelDisabled: false,

    /** Enable of button down click action* */
    buttonOnDown: true,

    /** Button help-mode* */
    buttonHelpMode: false,

    /** Disable toggle button */
    buttonDisabled: false,

    childViews:
        [
            'label',
            'button'
        ],

    /** Item lable */
    label: SDL.Label.extend( {
        classNames: 'lable',
        contentBinding: 'parentView.labelContent',
        iconBinding: 'parentView.labelIcon',
        disabledBinding: 'parentView.labelDisabled'
    } ),

    /** Item toggle button */
    button: SDL.Button.extend( {
        classNames: 'button',
        classNameBindings: 'parentView.buttonClasses',
        textBinding: 'parentView.buttonText',
        actionBinding: 'parentView.buttonAction',
        targetBinding: 'parentView.buttonTarget',
        rightTextBinding: 'parentView.buttonRightText',
        templateNameBinding: 'parentView.templateName',
        iconBinding: 'parentView.buttonIcon',
        timerBinding: 'parentView.buttonTimer',
        touchleaveBinding: 'parentView.buttonTouchleave',
        onDownBinding: 'parentView.buttonOnDown',
        helpModeBinding: 'parentView.buttonHelpMode',
        disabledBinding: 'parentView.buttonDisabled'
    } )
} );
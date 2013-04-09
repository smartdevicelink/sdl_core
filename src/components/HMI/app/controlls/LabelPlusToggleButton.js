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
 * @name SDL.LabelPlusToggleButton
 * @desc Universal label component with toggle buttons for SDL application
 * @category Controlls
 * @filesource app/controlls/LabelPlusToggleButton.js
 * @version 1.0
 */

SDL.LabelPlusToggleButton = Em.ContainerView.extend( {

    classNames:
        [
            'lableToggleButton'
        ],

    /** Label text */
    labelContent: '',

    /** Label ico */
    labelIcon: null,

    templateName: '',

    /** Index of selected item */
    tButtonValue: '',

    /** Number of toggle buttons */
    tButtonRange: '',

    /** Array of labels of toggle buttons */
    tButtonLabels:
        [
            ''
        ],

    /** Disable lable */
    labelDisabled: false,

    /** Disable toggle button */
    tButtonDisabled: false,

    tButtonsClases: 'toggles button',

    classContainer:
        [
            'toogleButtonContainer'
        ],

    childViews:
        [
            'label',
            'buttonsGroup'
        ],

    /** Item lable */
    label: SDL.Label.extend( {
        classNames:
            [
                'lableToggle'
            ],
        contentBinding: 'parentView.labelContent',
        iconBinding: 'parentView.labelIcon',
        templateNameBinding: 'parentView.templateName',
        disabledBinding: 'parentView.labelDisabled'
    } ),

    /** Item toggle button */
    buttonsGroup: SDL.ButtonsGroup.extend( {
        classNameBindings: 'parentView.classContainer',
        valueBinding: 'parentView.tButtonValue',
        rangeBinding: 'parentView.tButtonRange',
        labelsBinding: 'parentView.tButtonLabels',
        disabledBinding: 'parentView.tButtonDisabled',
        classesBinding: 'this._parentView.tButtonsClases',
        controllerBinding: 'SDL.SettingsController'
    } )
} );
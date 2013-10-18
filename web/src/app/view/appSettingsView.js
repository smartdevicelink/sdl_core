/**
 * @name MFT.appSettings
 * 
 * @desc App Settings module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/appSettingsView.js
 * @version		2.0
 *
 * @author		Andrii Velychko
 */
MFT.appSettings = Em.ContainerView.create(MFT.LoadableView,{
	/** View Id */
	elementId:	'appSettings',

    classNames: 'appsettings-block',
 		
	/** Media components */		
	childViews: [
        'topTitle',
		'list',
        'errorSendMailPopup'
	],

    topTitle: MFT.Label.extend({
        elementId:		   'appsettings-head',
        classNames:		  ['appsettings-head'],
        contentBinding:	 'MFT.locale.label.view_appSettings_head'
    }),

    list:	MFT.List.extend( {

        elementId:	  			  'appsettings-list',

        classNames: 'center-btn',

        disableScrollbar:	true,

        items:[
            {
                type:		MFT.LabelPlusToggleButton,
                params:		{
                    labelContentBinding:    'MFT.locale.label.view_appSettings_fullScreen',
                    tButtonValueBinding:    'MFT.AppSettingsController.fullScreen',
                    tButtonRange:		    2,
                    classNameBindings: 'FFW.isWindows:hidden',
                    tButtonLabelsBinding:   'MFT.locale.label.view_appSettings_fullScreen_OnOFF',
                    labelDisabled: 		    false,
                    tButtonDisabled:	    false,
                    classContainer:	        ['toogleButtonContainer doubletoggle']
                }
            },
            {
                type:		MFT.Button,
                params:		{
                    goToState:   'configure',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_appSettings_configureMyFordTouchGuide',
                    templateName: 'arrow',
                    disabled:	false,
                    target:	  'MFT.AppSettingsController'
                }
            },
            {
                type:		MFT.Button,
                params:		{
                    goToState:   'appRate',
                    action:	  'onChildState',
                    className:  'button',
                    classNameBindings: 'FFW.isWindows:hidden',
                    textBinding: 'MFT.locale.label.view_appSettings_rateMyFordTouchGuid',
                    templateName: 'arrow',
                    disabled:	false,
                    target:	  'MFT.AppSettingsController'
                }
            },
            {
                type:		MFT.Button,
                params:		{
                    goToState:   'about',
                    action:	  'onChildState',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_appSettings_aboutMyFordTouchGuid',
                    templateName: 'arrow',
                    disabled:	false,
                    target:	  'MFT.AppSettingsController'
                }
            }
        ]
    }),
    errorSendMailPopup: Em.ContainerView.create({

        elementId: 'errorSendMailPopup',

        classNameBindings: ['MFT.AppSettingsController.errorSendMailPopupIsVisible:popUpIsVisible'],

        isVisible: true,

        childViews: [
            'wrapper',
            'content'
        ],

        wrapper: Em.View.extend({
            elementId: 'errorSendMailPopup__content_wrapper',
            classNames: ['wrapper']
        }),

        content:  Em.ContainerView.extend({

            elementId: 'errorSendMailPopup_content',

            classNames: ['content'],


            childViews: [
                'icon',
                'title',
                'separator',
                'text',
                'okButton'
            ],

            icon: Em.View.extend({
                elementId:  'errorSendMailPopup_content_icon',
                classNames: 'icon'
            }),

            title: MFT.Label.extend({
                elementId:  'errorSendMailPopup_content_title',
                classNames: 'title',
                content:	MFT.locale.label.view_appSettings_errorSendMailPopup_title
            }),

            separator: Em.View.extend({
                elementId: 'errorSendMailPopup_content_separator',
                classNames: 'separator'
            }),

            text: Em.View.extend({
                elementId:	'errorSendMailPopup_content_message',
                classNames:   'text',
                template: Em.Handlebars.compile(
                    '<p>{{MFT.locale.label.view_appSettings_errorSendMailPopup_text}}</p>'
                )
            })
            ,

            okButton: MFT.Button.extend({
                elementId: 			 'errorSendMailPopup_content_okButton',
                classNames:			'rateDialogue_button okButton',
                text:	MFT.locale.label.view_appSettings_errorSendMailPopup_ok,
                action:			    'hideErrorSendMailPopup',
                target:			    'MFT.AppSettingsController'
            })
        })
    })
});
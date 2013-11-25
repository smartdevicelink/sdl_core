/**
 * @name MFT.SDLPopupsView
 *
 * @desc Leaving MFT Guide SDL popups visual representation
 *
 * @category	View
 * @filesource	app/view/SDLPopupsView.js
 * @version		1.0
 *
 * @author		Sergei Polishchuk
 */
MFT.SDLPopupsView = Em.ContainerView.create({
    elementId: 'SDLPopupsView',

    childViews: [
        'messagesPopup',
        'grantAccessPopup'
    ],

    messagesPopup: Em.ContainerView.create({
        classNames: 'popup',
        elementId: 'SDLMessagesPopup',

        childViews: [
            'window'
        ],

        window: Em.ContainerView.create({
            classNames: 'popup-window',
            elementId: 'SDLMessagesPopupWindow',
            childViews: [
                'label'
            ],

            label: MFT.Label.create({
                elementId: 'SDLMessagesPopupLabel',
                contentBinding: 'MFT.AppController.popUpMessage'
            })
        })
    }),

    grantAccessPopup: Em.ContainerView.create({
        classNames: 'popup',
        elementId: 'SDLGrantAccessPopup',

        childViews: [
            'window'
        ],

        window: Em.ContainerView.create({
            classNames: 'popup-window',
            elementId: 'SDLGrantAccessPopupWindow',
            childViews: [
                'label',
                'makeRequest',
                'close'
            ],

            label: MFT.Label.create({
                elementId: 'SDLGrantAccessPopupLabel',
                content: MFT.locale.label.view_sdl_grantAccess_message_popup_label
            }),

            makeRequest: MFT.Button.extend({
                elementId:	'SDLGrantAccessPopup_makeRequestButton',
                classNames:			['rs-item'],
                target:				'MFT.AppController',
                action:				'sendAccessRequest',
                onDown: false,
                templateName: 'text',
                text: MFT.locale.label.view_sdl_grantAccess_message_popup_makeRequest
            }),

            close: MFT.Button.extend({
                elementId:	'SDLGrantAccessPopup_closeButton',
                classNames:			['rs-item'],
                target:				'MFT.AppController',
                action:				'hideSDLGrantAccessPopup',
                onDown: false,
                templateName: 'text',
                text: MFT.locale.label.view_sdl_grantAccess_message_popup_close
            })
        })
    })
});
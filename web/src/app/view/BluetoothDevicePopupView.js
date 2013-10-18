/**
 * @name MFT.BluetoothDevicePopupView
 *
 * @desc Leaving MFT Guide Add Bluetooth Device Popup visual representation
 *
 * @category	View
 * @filesource	app/view/BluetoothDevicePopupView.js
 * @version		1.0
 *
 * @author		Sergei Polishchuk
 */
MFT.BluetoothDevicePopupView = Em.ContainerView.create({
    elementId: 'BluetoothDevicePopupView',

    childViews: [
        'deviceListBTPopup',
        'connectBTPopup'
    ],
    
    deviceListBTPopup: Em.ContainerView.create({
        classNames: 'popup',
        elementId: 'deviceListBTPopup',

        childViews: [
            'window'
        ],

        window: Em.ContainerView.create({
            classNames: 'popup-window',
            elementId: 'deviceListBTPopupWindow',
            childViews: [
                'label',
                'list',
                'button'
            ],

            label: MFT.Label.create({
                elementId: 'deviceListBTPopupLabel',
                contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_bt_selectDevicePopap_label')
            }),

            list: MFT.List.extend({

                elementId: 'deviceListBTPopupList',

                classNames: 'device-list',

                itemsOnPage: 3,
                disableScrollbar: true,

                items: [
                    {
                        type: MFT.Button,
                        params: {
                            className: 'button',
                            textBinding: 'MFT.BTModel.deviceList.0.name',
                            classNameBindings: 'MFT.BTModel.deviceList.0.isConnected:connected',
                            contentBinding: 'MFT.BTModel.deviceList.0',
                            templateName: 'text',
                            target: 'MFT.MediaController',
                            action: 'connectBTDevice',
                            disabledBinding: 'MFT.BTModel.deviceList.0.isConnected:connected'
                        }
                    },
                    {
                        type: MFT.Button,
                        params: {
                            className: 'button',
                            classNameBindings: 'MFT.BTModel.deviceList.1.isConnected:connected',
                            textBinding: 'MFT.BTModel.deviceList.1.name',
                            contentBinding: 'MFT.BTModel.deviceList.1',
                            templateName: 'text',
                            target: 'MFT.MediaController',
                            action: 'connectBTDevice',
                            disabledBinding: 'MFT.BTModel.deviceList.1.isConnected:connected'
                        }
                    },
                ]
            }),
            button: MFT.Button.create({
                elementId: 'deviceListBTPopupClose',
                classNames: ['close_button'],
                target: 'MFT.MediaController',
                action:	'hideDeviceListBTPopup',
                onDown: false,
                textBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_bt_popap_close')
            })

        })
    }),
    connectBTPopup: Em.ContainerView.create({
        classNames: 'popup',
        elementId: 'connectBTPopup',

        childViews: [
            'window'
        ],

        window: Em.ContainerView.create({
            classNames: 'popup-window',
            elementId: 'connectBTPopupBTPopupWindow',
            childViews: [
                'label',
                'button'
            ],

            label: MFT.Label.create({
                elementId: 'connectBTPopupBTPopupLabel',
                contentBinding: Ember.Binding.oneWay('MFT.MediaController.popUpMessage')
            }),
            button: MFT.Button.create({
                elementId: 'deviceListBTPopupCancel',
                classNames: ['close_button'],
                target:			   'MFT.MediaController',
                action:			   'cancelConnectBTDevice',
                onDown: false,
                textBinding: Ember.Binding.oneWay('MFT.locale.label.view_media_bt_popap_cancel')
            })

        })
    })
});
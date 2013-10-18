/**
 * @name MFT.AppSettingsConfigureView
 *
 * @desc App Settings Configure module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/appSettings/configureView.js
 * @version        2.0
 *
 * @author        Andrii Velychko
 */

MFT.configureView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'appSettingsConfigure',

    classNames: 'appsettings-block',

    childViews: [
        'backButton',
        'topTitle',
        'sdCardToggle',
        'vehicleType'
    ],

    backButton: MFT.Button.extend({
        elementId: 'appSettingsConfigure-backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId:		   'appsettings-configure-head',
        classNames:		  ['appsettings-head'],
        contentBinding:	 Ember.Binding.oneWay('MFT.locale.label.view_appSettings_configure_head')
    }),

    sdCardToggle: Em.ContainerView.extend({
        elementId: 'appsettings-configure-sdcard',
        classNames: [
            'center-btn',
            'lableToggleButton'
        ],

        childViews: [
            'label',
            'buttons'
        ],

        label: MFT.Label.extend({
            classNames: 'lableToggle',
            contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_appSettings_configure_sdCardNavigation')
        }),

        buttons: Em.ContainerView.extend({
            classNames: ['toogleButtonContainer doubletoggle'],
            childViews: [
                'on',
                'off'
            ],

            on: MFT.Button.extend({
                elementId: 'navsdcard-on-btn',
                classNames: ['button', 'toggles'],
                classNameBindings: ['FFW.Backend.isNavigationEnabled:active'],
                templateName: 'text',
                textBinding: Ember.Binding.oneWay('MFT.locale.label.view_appSettings_configure_sdCardNavigation_On'),
                action: 'navigationEnable',
                target: 'MFT.AppSettingsController'
            }),

            off: MFT.Button.extend({
                elementId: 'navsdcard-off-btn',
                classNameBindings: ['FFW.Backend.isNavigationEnabled::active'],
                classNames: ['button', 'toggles'],
                templateName: 'text',
                textBinding: Ember.Binding.oneWay('MFT.locale.label.view_appSettings_configure_sdCardNavigation_Off'),
                action: 'navigationDisable',
                target: 'MFT.AppSettingsController'
            })
        })
    }),

    vehicleType: Em.ContainerView.create({
        elementId: 'appsettings-configure-vehicles',

        classNames: 'center-btn',

        childViews: [
            'label',
            'vehicleList'
        ],

        label: MFT.Label.extend({
            elementId: 'apps-vehicle-label',
            contentBinding:	 Ember.Binding.oneWay('MFT.locale.label.view_appSettings_configure_chooseVehicleModel')
        }),

        vehicleList: Em.ContainerView.extend({
            elementId: 'apps-vehicle-list',

            childViews: [
                'fiesta',
                'focus',
                'fusion',
                'taurus',
                'escape',
                'edge',
                'explorer',
                'flex',
                'f150'
            ],

            fiesta: MFT.Button.extend({
                value: 'fiesta',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: "Fiesta",
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            }),

            focus: MFT.Button.extend({
                value: 'focus',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: 'Focus',
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            }),

            fusion: MFT.Button.extend({
                value: 'fusion_2013',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: "Fusion",
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            }),

            taurus: MFT.Button.extend({
                value: 'taurus_2013',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: "Taurus",
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController',
                disabled: MFT.SettingsModel.isSpanish
            }),

            escape: MFT.Button.extend({
                value: 'escape_2013',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: "Escape",
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            }),

            edge: MFT.Button.extend({
                value: 'edge',
                classNameBindings: ['active'],
                classNames: ['button'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: 'Edge',
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            }),

            explorer: MFT.Button.extend({
                value: 'explorer',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: 'Explorer',
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            }),

            flex: MFT.Button.extend({
                value: 'flex_2013',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: "Flex",
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController',
                disabled: MFT.SettingsModel.isSpanish
            }),

            f150: MFT.Button.extend({
                value: 'f150_2013',
                classNames: ['button'],
                classNameBindings: ['active'],
                templateName: 'text',
                active: function(){
                    return MFT.AppSettingsController.get('vehicleModelName') === this.value;
                }.property('MFT.AppSettingsController.vehicleModelName'),
                text: MFT.SettingsModel.isSpanish ? "Lobo" : "F150",
                action: 'setActiveVehicleModel',
                target: 'MFT.AppSettingsController'
            })
        })
    })

});
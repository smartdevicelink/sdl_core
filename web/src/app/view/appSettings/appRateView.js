/**
 * @name MFT.AppSettingsAppRateView
 *
 * @desc App Rating module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/appSettings/appRateView.js
 * @version        1.0
 *
 * @author        Sergei Polishchuk
 */

MFT.AppSettingsAppRateView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'appSettingsAppRate',

    classNames: 'appsettings-block',

    childViews: [
        'backButton',
        'topTitle',
        'content',
        'button'
    ],

    backButton: MFT.Button.extend({
        elementId: 'appSettingsAppRate-backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId:		   'appsettings-app-rate-head',
        classNames:		  ['appsettings-head'],
        contentBinding:	 'MFT.locale.label.view_appSettings_appRate_head'
    }),

    content: Em.View.extend({
        classNames: 'message',

        template: Ember.Handlebars.compile(
            '<div class="app_rate_message">'+
                '<p class="text">{{MFT.AppSettingsController.rateMessage}} </p>'+
                '<p class="thanks"> {{MFT.locale.label.view_appSettings_appRate_thanks}} </p>'+
            '</div>'
        )
    }),

    button: MFT.Button.extend({
        elementId: 'rate_now_button',
        templateName: 'text',
        classNames:  'button',
        textBinding: 'MFT.locale.label.view_appSettings_appRate_rateNow',
        action: 'rateApp',
        onDown: true,
        disabled: false,
        target: 'MFT.RateController'
    })

});
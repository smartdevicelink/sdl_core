/**
 * @name MFT.AppSettingsAboutView
 *
 * @desc App Settings About module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/appSettings/aboutView.js
 * @version        2.0
 *
 * @author        Andrii Velychko
 */

MFT.aboutView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'appSettingsAbout',

    classNames: 'appsettings-block',

    childViews: [
        'backButton',
        'topTitle',
        'version',
        'list'
    ],

    backButton: MFT.Button.extend({
        elementId: 'appSettingsAbout-backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId:		   'appsettings-about-head',
        classNames:		  ['appsettings-head'],
        contentBinding:	 'MFT.locale.label.view_appSettings_about_head'
    }),

    version: Em.View.extend({
        elementId: 'apps-version-block',
        classNames: ['center-btn', 'list-item'],
        template: Em.Handlebars.compile(
            '<span>{{MFT.locale.label.view_appSettings_appVersion}}</span>' +
            '<div>{{MFT.locale.label.view_appSettings_version}} 3.6d</div>'
        )
    }),

    list: MFT.List.extend( {

        elementId:	  			  'appSettingsAbout-list',

        classNames: 'center-btn',

        disableScrollbar:	true,

        items:[
            {
                type:		MFT.Button,
                params:		{
                    action:	  'sendSupportEmail',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_appSettings_contactAppSupport',
                    secondText: 'mftguide@ford.com',
                    templateName: 'doubleTextArrow',
                    disabled:	false,
                    target:	  'MFT.AppSettingsController'
                }
            },
            {
                type:		MFT.Button,
                params:		{
                    action:	  'openEULA',
                    className:  'button',
                    textBinding: 'MFT.locale.label.view_appSettings_MFTGuideEULA',
                    templateName: 'arrow',
                    disabled:	false,
                    target:	  'MFT.AppSettingsController'
                }
            }
        ]
    })

});
/**
 * @name MFT.SettingsHelpVoiceCommandListVoiceInstructionsView
 *
 * @desc Settings Help Voice Command List Voice Instructions module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/help/voiceCommandList/voiceInstructionsView.js
 *
 * @author        Sergey Polishchuk
 */

MFT.SettingsHelpVoiceCommandListVoiceInstructionsView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'settings_help_voiceCommandList_voiceInstructions',
    classNames: 'settings-list-block',
    childViews: [
        'backButton',
        'topTitle',
        'list'
    ],

    backButton: MFT.Button.extend({
        elementId: 'settings_help_voiceCommandList_voiceInstructions_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId: 'settings_help_voiceCommandList_voiceInstructions_title',
        classNames: ['block-header-title'],
        contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_title')
    }),

    list: MFT.List.extend(MFT.ListHelpScroll, {

        elementId: 'settings_help_voiceCommandList_voiceInstructions_list',

        classNames: 'settings-list',

        itemsOnPage: 5,

        items: [
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_help',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_line1',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_line2',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_line3',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_line4',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_listCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_mainMenu',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_voiceInstructions',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceInstructions_voiceInstructionsHelp',
                    templateName: 'text',
                    disabled: true
                }
            },
        ]
    })
});
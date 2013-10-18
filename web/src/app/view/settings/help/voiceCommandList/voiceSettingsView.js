/**
 * @name MFT.SettingsHelpVoiceCommandListVoiceSettingsView
 *
 * @desc Settings Help Voice Command List Voice Settings module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/help/voiceCommandList/voiceSettingsView.js
 *
 * @author        Andrii Velychko
 */

MFT.SettingsHelpVoiceCommandListVoiceSettingsView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'settings_help_voiceCommandList_voiceSettings',
    classNames: 'settings-list-block',
    childViews: [
        'backButton',
        'topTitle',
        'list'
    ],

    backButton: MFT.Button.extend({
        elementId: 'settings_help_voiceCommandList_voiceSettings_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId: 'settings_help_voiceCommandList_voiceSettings_title',
        classNames: ['block-header-title'],
        contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_title')
    }),

    list: MFT.List.extend(MFT.ListHelpScroll, {

        elementId: 'settings_help_voiceCommandList_voiceSettings_list',

        classNames: 'settings-list',

        itemsOnPage: 5,

        items: [
            {

                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_confirmationPrompts',
                    templateName: 'text',
                    disabled: true
                }

            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_help',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_interactionModeAdvanced',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_interactionModeNovice',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_listOfCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_mediaCandidate',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_phoneCandidate',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_voiceSettings_voiceSettingsHelp',
                    templateName: 'text',
                    disabled: true
                }
            }

        ]
    })
});
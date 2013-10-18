/**
 * @name MFT.SettingsHelpVoiceCommandListRadioView
 *
 * @desc Settings Help Voice Command List Radio module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/help/voiceCommandList/radioView.js
 *
 * @author        Sergey Polishchuk
 */

MFT.SettingsHelpVoiceCommandListRadioView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'settings_help_voiceCommandList_radio',
    classNames: 'settings-list-block',
    childViews: [
        'backButton',
        'topTitle',
        'list'
    ],

    backButton: MFT.Button.extend({
        elementId: 'settings_help_voiceCommandList_radio_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId: 'settings_help_voiceCommandList_radio_title',
        classNames: ['block-header-title'],
        contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandList_radio_title')
    }),

    list: MFT.List.extend(MFT.ListHelpScroll, {

        elementId: 'settings_help_voiceCommandList_radio_list',

        classNames: 'settings-list',

        itemsOnPage: 5,

        items: [
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AM',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AMFrequency',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AMAutoset',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AMAutosetPresetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AMPresetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AM1',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_AM1PresetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FM',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FM12',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FM12PresetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FMFrequency',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FMFrequencyHD',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FMAutoset',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FMAutosetPresetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_FMPresetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_help',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_listCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_play',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_presetNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_radioHelp',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_radioListCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_radio',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_radio_tune',
                    templateName: 'text',
                    disabled: true
                }
            },
        ]
    })
});
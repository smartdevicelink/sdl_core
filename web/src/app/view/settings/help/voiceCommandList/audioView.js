/**
 * @name MFT.SettingsHelpVoiceCommandListAudioView
 *
 * @desc Settings Help Voice Command List Audio module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/help/voiceCommandList/audioView.js
 *
 * @author        Andrii Velychko
 */

MFT.SettingsHelpVoiceCommandListAudioView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'settings_help_voiceCommandList_audio',
    classNames: 'settings-list-block',
    childViews: [
        'backButton',
        'topTitle',
        'list'
    ],

    backButton: MFT.Button.extend({
        elementId: 'settings_help_voiceCommandList_audio_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId: 'settings_help_voiceCommandList_audio_title',
        classNames: ['block-header-title'],
        contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandList_audio_title')
    }),

    list: MFT.List.extend(MFT.ListHelpScroll, {

        elementId: 'settings_help_voiceCommandList_audio_list',

        classNames: 'settings-list',

        itemsOnPage: 5,

        items: [
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_deviceName',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_AM',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_AMAutostore',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_AM1',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_audio',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_audioHelp',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_audioListCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_audioOnOff',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_bluetoothStereo',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_browse',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_CD',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_FM',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_FM12',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_FMAutostore',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_help',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_AVIn',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_listOfCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_onOff',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_Radio',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_SAT',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_SAT13',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_SDCard',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_Sirius',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_USB',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_audio_whatsPlaying',
                    templateName: 'text',
                    disabled: true
                }
            }

        ]
    })
});
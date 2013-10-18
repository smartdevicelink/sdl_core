/**
 * @name MFT.SettingsHelpVoiceCommandListDiscView
 *
 * @desc Settings Help Voice Command List Disc module visual representation
 *
 * @category    View
 * @filesource    app/view/settings/help/voiceCommandList/discView.js
 *
 * @author        Andrii Velychko
 */

MFT.SettingsHelpVoiceCommandListDiscView = Em.ContainerView.create(MFT.LoadableView, {
    elementId: 'settings_help_voiceCommandList_disc',
    classNames: 'settings-list-block',
    childViews: [
        'backButton',
        'topTitle',
        'list'
    ],

    backButton: MFT.Button.extend({
        elementId: 'settings_help_voiceCommandList_disc_backButton',
        classNames: ['backButton', 'button'],
        action: 'back',
        target: 'MFT.States',
        icon: 'images/media/ico_back.png'
    }),

    topTitle: MFT.Label.extend({
        elementId: 'settings_help_voiceCommandList_disc_title',
        classNames: ['block-header-title'],
        contentBinding: Ember.Binding.oneWay('MFT.locale.label.view_settings_help_voiceCommandList_disc_title')
    }),

    list: MFT.List.extend(MFT.ListHelpScroll, {

        elementId: 'settings_help_voiceCommandList_disc_list',

        classNames: 'settings-list',

        itemsOnPage: 5,

        items: [
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_CD',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_CDHelp',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_CDListCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_CDPlay',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_help',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_listOfCommands',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_pause',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_play',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_playNextTrack',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_playPrevioustrack',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_playTrackNumber',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_repeatFolder',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_repeatOnOff',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_repeatTrack',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_shuffleDiscOn',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_shuffleFolderOn',
                    templateName: 'text',
                    disabled: true
                }
            },
            {
                type: MFT.Button,
                params: {
                    className: 'button',
                    textBinding: 'MFT.locale.label.view_settings_help_voiceCommandList_disc_shuffleOnOff',
                    templateName: 'text',
                    disabled: true
                }
            }
        ]
    })
});
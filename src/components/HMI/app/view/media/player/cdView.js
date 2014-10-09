/**
 * @name SDL.CDView
 *
 * @desc Media CD module visual representation
 *
 * @category    View
 * @source    app/view/media/player/cdView.js
 * @version        1.0
 *
 * @author        Hoang Dinh
 */
SDL.cdView = Em.ContainerView.create({

    elementId: 'media_player_cd_view',

    classNames: 'media_player_view_wrapper',

    classNameBindings: [
        'SDL.States.media.player.cd.active:active_state:inactive_state'
    ],
    /** View components*/
    childViews: [
        'controlls',
        'info',
        'rightmenu'
    ],

    info: Em.View.extend({
        elementId: 'media_player_cd_view_info',

        template: Em.Handlebars.compile(
            '<div class="track-info">' +
                '<div class="total">{{SDL.MediaController.currentSelectedPlayer.currentTrack}}/{{SDL.MediaController.currentSelectedPlayer.totalTracks}}</div>' +
                '<div class="divider_o"></div>' +
                '<div class="title">{{SDL.MediaController.currentSelectedPlayer.data.selectedItem.album}}</div>' +
                '<div class="track-number" >{{SDL.MediaController.currentSelectedPlayer.data.selectedItem.name}}</div>' +
                '<div class="time">{{SDL.MediaController.currentSelectedPlayer.formatTimeToString}}</div>' +
                '<div id="cd_logo"></div>' +
                '</div>'
        )
    }),

    controlls: Em.ContainerView.extend({

        elementId: 'media_player_cd_view_controlls',
        /** View components*/
        childViews: [
            'PrevTrackButton',
            'PlayButton',
            'NextTrackButton'
        ],

        classNames: 'player_controlls',

        PrevTrackButton: SDL.Button.extend({
            elementId: 'media_player_cd_view_controlls_prev_track_button',
            classNames: ['bc-item-big', 'prev-cd'],
            target: 'SDL.MediaController',
            action: 'prevTrack',
            icon: 'images/media/ico_prew.png'
        }),

        PlayButton: SDL.Button.extend({
            elementId: 'media_player_cd_view_controlls_play_button',
            classNames: ['bc-item-big', 'play-cd'],
            target: 'SDL.MediaController',
            action: 'playTrack',
            /** Define button template */
            template: Ember.Handlebars.compile(
                '<img class="playIcon hideicon"{{bindAttr class="SDL.MediaController.currentSelectedPlayer.isPlaying:visible"}} src="images/media/ico_pause.png" />' +
                    '<img class="playIcon showicon"{{bindAttr class="SDL.MediaController.currentSelectedPlayer.isPlaying:not-visible"}} src="images/media/ico-play.png" />'
            )
        }),

        NextTrackButton: SDL.Button.extend({
            elementId: 'media_player_cd_view_controlls_next_track_button',
            classNames: ['bc-item-big', 'next-cd'],
            target: 'SDL.MediaController',
            action: 'nextTrack',
            icon: 'images/media/ico_next.png'
        })
    }),

    rightmenu: Em.ContainerView.create({
        /** View ID */
        elementId: 'media_cd_rightmenu',

        /** Class Names */
        classNames: ['player-right-stock'],

        classNameBindings: [
            'SDL.States.media.player.cd.options.active:hidden',
            'SDL.States.media.player.cd.browse.active:hidden',
            'SDL.States.media.player.cd.moreinfo.active:hidden'
        ],
        /** View Components*/
        childViews: [
            'repeatButton',
            'shuffleButton',
            'scanButton',
            'moreInfoButton',
            'optionsButton',
            'browseButton'
        ],

        repeatButton: SDL.Button.extend({
            elementId: 'media_cd_rightmenu_repeatButton',
            classNames: ['rs-item'],
            classNameBindings: ['SDL.helpMode: hide_icon'],
            icon: 'images/media/passiv_horiz_led.png',
            textBinding: Ember.Binding.oneWay('SDL.locale.label.view_media_repeat'),
            disabled: true
        }),

        shuffleButton: SDL.Button.extend({
            classNameBindings: ['SDL.helpMode:shuffleButton_help'],
            elementId: 'media_cd_rightmenu_shuffleButton',
            classNames: ['rs-item', 'helpmode_box_shadow'],
            icon: 'images/media/passiv_horiz_led.png',
            textBinding: Ember.Binding.oneWay('SDL.locale.label.view_media_shuffle'),
            target: 'SDL.MediaController',
            action: 'turnOnShuffle',
            onDown: false,
            disabledBinding: 'SDL.reversHelpModeBoolean'
        }),

        scanButton: SDL.Button.extend({
            elementId: 'media_cd_rightmenu_scanButton',
            classNameBindings: ['SDL.helpMode:scan_button_help'],
            classNames: ['rs-item', 'helpmode_box_shadow'],
            icon: 'images/media/passiv_horiz_led.png',
            target: 'SDL.MediaController',
            action: 'turnOnScan',
            onDown: false,

            textBinding: 'SDL.locale.label.view_media_scan',

            disabledBinding: 'SDL.reversHelpModeBoolean'

        }),
        moreInfoButton: SDL.Button.extend({
            classNameBindings: ['SDL.helpMode:moreinfoButton_help'],
            elementId: 'media_cd_rightmenu_moreinfoButton',
            action: 'turnOnMoreInfo',
            target: 'SDL.MediaController',
            classNames: ['rs-item', 'helpmode_box_shadow'],
            icon: 'images/media/active_arrow.png',
            textBinding: Ember.Binding.oneWay('SDL.locale.label.view_media_moreInfo'),
            onDown: false
        }),

        optionsButton: SDL.Button.extend({
            elementId: 'media_cd_rightmenu_optionButton',
            classNames: ['rs-item'],
            icon: 'images/media/active_arrow.png',
            textBinding: Ember.Binding.oneWay('SDL.locale.label.view_media_options'),
            action: 'turnOnOptions',
            target: 'SDL.MediaController',
            disabledBinding: 'SDL.helpMode',

            // Define button template
            template: Ember.Handlebars.compile(
                '{{#with view}}' +
                    '<img class="ico" {{bindAttr src="icon"}} />' +
                    '<span>{{text}}</span>' +
                    '{{/with}}'
            )
        }),

        browseButton: SDL.Button.extend({
            classNameBindings: ['SDL.helpMode:browse_button_help'],
            elementId: 'media_cd_rightmenu_browseButton',
            action: 'turnOnBrowse',
            target: 'SDL.MediaController',
            classNames: ['rs-item', 'helpmode_box_shadow'],
            icon: 'images/media/active_arrow.png',
            textBinding: Ember.Binding.oneWay('SDL.locale.label.view_media_browse'),
            onDown: false
        })
    })
});
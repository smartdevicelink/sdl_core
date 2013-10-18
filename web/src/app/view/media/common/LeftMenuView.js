/**
 * @name MFT.LeftMenuView
 *
 * @desc Media Left Menu module visual representation
 *
 * @category	View
 * @filesource	app/view/media/SiriusView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.LeftMenuView = Em.ContainerView.extend({
    /** View ID*/
    elementId: 'media_leftmenu',

    classNameBindings: [
        'isHide:hide-important'
    ],

    isHide: function () {
        return MFT.States.media.radio.am.options.active
            || MFT.States.media.radio.fm.options.active
            || MFT.States.media.radio.sirius.browse.active
            || MFT.States.media.radio.sirius.options.active
            || MFT.States.media.player.cd.options.active
            || MFT.States.media.player.cd.browse.active
            || MFT.States.media.player.cd.moreinfo.active
            || MFT.States.media.player.usb.options.active
            || MFT.States.media.player.usb.browse.active
            || MFT.States.media.player.usb.moreinfo.active
            || MFT.States.media.player.sd.options.active
            || MFT.States.media.player.sd.browse.active
            || MFT.States.media.player.sd.moreinfo.active
            || MFT.States.media.player.bluetooth.options.active
    }.property(
            'MFT.States.media.radio.am.options.active',
            'MFT.States.media.radio.fm.options.active',
            'MFT.States.media.radio.sirius.browse.active',
            'MFT.States.media.radio.sirius.options.active',
            'MFT.States.media.player.cd.options.active',
            'MFT.States.media.player.cd.browse.active',
            'MFT.States.media.player.cd.moreinfo.active',
            'MFT.States.media.player.usb.options.active',
            'MFT.States.media.player.usb.browse.active',
            'MFT.States.media.player.usb.moreinfo.active',
            'MFT.States.media.player.sd.options.active',
            'MFT.States.media.player.sd.browse.active',
            'MFT.States.media.player.sd.moreinfo.active',
            'MFT.States.media.player.bluetooth.options.active'
        ),

    /** View components*/
    childViews: [
        'border',
        'amButton',
        'fmButton',
        'siriusButton',
        'cdButton',
        'usbButton',
        'scrolldownButton',
        'scrollupButton',
        'btButton',
        'sdButton',
        'avinButton'
    ],
    /** Border decoration*/
    border: Em.View.extend({
        elementId: 'media_left_menu_border',
        classNames:['ls_border']
    }),

    /** AM Station Button*/
    amButton:  MFT.ToggleIndicatorsButton.create({
        classNameBindings: ['MFT.States.media.radio.am.active:active_menu', 'MFT.MediaController.isTopListMenu:displayed'],
        elementId:			 'media_amButton',
        classNames:			['media-ls-item'],
        action:				'toggleAmStation',
        icon:				  'images/media/ico_am.png',
        target:				'MFT.MediaController',
        contentBinding:		Ember.Binding.oneWay('MFT.AmModel.band'),
        indActiveClass:		'small_ind_active',
        indDefaultClass:	   'small_ind_def',
        textBinding:		   Ember.Binding.oneWay('MFT.MediaController.amLabel')
    }),
    /** FM Station Button*/
    fmButton:  MFT.ToggleIndicatorsButton.extend({
        classNameBindings: ['MFT.States.media.radio.fm.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed'],
        elementId:			 'media_fmButton',
        classNames:			['media-ls-item'],
        action:				'toggleFmStation',
        icon:				  'images/media/ico_fm.png',
        target:				'MFT.MediaController',
        contentBinding:		 Ember.Binding.oneWay('MFT.FmModel.band'),
        indActiveClass:		'small_ind_active',
        indDefaultClass:	   'small_ind_def',
        textBinding:		    Ember.Binding.oneWay('MFT.MediaController.fmLabel')
    }),
    /** SIRIUS Station Button*/
    siriusButton:  MFT.ToggleIndicatorsButton.extend({
        classNameBindings: ['MFT.States.media.radio.sirius.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed', 'MFT.SettingsModel.isSpanish:hidden'],
        elementId:			 'media_siriusButton',
        classNames:			['media-ls-item'],
        action:				'toggleSiriusStation',
        icon:				  'images/media/ico_sirius.png',
        target:				'MFT.MediaController',
        contentBinding:		 Ember.Binding.oneWay('MFT.SiriusModel.band'),
        indActiveClass:		'small_ind_active',
        indDefaultClass:	   'small_ind_def',
        textBinding:		    Ember.Binding.oneWay('MFT.MediaController.siriusLabel')
    }),
    /** CD Station Button*/
    cdButton:  MFT.Button.extend({
        classNameBindings: ['MFT.States.media.player.cd.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed'],
        elementId:			 'media_cdButton',
        classNames:			['media-ls-item'],
        action:				'turnOnCD',
        icon:				  'images/media/ico_cd.png',
        target:				'MFT.MediaController',
        text:		   MFT.locale.label.view_media_cd
    }),
    /** USB Station Button*/
    usbButton:  MFT.Button.extend({
        classNameBindings: ['MFT.States.media.player.usb.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed'],
        elementId: 			 'media_usbButton',
        classNames:			['media-ls-item'],
        action:				'turnOnUSB',
        icon:				  'images/media/ico_usb.png',
        target:				'MFT.MediaController',
        text:		   MFT.locale.label.view_media_usbHard
    }),
    /** SCROLL DOWN Menu List Button*/
    scrolldownButton:  MFT.Button.extend({
        classNameBindings: ['MFT.MediaController.isTopListMenu:displayed'],
        elementId:			'media_scrolldownButton',
        target:				'MFT.MediaController',
        action:				'listDown',
        classNames:			['media-ls-item'],
        icon:				  'images/media/ico_arrow_down.png'
    }),
    /** SCROLL UP Menu List Button*/
    scrollupButton:  MFT.Button.extend({
        classNameBindings: ['MFT.MediaController.isTopListMenu:displayed'],
        elementId:			 'media_scrollupButton',
        target:				'MFT.MediaController',
        action:				'listUp',
        classNames:			['media-ls-item'],
        icon:				  'images/media/ico_arrow_up.png'
    }),
    /** SD  Button*/
    sdButton:  MFT.Button.extend({
        classNameBindings: ['MFT.States.media.player.sd.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed'],
        elementId:			'media_sdButton',
        classNames:			['media-ls-item'],
        action:				'turnOnSD',
        icon:				  'images/media/ico_sd.png',
        target:				'MFT.MediaController',
        text:		 MFT.locale.label.view_media_usbSD
    }),
    /** BLUETOOTH Button*/
    btButton:  MFT.ToggleIndicatorsButton.create({
        classNameBindings: ['MFT.States.media.player.bluetooth.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed', 'MFT.MediaController.isAllSlotsBTEmpty:ind_hidden'],
        elementId:			'media_btButton',
        classNames:			['media-ls-item','helpmode_box_shadow'],
        action:				'toggleBT',
        contentBinding:		Ember.Binding.oneWay('MFT.BTModel.range'),
        icon:				'images/media/ico_bt.png',
        indActiveClass:		'small_ind_active',
        indDefaultClass:	   'small_ind_def',
        target:				'MFT.MediaController',
        stateName:			'bluetooth',
        textBinding:		Ember.Binding.oneWay('MFT.MediaController.activeSlotBTTitle'),
        onDown: function(){
            if(MFT.helpMode){
                return false;
            }else{
                return true;
            }
        }.property('MFT.helpMode')
    }),
    /** AVIN Button*/
    avinButton:  MFT.Button.extend({
        classNameBindings: ['MFT.States.media.avin.active:active_menu' , 'MFT.MediaController.isTopListMenu:displayed'],
        elementId:			'media_avinButton',
        classNames:			['media-ls-item'],
        action:				'turnOnAVin',
        icon:				'images/media/ico_li.png',
        target:				'MFT.MediaController',
        text:		MFT.locale.label.view_media_avi
    })
});

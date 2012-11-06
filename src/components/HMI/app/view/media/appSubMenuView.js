/**
 * @name MFT.AppOptionsView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/AppOptionsView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.AppSubMenuView = Em.ContainerView.create({
    classNameBindings:  [ 'MFT.States.media.appsubmenu.active:active_state',
                          //  'MFT.MediaController.currentOptionsData.view',
                        ],

    classNames:        ['media_app_sub_menu_view', 'hidden'],

    elementId:          'media_app_sub_menu_view',
    childViews:         [
                            'backButton',
                           // 'buttonsWrapper'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'optionsBack',
        target:            'MFT.MediaController',   
        icon:              'images/media/ico_back.png',   
    })
});
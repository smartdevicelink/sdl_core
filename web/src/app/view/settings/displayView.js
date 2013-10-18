/**
 * @name MFT.SettingsDisplayView
 * 
 * @desc Settings Display module visual representation
 * 
 * @category	View
 * @filesource	app/view/settings/SettingsDisplayView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsDisplayView = Em.ContainerView.create(MFT.LoadableView,{
	/** Bind class for visual representation */
	
	elementId:	'settings_display',

    childViews: [
        'listDisplaySettings'
    ],

    listDisplaySettings: MFT.List.extend({
        elementId:		   'settings_display_list',

        disableScrollbar:	true,

        /** Items */
        items: [
            {
                type:		MFT.PlusMinusRangeBar,
                params:		{
                    labelContentBinding:    'MFT.locale.label.view_settings_display_brightness',
                    actionInc:			    'brightIncrease',
                    actionDec:		        'brightDecrease',
                    target:				    'MFT.SettingsController',
                    contentBinding:		    'MFT.SettingsModel.brightness',
                    indActiveClass:		    'ind_act',
                    indDefaultClass:	    'ind_inact',
                    incIcon:                'images/common/plus-ico.png',
                    decIcon:                'images/common/minus-ico.png',
                    range:                  24,
                    valueBinding:           'MFT.SettingsModel.brightness.value',
                    classNameBindings:      'MFT.SettingsController.isAutoDim:hidden'
                }

            },
            {
                type:		MFT.LabelPlusToggleButton,
                params:		{
                    labelContentBinding:    'MFT.locale.label.view_settings_display_auto_dim',
                    tButtonValueBinding:    'MFT.SettingsModel.autoDim',
                    tButtonRange:		    2,
                    tButtonLabelsBinding:   'MFT.locale.label.view_settings_display_OnOFF',
                    labelDisabled: 		    false,
                    tButtonDisabled:	    false,
                    classContainer:	        ['toogleButtonContainer doubletoggle']
                }
            },
            {
                type:		MFT.LabelPlusToggleButton,
                params:		{
                    labelContentBinding:  'MFT.locale.label.view_settings_display_mode',
                    tButtonValue:		 0,
                    tButtonRange:		 4,
                    tButtonLabelsBinding: 'MFT.locale.label.view_settings_display_modeType',
                    labelDisabled: 		false,
                    tButtonDisabled:	  false,
                    classContainer:	   ['toogleButtonContainer']
                }
            },
            {
                type:		MFT.BalanceRangeBar,
                params:		{
                    labelContentBinding:    'MFT.locale.label.view_settings_display_auto_offset',
                    actionInc:			    'autoDimOffsetIncrease',
                    actionDec:		        'autoDimOffsetDecrease',
                    target:				    'MFT.SettingsController',
                    contentBinding:		    'MFT.SettingsModel.autoDimOffset',
                    indActiveClass:		    'ind_act',
                    indDefaultClass:	    'ind_inact',
                    incIcon:                'images/common/plus-ico.png',
                    decIcon:                'images/common/minus-ico.png',
                    range:                  6,
                    valueBinding:           'MFT.SettingsModel.autoDimOffset.value',
                    classNameBindings:      'MFT.SettingsController.isAutoDim::hidden'
                }

            },
            {
                type:		MFT.Button,
                params:		{
                    textBinding: 'MFT.locale.label.view_settings_display_edit_wallpaper',
                    templateName: 'arrow',
                    disabled: true
                }
            }
        ],

        itemsOnPage:	6
    })
});

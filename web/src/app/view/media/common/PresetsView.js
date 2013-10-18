/**
 * @name MFT.PresetView
 * 
 * @desc Media component for radio presets
 * 
 * @category	view
 * @filesource	app/view/media/PresetsView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.PresetsView = Em.ContainerView.create({
	
	elementId: 'media_presetButtons',

    classNameBindings: [
        'isHide:hide-important'
    ],
	
	classNames: ['main-preset-buttons-wraper'],
	
	childViews: ['wrapper', 'helpModePresetsLayer'],

    /** Hide media_presetButtons when it is not needed */
    isHide: function(){
        return MFT.States.media.radio.am.options.active || MFT.States.media.radio.fm.options.active || MFT.States.media.radio.sirius.browse.active || MFT.States.media.radio.sirius.options.active || MFT.States.media.player.active || MFT.States.media.avin.active;
    }.property('MFT.States.media.radio.am.options.active',
            'MFT.States.media.radio.fm.options.active',
            'MFT.States.media.radio.sirius.browse.active',
            'MFT.States.media.radio.sirius.options.active',
            'MFT.States.media.player.active',
            'MFT.States.media.avin.active'
        ),

    wrapper:Em.ContainerView.extend({
		elementId: 'media_presetButtons_wrapper',
		
		childViews: ['1','2','3','4','5','6'],
		
		presetDataBinding:  Ember.Binding.oneWay('MFT.MediaController.currentModuleData'),
		
		classNames: ['preset-items'],
		
		'1': MFT.PresetButton.extend({
			elementId: 'media_preset_button1',
			classNames: 'a0',
			contentBinding:  Ember.Binding.oneWay( 'parentView.presetData.items.0'),
			num: 1,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData')
		}),
		'2': MFT.PresetButton.extend({
			elementId: 'media_preset_button2',
			classNames: 'a1',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.1'),
			num: 2,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData')
			
		}),
		'3': MFT.PresetButton.extend({
			elementId: 'media_preset_button3',
			classNames: 'a2',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.2'),
			num: 3,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData')
			
		}),
		'4': MFT.PresetButton.extend({
			elementId: 'media_preset_button4',
			classNames: 'a3',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.3'),
			num: 4,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData')
			
		}),
		'5': MFT.PresetButton.extend({
			elementId: 'media_preset_button5',
			classNames: 'a4',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.4'),
			num: 5,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData')
		}),
		'6': MFT.PresetButton.extend({
			elementId: 'media_preset_button6',
			classNames: 'a5',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.5'),
			num: 6,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData')
			
		})
	}),

    helpModePresetsLayer: Em.ContainerView.extend({
        elementId: 'help-mode-presets-layer',
        classNames: ['helpmode_box_shadow', 'shadow-presets'],
        actionUp: function() {
            MFT.VideoPlayerController.start('ent_Presets');
        }
    })
});
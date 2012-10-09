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
	
	classNameBindings: 	['MFT.States.media.Fm.active:active_state' , 'MFT.States.media.Am.active:active_state','MFT.States.media.Sirius.active:active_state'],
	
	classNames: ['main-preset-buttons-wraper', 'hidden'],
	
	childViews: ['wrapper', 'preset_help_yellow_border'],
	
	wrapper:Em.ContainerView.extend({
		elementId: 'media_presetButtons_wrapper',
		
		childViews: ['1','2','3','4','5','6'],
		
		presetDataBinding:  Ember.Binding.oneWay('MFT.MediaController.currentModuleData'),
		
		classNames: 'preset-items',
		
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
	
	preset_help_yellow_border: MFT.Button.extend({
		elementId: 'media_preset_help_yellow_border',
		classNameBindings:['MFT.helpMode:active_state'],
		classNames: ['hidden'],
		target: 'MFT.MediaController',
		action: 'turnPresetHelpVideoOn',
		onDown:				false
	})
});
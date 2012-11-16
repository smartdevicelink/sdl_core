/**
 * @name MFT.HomePresetView
 * 
 * @desc Media component for radio presets
 * 
 * @category	view
 * @filesource	app/view/media/common/HomePresetView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.HomePresetView = Em.ContainerView.extend({
	
	elementId: 'media_home_presetButtons',
	
	classNames: ['home-preset-buttons-wraper'],
	
	childViews: ['wrapper'],
	
	wrapper:Em.ContainerView.extend({

		elementId: 'media_home_presetButtons_wrapper',
		
		childViews: ['1','2','3','4','5','6'],
		
		presetDataBinding:  Ember.Binding.oneWay('MFT.MediaController.currentModuleData'),
		
		classNames: ['preset-items','helpmode_box_shadow'],
		
		'1': MFT.PresetButton.extend({
			elementId: 'media_home_preset_button1',
			classNames: 'a0',
			contentBinding:  Ember.Binding.oneWay( 'parentView.presetData.items.0'),
			num: 1,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData'),
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<p>{{view.frequency}}</p>'
			)
		}),
		'2': MFT.PresetButton.extend({
			elementId: 'media_home_preset_button2',
			classNames: 'a1',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.1'),
			num: 2,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData'),
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<p>{{view.frequency}}</p>'
			)
		}),
		'3': MFT.PresetButton.extend({
			elementId: 'media_home_preset_button3',
			classNames: 'a2',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.2'),
			num: 3,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData'),
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<p>{{view.frequency}}</p>'
			)
		}),
		'4': MFT.PresetButton.extend({
			elementId: 'media_home_preset_button4',
			classNames: 'a3',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.3'),
			num: 4,
			timer: 2000,
			dataBinding: Ember.Binding.oneWay( 'parentView.presetData'),
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<p>{{view.frequency}}</p>'
			)
		}),
		'5': MFT.PresetButton.extend({
			elementId: 'media_home_preset_button5',
			classNames: 'a4',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.4'),
			num: 5,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData'),
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<p>{{view.frequency}}</p>'
			)
		}),
		'6': MFT.PresetButton.extend({
			elementId: 'media_home_preset_button6',
			classNames: 'a5',
			contentBinding:   Ember.Binding.oneWay('parentView.presetData.items.5'),
			num: 6,
			timer: 2000,
			dataBinding:  Ember.Binding.oneWay('parentView.presetData'),
			/** Define button template */
			template: Ember.Handlebars.compile(
				'<p>{{view.frequency}}</p>'
			)
			
		})
	})
});
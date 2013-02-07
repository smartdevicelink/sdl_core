/**
 * @name MFT.InfoNonMedia
 * 
 * @desc NonMedia application visual representation
 * 
 * @category	View
 * @filesource	app/view/info/nonMedia.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.InfoNonMedia = Em.ContainerView.create( MFT.LoadableView, {
	
	elementId: 'info_nonMedia',
	
	controller: MFT.ApplinkAppController,
	
	childViews: [
		'content'
	],

	content: Em.ContainerView.extend({
		classNames: ['inner_content'],
		
		childViews: [
			'deviceName',
			'image',
			'fields',
			'innerMenu',
			'presets'
		],

	    /**
	     * Deactivate View
	     */
	    deactivate: function() {
	        MFT.States.goToState('info.apps');
	    },

		deviceName: MFT.Label.extend({
		
			classNames: ['app_title'],
			
			contentBinding: 'MFT.ApplinkAppController.model.deviceName'
		}),

		image: Em.View.extend({
			
			classNames: ['app_image'],
			
			template: Ember.Handlebars.compile(
				'<img width="150px" {{bindAttr src="MFT.ApplinkAppController.model.appInfo.trackIcon"}}>'
			)
		}),
		
		fields: Em.ContainerView.extend({
			
			classNames: ['app_fields'],
			
			childViews: [
				'field1',
				'field2',
				'field3',
				'field4'
			],
			
			field1: MFT.Label.extend({
				contentBinding: 'controller.model.appInfo.field1'
			}),
			
			field2: MFT.Label.extend({
				contentBinding: 'controller.model.appInfo.field2'
			}),
			
			field3: MFT.Label.extend({
				contentBinding: 'controller.model.appInfo.field3'
			}),
			
			field4: MFT.Label.extend({
				contentBinding: 'controller.model.appInfo.field4'
			})
		
		}),
		
		innerMenu: MFT.MenuList.extend({
			
			refreshItems: function() {
                if ( MFT.ApplinkAppController.model.appId == MFT.NonMediaController.currentAppId ) {
                    this.addItems(MFT.ApplinkAppController.model.softButtons);
                }                    
            }.observes('MFT.ApplinkAppController.model.softButtons.@each'),
			
			content: Em.ContainerView.extend({
				
				classNames: ['content'],
				
				attributeBindings: ['parentView.contentPositon:style'],
				
				childViews: [
					'optionsButton'
				],
				
				optionsButton: MFT.Button.extend({
					text: 'Options',
					
					templateName: 'arrow',
					
					action:		'openCommandsList',
					target:		'MFT.ApplinkAppController'
				})
			})
		}),
		
		presets: Em.ContainerView.extend({
            classNames: ['presets'],
            
            classNameBindings: ['hidden'],
            
            hidden: function() {
            
                return !Boolean(MFT.ApplinkAppController.model.get('appInfo.customPresets').length);
            
            }.property('MFT.ApplinkAppController.model.appInfo.customPresets.@each'),
            
            childViews: [
    			'perset1',
    			'perset2',
    			'perset3',
    			'perset4',
    			'perset5',
    			'perset6'
            ],
            
            perset1: MFT.ApplinkPresetButton.extend({textBinding: 'MFT.ApplinkAppController.model.appInfo.customPresets.0', presetName: 'PRESET_1'}),
            perset2: MFT.ApplinkPresetButton.extend({textBinding: 'MFT.ApplinkAppController.model.appInfo.customPresets.1', presetName: 'PRESET_2'}),
            perset3: MFT.ApplinkPresetButton.extend({textBinding: 'MFT.ApplinkAppController.model.appInfo.customPresets.2', presetName: 'PRESET_3'}),
            perset4: MFT.ApplinkPresetButton.extend({textBinding: 'MFT.ApplinkAppController.model.appInfo.customPresets.3', presetName: 'PRESET_4'}),
            perset5: MFT.ApplinkPresetButton.extend({textBinding: 'MFT.ApplinkAppController.model.appInfo.customPresets.4', presetName: 'PRESET_5'}),
            perset6: MFT.ApplinkPresetButton.extend({textBinding: 'MFT.ApplinkAppController.model.appInfo.customPresets.5', presetName: 'PRESET_6'})
		})
	})
});
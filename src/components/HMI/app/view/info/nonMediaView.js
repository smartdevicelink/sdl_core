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
			'persets'
		],
		
		deviceName: MFT.Label.extend({
		
			classNames: ['app_title'],
			
			contentBinding: 'MFT.ApplinkModel.deviceName'
		}),
		
		image: Em.View.extend({
			
			classNames: ['app_image'],
			
			template: Ember.Handlebars.compile(
				'<img width="150px" src="images/media/mi-image.png">'
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
					'optionsButton',
					'routeInfoButton'
				],
				
				optionsButton: MFT.Button.extend({
					text: 'Options',
					
					templateName: 'arrow',
					
					action:		'openCommandsList',
					target:		'MFT.ApplinkAppController'
				}),
				
				routeInfoButton: MFT.Button.extend({
					text: 'Route Info',
					
					//hidden:	true,
					
					templateName: 'arrow'
				})
			})
		}),
		
		persets: Em.ContainerView.extend({
            classNames: ['persets'],
            
            childViews: [
    			'perset1',
    			'perset2',
    			'perset3',
    			'perset4',
    			'perset5',
    			'perset6'
            ],
            
            perset1: MFT.Button.extend({text:'Perset 1'}),
            perset2: MFT.Button.extend({text:'Perset 2'}),
            perset3: MFT.Button.extend({text:'Perset 3'}),
            perset4: MFT.Button.extend({text:'Perset 4'}),
            perset5: MFT.Button.extend({text:'Perset 5'}),
            perset6: MFT.Button.extend({text:'Perset 6'})
		})
	})
});
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
	
	controller: MFT.NonMediaController,
	
	childViews: [
		'content'
	],
	
	content: Em.ContainerView.extend({
		classNames: ['inner_content'],
		
		childViews: [
			'deviceName',
			'image',
			'fields',
			'innerMenu'
		],
		
		deviceName: MFT.Label.extend({
		
			classNames: ['app_title'],
			
			contentBinding: 'controller.model.deviceName'
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
				content: 'Field 1'
			}),
			
			field2: MFT.Label.extend({
				content: 'Field 2'
			}),
			
			field3: MFT.Label.extend({
				content: 'Field 3'
			}),
			
			field4: MFT.Label.extend({
				content: 'Field 4'
			})
		
		}),
		
		innerMenu: MFT.MenuList.extend({
						
			content: Em.ContainerView.extend({
				
				classNames: ['content'],
				
				attributeBindings: ['parentView.contentPositon:style'],
				
				childViews: [
					'optionsButton',
					'routeInfoButton',
					'softButton1',
					'softButton2',
					'softButton3',
					'softButton4',
					'softButton5'
				],
				
				optionsButton: MFT.Button.extend({
					text: 'Options',
					
					templateName: 'arrow',
					
					goToState:	'options',
					action:		'onChildState',
					target:		'MFT.InfoController'
				}),
				
				routeInfoButton: MFT.Button.extend({
					text: 'Route Info',
					
					//hidden:	true,
					
					templateName: 'arrow'
				}),
				
				softButton1: MFT.Button.extend({
					text: 'Soft Button 1'
				}),
				
				softButton2: MFT.Button.extend({
					text: 'Soft Button 2'
				}),
				
				softButton3: MFT.Button.extend({
					text: 'Soft Button 3'
				}),
				
				softButton4: MFT.Button.extend({
					text: 'Soft Button 4'
				}),
				
				softButton5: MFT.Button.extend({
					text: 'Soft Button 5'
				})
			})
		})
	})
});
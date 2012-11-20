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
		
		innerMenu: Em.ContainerView.extend({
			
			classNames: ['ffw_list_menu'],
			
			attributeBindings: ['elementHeight:style'],
			
			// Position of current page
			page:	0,
			
			// Items per one page
			itemsOnPage: 5,
			
			// Height of one item
			ITEM_HEIGHT: 50,
			
			pageHeight: function() {
				return this.itemsOnPage*this.ITEM_HEIGHT;
			}.property('this.itemsOnPage'),
			
			elementHeight: function() {
				return 'height:' + String( this.get('pageHeight') + this.ITEM_HEIGHT - 2 ) + 'px;';
			}.property('this.pageHeight'),
			
			// Position of content block
			contentPositon: function() {
				return 'top:' + String( -( this.get('page') * this.get('pageHeight') ) ) + 
				'px; height:' + String( this.get('pageHeight') - (Boolean(this.get('onLastPage') && this.get('page')) * this.ITEM_HEIGHT) ) + 'px;';
			}.property('onLastPage'),
			
			// Property for show or hide 'down' button
			onLastPage: function() {
				return ( ( this.get('page')  * this.itemsOnPage ) < ( this.get('content.childViews.length') - this.itemsOnPage ) );
			}.property(
				'page',
				'content.childViews.length'
			),
			
			
			// Handeler to go previous page
			pageUp: function() {
				if ( this.get('page') ) {
					this.set( 'page', ( this.get('page') - 1 ) );
				}
			},
			
			// Handeler to go next page
			pageDown: function() {
				if ( this.get('onLastPage') ) {
					this.set( 'page', ( this.get('page') + 1) );
				}				
			},
			
			childViews: [
				'upButton',
				'content',
				'downButton'
			],
			
			upButton: MFT.Button.extend({
				
				classNames:	['control','up_button'],
				
				hidden: function() {
					if ( this.get('parentView.page') ) {
						return false;
					} else {
						return true;
					}
				}.property('parentView.page'),
				
				
				templateName: 'icon',
				
				icon:	'images/media/ico_arrow_up.png',
				
				action:	'pageUp',
				
				target:	'parentView'				
			}),
			
			downButton: MFT.Button.extend({
				
				classNames:	['control','down_button'],
				
				hidden: function() {
					if ( this.get('parentView.onLastPage') ) {
						return false;
					} else {
						return true;
					}
				}.property('parentView.onLastPage'),
				
				templateName: 'icon',
				
				icon:	'images/media/ico_arrow_down.png',
				
				action:	'pageDown',
				
				target:	'parentView'				
			}),
			
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
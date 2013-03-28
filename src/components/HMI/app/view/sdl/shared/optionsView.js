/**
 * @name MFT.SliderView
 * 
 * @desc Options visual representation
 * 
 * @category	View
 * @filesource	app/view/sdl/shared/optionsView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.OptionsView = MFT.SDLAbstractView.create({
	
	elementId: 'sdl_options',
	
	childViews: [
		'backButton',
		'captionText',
		'commands'
	],


	// Menu caption text
    captionBinding: 'MFT.SDLAppController.model.currentSubMenuLabel',
	
	commands: MFT.List.extend({
		
		elementId:		'info_nonMedia_options_list',
				
		itemsOnPage:    5,
		
		items: [],
		
		/*
		itemsDefault: [
			{
				type:	MFT.Button,
			
				params: {
					templateName:	'text',
					text:			'Exit',
					target:		'this.parentView.parentView.parentView',
		            action:		'deactivate',
		            onDown:     false
				}
			},
			{
				type:	MFT.Button,
			
				params: {
					templateName:	'arrow',
					text:			'Device Information',
				}
			}
		],
		*/
		
		refreshItems: function() {
            var commands = MFT.SDLAppController.model.get('currentCommandsList'),
                i, len;
            
            this.items = [];
            
            len = commands.length;
            
            for ( i=0; i<len; i++ ) {
                this.items.push({
                    type: MFT.Button,
                    params: {
    					templateName:  commands[i].menuId ? 'arrow' : '',
    					text:          commands[i].name,
    					commandId:     commands[i].commandId,
    					menuId:        commands[i].menuId,
    					icon:          commands[i].icon,
    					target:		   'MFT.SDLAppController',
    		            action:        'onCommand',
    		            onDown:        false
				    }
                })
            }
                        
            this.list.refresh();
            
                
		}.observes('MFT.SDLAppController.model.currentCommandsList')
	}),

    activate: function( text ) {
       this._super();

        MFT.SDLController.onSystemContextChange();
    },
	
    // Extend deactivate window
    deactivate: function() {
        
        if ( MFT.SDLAppController.model ) {
                    
            if ( MFT.SDLAppController.model.get('currentSubMenuId') ) {
                MFT.SDLAppController.onSubMenu(0);
            } else {
                this._super();
        		MFT.SDLController.onSystemContextChange();
            }
        }

    }
});

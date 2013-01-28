/**
 * @name MFT.SliderView
 * 
 * @desc Options visual representation
 * 
 * @category	View
 * @filesource	app/view/applink/shared/optionsView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.OptionsView = MFT.ApplinkAbstractView.create({
	
	elementId: 'applink_options',
	
	childViews: [
		'backButton',
		'captionText',
		'commands'
	],
	
	commands: MFT.ApplinkCommandsList.extend({
		
		elementId:		'info_nonMedia_options_list',
				
		itemsOnPage:    5,
		
		items: [],
		
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
		
		refreshItems: function() {
            var commands = MFT.NonMediaController.model.get('commandsList'),
                i, len;
            
            
            this.items = [];//this.itemsDefault;
            
            len = commands.length;
            
            for ( i=0; i<len; i++ ) {
                console.log(commands[i]);
                this.items.push({
                    type: MFT.Button,
                    params: {
    					//templateName:	'text',
    					text:			commands[i].name,
    					//target:		'this.parentView.parentView.parentView',
    		            //action:		'deactivate',
    		            onDown:     false
				    }
                })
            }
                        
            this.list.refresh();
            
                
		}.observes('MFT.NonMediaController.model.commandsList.@each')
	}),
	
	init: function() {
	   this._super();
	   
	   this.set('captionText.content','Options');
	}
});

/**
 * @name MFT.SDLAbstractView
 * 
 * @desc SDL application abstract shared view
 * 
 * @category	View
 * @filesource	app/view/sdl/shared/AbstractView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.SDLAbstractView = Em.ContainerView.extend({
	
	
	classNames: [
		'sdl-window'
	],
	
	classNameBindings: [
		'active:active_state:inactive_state'
	],
	
	active: false,
	
	caption: 'Caption Text',
    
    /**
     * Activate window
     * and set caption text
     *
     * @param text: String
     */
    activate: function( text ) {
    	if (text) {
    		this.set('caption',text);
    	}
    	this.set('active',true);
    },
    
    /**
     * Deactivate window
     */
    deactivate: function() {
    	this.set('active',false);
    },
    
    onStateChange: function() {
        if ( this.active ) {
            this.deactivate();
        }
    }.observes('MFT.TransitionIterator.ready'),
    
	backButton: MFT.Button.extend({
		classNames:	[
			'back-button'
		],
		target:		'this.parentView',
		action:		'deactivate',
		icon:		'images/media/ico_back.png',
		onDown:     false
	}),
	
	captionText: MFT.Label.extend({
        classNames:	[
        	'caption-text'
        ],
        
        contentBinding:	'this.parentView.caption'
    })
});
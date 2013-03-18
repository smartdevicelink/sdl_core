/**
 * @name MFT.sdlView
 * 
 * @desc SDL Media application module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/sdlView.js
 * @version		2.0
 *
 * @author		Melnik Andriy
 */
MFT.sdlView = Em.ContainerView.create(MFT.LoadableView,{
	
	/**
	 * View Id
	 */	
	elementId: 'sdl_view_container',

	/**
	 * View Components
	 */
	childViews: [
		'innerMenu',
		'controlls'
	],
	
	controlls: MFT.SDLMediaControlls,

    /**
     * Deactivate View
     */
    deactivate: function() {
        MFT.States.goToState('info.apps');
    },

	/**
	 * Function to choose appropriate view
	 * to current version of protocol
	 */
	changeControlls: function(){
		if(MFT.SDLController.protocolVersion2State){
			this.get('childViews').removeObjects(
                this.get('childViews').filterProperty( 'sdlMediaControlls' , 'V1' )
            );
			MFT.sdlView.get('childViews').pushObject(MFT.SDLMediaControlls);
		}else{
			this.get('childViews').removeObjects(
                this.get('childViews').filterProperty( 'sdlMediaControlls' , 'V2' )
            );
			MFT.sdlView.get('childViews').pushObject(MFT.SDLMediaControllsV1);
		}
	},//.observes('MFT.SDLController.protocolVersion2State'),

	innerMenu: MFT.MenuList.extend({
			
		refreshItems: function() {
            if ( MFT.SDLAppController.model.appId == MFT.SDLMediaController.currentAppId ) {
                this.addItems( MFT.SDLAppController.model.softButtons, MFT.SDLAppController.model.appId );
            }                    
        }.observes('MFT.SDLAppController.model.softButtons.@each'),

        groupName:      "sdl_view_container",
		
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
				target:		'MFT.SDLAppController'
			})
		})
	})

	/** Calls SDL SystemContext switcher when turn On/Of SDL application */
	/*onTurnOnSDLApp: function(systemContextValue){
		if(MFT.States.media.sdl.active){
            MFT.SDLMediaController.onSystemContextSwitcher(MFT.SDLMediaController.eSystemContext.application);
		}else{
            MFT.SDLMediaController.onSystemContextSwitcher(MFT.SDLMediaController.eSystemContext.main);
		}
	}.observes('MFT.States.media.sdl.active')*/
});
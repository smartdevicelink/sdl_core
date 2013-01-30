/**
 * @name MFT.applinkView
 * 
 * @desc Applink Media application module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/applinkView.js
 * @version		2.0
 *
 * @author		Melnik Andriy
 */
MFT.applinkView = Em.ContainerView.create(MFT.LoadableView,{
	
	/**
	 * View Id
	 */	
	elementId: 'applink_view_container',

	/**
	 * View Components
	 */
	childViews: [
		'innerMenu',
		'controlls'
	],
	
	controlls: MFT.ApplinkMediaControllsV1,

	/**
	 * Function to choose appropriate view
	 * to current version of protocol
	 */
	changeControlls: function(){
		if(MFT.ApplinkController.protocolVersion2State){
			this.get('childViews').removeObjects(
                this.get('childViews').filterProperty( 'applinkMediaControlls' , 'V1' )
            );
			MFT.applinkView.get('childViews').pushObject(MFT.ApplinkMediaControllsV2);
		}else{
			this.get('childViews').removeObjects(
                this.get('childViews').filterProperty( 'applinkMediaControlls' , 'V2' )
            );
			MFT.applinkView.get('childViews').pushObject(MFT.ApplinkMediaControllsV1);
		}
	}.observes('MFT.ApplinkController.protocolVersion2State'),

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
	})

	/** Calls Applink SystemContext switcher when turn On/Of Applink application */
	/*onTurnOnApplinkApp: function(systemContextValue){
		if(MFT.States.media.applink.active){
            MFT.ApplinkMediaController.onSystemContextSwitcher(MFT.ApplinkMediaController.eSystemContext.application);
		}else{
            MFT.ApplinkMediaController.onSystemContextSwitcher(MFT.ApplinkMediaController.eSystemContext.main);
		}
	}.observes('MFT.States.media.applink.active')*/
});
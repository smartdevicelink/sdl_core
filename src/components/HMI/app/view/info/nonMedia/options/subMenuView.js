/**
 * @name MFT.InfoNonMediaOptionsSubMenu
 * 
 * @desc NonMedia application visual representation
 * 
 * @category	View
 * @filesource	app/view/info/nonMedia/options/subMenuView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.InfoNonMediaOptionsSubMenu = Em.ContainerView.create( MFT.LoadableView, {
	
	elementId: 'info_nonMedia_options_submenu',
	
	childViews: [
		'backButton',
		'title',
		'listOfSubmenuButtons'
	],
	
	backButton: MFT.Button.extend({		
		classNames:	'backButton',
		
		icon:		'images/common/ico_back.png',
		
		target:		'MFT.States',
		action:		'back'
	}),
	
	title: MFT.Label.extend({
		classNames:	'options_title',
		
		content:	'Submenu'
	}),


    /** Applink Sub Mennu activate handler */
    applinkSubMenuActivate: function(){
        if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
            this.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
        }else{
            this.set('currentApplinkSubMenuid', null);
        }
    }.observes('MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active'),

    SubMenuActivate: function( ){

        var count = this.listOfSubmenuButtons.items.length;
        if(count > 0){
            for(var i = count-1; i >= 0; i--){
                this.listOfSubmenuButtons.deleteItem(i);
            }
        }

        this.listOfSubmenuButtons.list.refresh();

        count = MFT.ApplinkNonMediaModel.subMenuCommands.length;
        for(var i = 0; i < count; i++){
            if( MFT.ApplinkMediaController.currentApplinkSubMenuid == MFT.ApplinkNonMediaModel.subMenuCommands[i].menuParams.parentID ){

                this.listOfSubmenuButtons.items.push({
                    type:       MFT.Button,
                    params:     {
                        action:                 'onCommand',
                        target:                 'MFT.ApplinkMediaController',
                        commandId:              MFT.ApplinkNonMediaModel.subMenuCommands[i].cmdId,
                        text:                   MFT.ApplinkNonMediaModel.subMenuCommands[i].menuParams.menuName,
                        parentID:               MFT.ApplinkNonMediaModel.subMenuCommands[i].menuParams.parentID,
                        className:              'rs-item',
                        templateName:           'text'
                    }                                   
                });
            }
        }

        this.listOfSubmenuButtons.list.refresh();

    },
	
	listOfSubmenuButtons: MFT.List.extend({
		
		elementId:	'info_nonMedia_options_submenu_list',
		
		itemsOnPage:    5,
		
		items: [
			{
				type:	MFT.Button,
			
				params:		{
					templateName:	'text',
				
					text:	'Test'
				}
			}	
		]
	})
});
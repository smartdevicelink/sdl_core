/**
 * @name MFT.USBModel
 * 
 * @desc USB Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/USBModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.USBModel = Em.Object.create({
	
	active: false,
	
	/** USB Player*/
	init: function(){
		this._super();
		this.set('player',MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
	PlayList: MFT.Playlist.create({
		selectedIndex: 					0,
			items: {
				0:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track1 ),
				1:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track2 ),
				2:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track3 ),
				3:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track4 ),
				4:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track5 ),
				5:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track6 )
			},
			
			homeWidgetIcon: 'images/media/usb-h-ico.png',
			homeWidgetIcon_blur: 'images/media/help/usb-h-ico_blur.png'
			
	}),
		
	browseData : Em.Object.create({
		titleBinding: 'MFT.locale.label.view_media_usb_browse_USBHardDisk',
		
		disableScrollbar: true,
		
		items:	[
			{
				type:MFT.Button,
				params:{
					textBinding: 'MFT.locale.label.view_media_usb_browse_music',
					disabled:	true
				}
			},
			{
				type:MFT.Button,
				params:{
					textBinding: 'MFT.locale.label.view_media_usb_browse_exploreDevice',
					disabled:	true
				}
			}
		]
	}),
	
	optionsData: Em.Object.create({
				
		view: 'usb',
		
		titleBinding: 'MFT.locale.label.view_media_usb_options_title',
		
		items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_options_soundSettings',
						templateName:		  'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_options_mediaPlayerSettings',
						templateName:		  'arrow',
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_options_compression_deviceInformation',
						templateName:		  'arrow',
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_usb_options_compression_updateIndex',
						buttonTexBindingt:	'MFT.locale.label.view_media_usb_options_compression_start',
						labelDisabled:		true,
						buttonDisabled: 	   true
					}							
				}
					
			]
		})
});
/**
 * @name MFT.SDModel
 * 
 * @desc SD Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/SDModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.SDModel= Em.Object.create({
	
	active: false,
	
	/** SD Player*/
	init: function(){
		this._super();
		this.set('player',MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
PlayList: MFT.Playlist.create({
	
	selectedIndex: 					4,
	
	items: {
		0:MFT.PlaylistItem.create( MFT.locale.label.view_media_sd_playList_track1 ),
		1:MFT.PlaylistItem.create( MFT.locale.label.view_media_sd_playList_track2 ),
		2:MFT.PlaylistItem.create( MFT.locale.label.view_media_sd_playList_track3 ),
		3:MFT.PlaylistItem.create( MFT.locale.label.view_media_sd_playList_track4 ),
		4:MFT.PlaylistItem.create( MFT.locale.label.view_media_sd_playList_track5 ),
		5:MFT.PlaylistItem.create( MFT.locale.label.view_media_sd_playList_track6 )
	},
	
	homeWidgetIcon: 'images/media/sd-h-ico.png',
	homeWidgetIcon_blur: 'images/media/help/sd-h-ico_blur.png'
}),

browseData : Em.Object.create({
		
		titleBinding:  'MFT.locale.label.view_media_sd_browse_title',
		
		disableScrollbar: true,
		
		items:	[
			{
				type:MFT.Button,
				params:{
					textBinding:  'MFT.locale.label.view_media_sd_browse_music',
					disabled:	true
				}
			},
			{
				type:MFT.Button,
				params:{
					textBinding:  'MFT.locale.label.view_media_sd_browse_exploreDevice',
					disabled:	true
				}
			}
		]
	}),
	
	optionsData: Em.Object.create({
				
		view: 'sd',
		
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
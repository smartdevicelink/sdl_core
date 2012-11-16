/**
 * @name MFT.CDModel
 * 
 * @desc CD Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/CDModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.CDModel= Em.Object.create({
	
	active: false,
	
	/** CD Player*/
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
			5:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track6 ),
			6:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track7 ),
			7:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track8 ),
			8:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track9 ),
			9:MFT.PlaylistItem.create( MFT.locale.label.view_media_cd_playList_track10 ),
		},
		
		homeWidgetIcon: 'images/media/cd-ico-home.png',
		homeWidgetIcon_blur: 'images/media/help/cd-ico-home_blur.png',
	}),
	
	browseData : Em.Object.create({
				
		titleBinding:	'MFT.locale.label.view_media_cd_browse_CDTrack',
		
		disableScrollbar: false,
		
		currentPage: 0,
		
		items: [
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.0.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.1.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.2.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.3.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.4.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.5.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.6.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.7.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.8.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.9.title',
						disabled:	true
					}
				}
		]
	}),
	
	optionsData: Em.Object.create({
				
		view: 'cd',
		
		titleBinding: 'MFT.locale.label.view_media_cd_options_title',
		
		items:[
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_bt_options_soundSettings',
						templateName:		  'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LabelPlusToggleButton,
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_cd_options_compression',
						tButtonValue: 		 1,
						tButtonRange:		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_cd_options_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}		
				}
			]
	})
});
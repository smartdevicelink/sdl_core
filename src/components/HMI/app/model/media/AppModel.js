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

MFT.AppModel = Em.Object.create({
	
	active: false,
	
	/** App Player*/
	init: function(){
		this._super();
		this.set('player', MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
	PlayList: MFT.Playlist.create({
		
		selectedIndex: 	0,
		
		items: {
			0:MFT.PlaylistItem.create({field1: 'title', field2:'album', mediaTrack:'artist', mediaClock: "0:00", appName: "<App name>"})
		},
		
		homeWidgetIcon: 'images/media/sd-h-ico.png',
		homeWidgetIcon_blur: 'images/media/help/sd-h-ico_blur.png'
	}),

browseData : Em.Object.create({
		
		title: 'USB SD Drive DSK4',
		
		disableScrollbar: true,
		
		items:	[
			{
				type:MFT.Button,
				params:{
					text:'Music',
					disabled:	true
				}
			},
			{
				type:MFT.Button,
				params:{
					text:'Explore device',
					disabled:	true
				}
			}
		]
	})
});
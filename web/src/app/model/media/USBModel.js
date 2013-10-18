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
	
	/** USB Player*/
	init: function(){
		this._super();
		this.set('player',MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
	PlayList: MFT.Playlist.create({
		selectedIndex: 					0,
			items: {
			0:MFT.PlaylistItem.create({
				titleBinding: 'MFT.locale.label.view_media_track_1', album:'Color of the Fall', artist:'Luk Marko', genre:'Rock', disk:'Fall', duration:123} ),
			1:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_2',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
			2:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_3',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
			3:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_4',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
			4:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_5',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
			5:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_6',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123})
		},	
		homeWidgetIcon: 'images/media/usb-h-ico.png'
	})
});
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
	
	/** SD Player*/
	init: function(){
		this._super();
		this.set('player',MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
	PlayList: MFT.Playlist.create({
		
		selectedIndex: 					4,
		
		items: {
			0:MFT.PlaylistItem.create({
					titleBinding: 'MFT.locale.label.view_media_track_1', album:'Lazerhorse', artist:'The Electric Broncos', genreBinding:'MFT.locale.label.view_media_genre_electronic', disk:'Lazerhorse', duration:123} ),
				1:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_2',album:'Lazerhorse',artist:'The Electric Broncos',genreBinding:'MFT.locale.label.view_media_genre_electronic',disk:'Lazerhorse',duration:123}),
				2:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_3',album:'Lazerhorse',artist:'The Electric Broncos',genreBinding:'MFT.locale.label.view_media_genre_electronic',disk:'Lazerhorse',duration:123}),
				3:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_4',album:'Lazerhorse',artist:'The Electric Broncos',genreBinding:'MFT.locale.label.view_media_genre_electronic',disk:'Lazerhorse',duration:123}),
				4:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_5',album:'Lazerhorse',artist:'The Electric Broncos',genreBinding:'MFT.locale.label.view_media_genre_electronic',disk:'Lazerhorse',duration:123}),
				5:MFT.PlaylistItem.create({titleBinding: 'MFT.locale.label.view_media_track_6',album:'Lazerhorse',artist:'The Electric Broncos',genreBinding:'MFT.locale.label.view_media_genre_electronic',disk:'Lazerhorse',duration:123})
		},
		
		homeWidgetIcon: 'images/media/sd-h-ico.png'
	})
});
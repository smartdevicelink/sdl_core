/**
 * @name MFT.SiriusData
 * 
 * @desc Sirius Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/SiriusData.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
MFT.SiriusModel = Em.Object.create( {
	
	band:	MFT.RangedValue.create({value:-1, activeBand:0}),
	
	directTunestations: MFT.Playlist.create( {
		/** Direct tune Data */
		directTuneItems: {
			0:MFT.PlaylistItem.create({frequency: 'Hard Radio',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			1:MFT.PlaylistItem.create({frequency: 'Pop2K',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			2:MFT.PlaylistItem.create({frequency: 'Moto FM',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			3:MFT.PlaylistItem.create({frequency: 'Kids Radio',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			4:MFT.PlaylistItem.create({frequency: 'Lite Radio',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			5:MFT.PlaylistItem.create({frequency: 'Home FM',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			6:MFT.PlaylistItem.create({frequency: 'Highway',genreBinding: 'MFT.locale.label.view_media_genre_electronic',title: 'Electro',artist: 'NFS Band'}),
			7:MFT.PlaylistItem.create({frequency: 'Disco',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			8:MFT.PlaylistItem.create({frequency: 'The Pulse',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			9:MFT.PlaylistItem.create({frequency: 'Elvis Radio',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			10:MFT.PlaylistItem.create({frequency: 'Classic Vini',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			11:MFT.PlaylistItem.create({frequency: 'The Loft',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			12:MFT.PlaylistItem.create({frequency: 'Deep Track',genreBinding: 'MFT.locale.label.view_media_genre_electronic',title: 'Electro',artist: 'NFS Band'}),
			13:MFT.PlaylistItem.create({frequency: 'Jam On',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			14:MFT.PlaylistItem.create({frequency: 'The Pulse',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			15:MFT.PlaylistItem.create({frequency: 'The Heat',genre: 'R&B',title: 'Butterfly',artist: 'Kids Band'}),
			16:MFT.PlaylistItem.create({frequency: 'BPM',genre: 'Dance',title: 'Sea Sound',artist: 'Mark Elton'}),
			17:MFT.PlaylistItem.create({frequency: 'Home FM',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			18:MFT.PlaylistItem.create({frequency: 'Highway',genreBinding: 'MFT.locale.label.view_media_genre_electronic',title: 'Electro',artist: 'NFS Band'}),
			19:MFT.PlaylistItem.create({frequency: 'Hard Radio',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			20:MFT.PlaylistItem.create({frequency: 'Moto FM',genre: 'Pop',title: 'Road',artist: 'Mario'})
		}
		
	}),
			
	sir1: MFT.Playlist.create( {
		
		selectedIndex: 					3,

		items: {
			0:MFT.PlaylistItem.create({frequency: '80s on 8',channel: 'Channel 23',genre: 'Rock',title: 'The Moment',artist: '4U'}) ,
			1:MFT.PlaylistItem.create({frequency: 'ClsVinyl',channel: 'Channel 11',genre: 'Pop',title: 'My Soul',artist: 'Larry Sitra'}),
			2:MFT.PlaylistItem.create({frequency: 'Hits',channel: 'Channel 38',genre: 'Pop Rock',title: 'Come with me',artist: 'The bees'}),
			3:MFT.PlaylistItem.create({frequency: '1st Wave',channel: 'Channel 2',genre: 'Pop',title: 'Rainbow',artist: 'The Sky'}),
			4:MFT.PlaylistItem.create({frequency: 'Titanium',channel: 'Channel 34',genre: 'Rock',title: 'The Spring',artist: 'Jessica Rocky',logo: 'images/media/sirius-logo.png'}),
			5:MFT.PlaylistItem.create({frequency: 'Margvlle',channel: 'Channel 1',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
		}
	}),
	
	sir2: MFT.Playlist.create( {
		selectedIndex: 					3,

		items: {
			0:MFT.PlaylistItem.create({frequency: 'Retro Fm',channel: 'Channel 14',genre: 'Rock',title: 'The Moment',artist: '4U'}),
			1:MFT.PlaylistItem.create({frequency: 'Classic Pop',channel: 'Channel 11',genre: 'Pop',title: 'My Soul',artist: 'Larry Sitra'}),
			2:MFT.PlaylistItem.create({frequency: 'Hits 60s',channel: 'Channel 4',genre: 'Pop Rock',title: 'Love me',artist: 'The Brothers'}),
			3:MFT.PlaylistItem.create({frequency: 'Pop Fm',channel: 'Channel 2',genre: 'Pop',title: 'Angels',artist: 'Michael Johnson'}),
			4:MFT.PlaylistItem.create({frequency: 'Mix',channel: 'Channel 39',genre: 'Rock',title: 'Hello From Odessa',artist: 'The Black Sea'}),
			5:MFT.PlaylistItem.create({frequency: 'Classic FM',channel: 'Channel 2',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
		}
	}),
	
	sir3: MFT.Playlist.create({
		selectedIndex: 					3,
		
		items: {
			0:MFT.PlaylistItem.create({frequency: 'Best FM',channel: 'Channel 43',genre: 'Rock',title: 'Love me',artist: 'The Brothers'}),
			1:MFT.PlaylistItem.create({frequency: 'Kiss FM',channel: 'Channel 31',genre: 'Pop',title: 'The Moment',artist: '4U'}),
			2:MFT.PlaylistItem.create({frequency: 'Plus FM',channel: 'Channel 9',genre: 'Pop Rock',title: 'Rainbow',artist: 'The Sky'}),
			3:MFT.PlaylistItem.create({frequency: 'Maximum',channel: 'Channel 4',genre: 'Pop',title: 'Come with me',artist: 'The Bees'}),
			4:MFT.PlaylistItem.create({frequency: 'Modern',channel: 'Channel 37',genre: 'Rock',title: 'The Spring',artist: 'Jessics Rocky'}),
			5:MFT.PlaylistItem.create({frequency: 'Nostalgie',channel: 'Channel 32',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
		}
	})
});

/**
 * @name MFT.FmData
 * 
 * @desc FM Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/FmData.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.FmModel = Em.Object.create({

    init: function () {
        var i,
            directTunes = [],
            directTuneItems = {},
            frequency,
            pointIndex;

        this._super();

        for (i = 879; i <= 1079; i++) {
            directTunes.push(i.toString().split(''));

            pointIndex = (i <= 1000) ? 2 : 3;
            frequency = i.toString();
            frequency = frequency.slice(0,pointIndex)+'.'+frequency.slice(pointIndex);

            directTuneItems[i] = MFT.PlaylistItem.create({frequency: frequency});
        }

        this.directTunestations.set('directTunes', directTunes);
        this.directTunestations.set('directTuneItems', directTuneItems);
    },
	
	band: MFT.RangedValue.create({value:-1, activeBand:0}),

    customDirectTuneStations: MFT.Playlist.create({
        directTuneItems: {}
    }),
	
	directTunestations: MFT.Playlist.create({
		
			/** Direct tune dial station matrix */
			directTunes: [],
			
			/** Direct tune Data */
			directTuneItems: {}
	
	}),
	
	fm1: MFT.Playlist.create( {
		selectedIndex: 					1,

		items: {
			0:MFT.PlaylistItem.create({frequency: '99.1',genre: 'Pop',title: 'BlUE SKY',artist: 'THE MAX', isHd: false}) ,
			1:MFT.PlaylistItem.create({frequency: '98.8',genre: 'Club',title: 'JUMP AND DOWN',artist: 'THE PROJECT X', isHd: false}),
			2:MFT.PlaylistItem.create({frequency: '100.9',genre: 'Rock',title: 'WELCOME HOME',artist: 'TODD SULLIVAN',isHd: MFT.SettingsModel.isEnglish, HDChannels: 3, currentHDChannel: 2}),
			3:MFT.PlaylistItem.create({frequency: '103.5',genre: 'Pop',title: 'LETS DANCE',artist: 'MICHAEL JOHNSON',isHd: MFT.SettingsModel.isEnglish, HDChannels: 2, currentHDChannel: 1}),
			4:MFT.PlaylistItem.create({frequency: '106.1',genre: 'Pop Rock',title: 'YESTERDAY NIGHT',artist: 'JOHN SMITH', isHd: false}),
			5:MFT.PlaylistItem.create({frequency: '105.2',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'TENTH SYMPHONY',artist: 'SPENCER M.', isHd: false})
		}
	}),
	
	fm2: MFT.Playlist.create( {
		selectedIndex: 					4,

		items:{
			0:MFT.PlaylistItem.create({frequency: '101.1',genre: 'Club',title: 'SPRING TIME',artist: 'DJ SKY', isHd: false}) ,
			1:MFT.PlaylistItem.create({frequency: '103.2',genre: 'Rock',title: 'RAINBOW',artist: 'THE BEES', isHd: false}),
			2:MFT.PlaylistItem.create({frequency: '99.3',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'SUNSET',artist: 'SKYLARK',isHd: MFT.SettingsModel.isEnglish, HDChannels: 3, currentHDChannel: 2}),
			3:MFT.PlaylistItem.create({frequency: '103.5',genre: 'Club',title: 'JUMP AND DOWN',artist: 'THE PROJECT X',isHd: MFT.SettingsModel.isEnglish, HDChannels: 3, currentHDChannel: 3}),
			4:MFT.PlaylistItem.create({frequency: '104.1',genre: 'Pop',title: 'HONEY',artist: 'EPTON JOHN', isHd: false}),
			5:MFT.PlaylistItem.create({frequency: '105.2',genre: 'Pop',title: 'LETS DANCE',artist: 'MICHAEL JOHNSON', isHd: false})
		}
	}),
	
	fmAst: MFT.Playlist.create( {				
		selectedIndex: 					1,

		items:{
			0:MFT.PlaylistItem.create({frequency: '98.2',genre: 'Club',title: 'SPRING TIME',artist: 'DJ SKY', isHd: false}) ,
			1:MFT.PlaylistItem.create({frequency: '106.6',genre: 'Rock',title: 'WELCOME HOME',artist: 'TODD SULLIVAN', isHd: false}),
			2:MFT.PlaylistItem.create({frequency: '99.3',genre: 'Pop',title: 'GOOD MORNING',artist: 'SUSAN BAKER',isHd: MFT.SettingsModel.isEnglish, HDChannels: 2, currentHDChannel: 2}),
			3:MFT.PlaylistItem.create({frequency: '107.5',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'SUNSET',artist: 'SKYLARK',isHd: MFT.SettingsModel.isEnglish, HDChannels: 3, currentHDChannel: 1}),
			4:MFT.PlaylistItem.create({frequency: '106.1',genre: 'Pop',title: 'HONEY',artist: 'EPTON JOHN', isHd: false}),
			5:MFT.PlaylistItem.create({frequency: '104.2',genreBinding: 'MFT.locale.label.view_media_genre_classic',title: 'TENTH SYMPHONY',artist: 'SPENCER M.', isHd: false})
		}

	})

});
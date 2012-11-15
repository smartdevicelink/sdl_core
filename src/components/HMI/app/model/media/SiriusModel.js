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
	
	active: false,
	
	directTunestations: MFT.Playlist.create( {
		/** Direct tune Data */
		directTuneItems: {
			0:MFT.PlaylistItem.create({frequency: 'Hard Radio',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			1:MFT.PlaylistItem.create({frequency: 'Pop2K',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			2:MFT.PlaylistItem.create({frequency: 'Moto FM',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			3:MFT.PlaylistItem.create({frequency: 'Kids Radio',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			4:MFT.PlaylistItem.create({frequency: 'Lite Radio',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			5:MFT.PlaylistItem.create({frequency: 'Home FM',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			6:MFT.PlaylistItem.create({frequency: 'Highway',genre: 'Electronic',title: 'Electro',artist: 'NFS Band'}),
			7:MFT.PlaylistItem.create({frequency: 'Disco',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			8:MFT.PlaylistItem.create({frequency: 'The Pulse',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			9:MFT.PlaylistItem.create({frequency: 'Elvis Radio',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			10:MFT.PlaylistItem.create({frequency: 'Classic Vini',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			11:MFT.PlaylistItem.create({frequency: 'The Loft',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			12:MFT.PlaylistItem.create({frequency: 'Deep Tracks',genre: 'Electronic',title: 'Electro',artist: 'NFS Band'}),
			13:MFT.PlaylistItem.create({frequency: 'Jam On',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			14:MFT.PlaylistItem.create({frequency: 'The Spectrum',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			15:MFT.PlaylistItem.create({frequency: 'The Heat',genre: 'R&B',title: 'Butterfly',artist: 'Kids Band'}),
			16:MFT.PlaylistItem.create({frequency: 'BPM',genre: 'Dance',title: 'Sea Sound',artist: 'Mark Elton'}),
			17:MFT.PlaylistItem.create({frequency: 'Home FM',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			18:MFT.PlaylistItem.create({frequency: 'Highway',genre: 'Electronic',title: 'Electro',artist: 'NFS Band'}),
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
			5:MFT.PlaylistItem.create({frequency: 'Margvlle',channel: 'Channel 1',genre: 'Classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
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
			5:MFT.PlaylistItem.create({frequency: 'Classic FM',channel: 'Channel 2',genre: 'Classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
		}
	}),
	
	sir3: MFT.Playlist.create( {
		selectedIndex: 					3,
		
		items: {
			0:MFT.PlaylistItem.create({frequency: 'Best FM',channel: 'Channel 43',genre: 'Rock',title: 'Love me',artist: 'The Brothers'}),
			1:MFT.PlaylistItem.create({frequency: 'Kiss FM',channel: 'Channel 31',genre: 'Pop',title: 'The Moment',artist: '4U'}),
			2:MFT.PlaylistItem.create({frequency: 'Plus FM',channel: 'Channel 9',genre: 'Pop Rock',title: 'Rainbow',artist: 'The Sky'}),
			3:MFT.PlaylistItem.create({frequency: 'Maximum',channel: 'Channel 4',genre: 'Pop',title: 'Come with me',artist: 'The Bees'}),
			4:MFT.PlaylistItem.create({frequency: 'Modern',channel: 'Channel 37',genre: 'Rock',title: 'The Spring',artist: 'Jessics Rocky'}),
			5:MFT.PlaylistItem.create({frequency: 'Nostalgie',channel: 'Channel 32',genre: 'Classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
		}
	}),
	
	browseData : Em.Object.create({
				
		titleBinding: 'MFT.locale.label.view_media_sirius_browse_title',
		
		currentPage: 0,
		
		disableScrollbar: false,
		
		items: [
				{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_AllChanels',
						rightText: '158',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_pop',
						rightText: '13',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_rock',
						rightText: '24',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_hipHop',
						rightText: '6',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_dance',
						rightText: '3',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_classic',
						rightText: '235',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_retro',
						rightText: '43',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_trance',
						rightText: '56',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_jazz',
						rightText: '201',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_disco',
						rightText: '17',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				}
		]
		
	}),
	
	optionsData : Em.Object.create({
				
		view: 'sirius',
		
		titleBinding:	'MFT.locale.label.view_media_sirius_options_title',
		
		items: [
			{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_sirius_options_soundSettings',
						templateName:		  'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_setCategory',
						buttonTexBindingt:	'MFT.locale.label.view_media_sirius_options_allChannels',
						labelDisabled:		true,
						templateName:		  'arrow',
						buttonDisabled: 	   true
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_parentalLockout',
						buttonTexBindingt:	'MFT.locale.label.view_media_sirius_options_changePIN',
						labelDisabled:		true,
						templateName:		  'arrow',
						buttonDisabled: 	   true
					}							
				},
						
				{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_alerts',
						tButtonValue:		 1,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_sirius_options_OnOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				},		
								
				{
					type:		MFT.Label,
	
					params:{
						contentBinding: 'MFT.locale.label.view_media_sirius_options_serialNumber',
						disabled: 	 true
					}							
				},
			]
		})
});

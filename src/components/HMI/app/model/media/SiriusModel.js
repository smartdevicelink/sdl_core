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
			
	sir1: MFT.Playlist.create( {
		selectedIndex: 					3,

		items: {
			0:MFT.PlaylistItem.create({frequency: '80s on 8',channel: 'Channel 23',genre: 'Rock',title: 'The Moment',artist: '4U'}) ,
			1:MFT.PlaylistItem.create({frequency: 'ClsVinyl',channel: 'Channel 11',genre: 'Pop',title: 'My Soul',artist: 'Larry Sitra'}),
			2:MFT.PlaylistItem.create({frequency: 'Hits',channel: 'Channel 38',genre: 'Pop Rock',title: 'Come with me',artist: 'The bees'}),
			3:MFT.PlaylistItem.create({frequency: '1st Wave',channel: 'Channel 2',genre: 'Pop',title: 'Rainbow',artist: 'The Sky'}),
			4:MFT.PlaylistItem.create({frequency: 'Titanium',channel: 'Channel 34',genre: 'Rock',title: 'The Spring',artist: 'Jessica Rocky',logo: 'images/media/sirius-logo.png'}),
			5:MFT.PlaylistItem.create({frequency: 'Margvlle',channel: 'Channel 1',genre: 'Classic',title: 'Tenth Symphony',artist: 'Dr. Classic'})					
		},
		
		/** Direct tune dial station matrix */
		directTunes: [
			[1,1,6],
			[1,4,5],
			[3,2,5],
			[3,8,6],
			[7,2,4],
			[7,3,9]
		],
		
		/** Direct tune Data */
		directTuneItems: {
			116:MFT.PlaylistItem.create({frequency: 'Hard Radio',channel: 'Channel 116',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			145:MFT.PlaylistItem.create({frequency: 'Moto FM',channel: 'Channel 145',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			325:MFT.PlaylistItem.create({frequency: 'Kids Radio',channel: 'Channel 325',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			386:MFT.PlaylistItem.create({frequency: 'Lite Radio',channel: 'Channel 386',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			724:MFT.PlaylistItem.create({frequency: 'Home FM',channel: 'Channel 724',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			739:MFT.PlaylistItem.create({frequency: 'Highway',channel: 'Channel 739',genre: 'Electronic',title: 'Electro',artist: 'NFS Band'})
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
		},
		
		/** Direct tune dial station matrix */
		directTunes: [
			[1,1,6],
			[1,4,5],
			[3,2,5],
			[3,8,6],
			[7,2,4],
			[7,3,9]
		],
		
		/** Direct tune Data */
		directTuneItems: {
			116:MFT.PlaylistItem.create({frequency: 'Hard Radio',channel: 'Channel 116',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			145:MFT.PlaylistItem.create({frequency: 'Moto FM',channel: 'Channel 145',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			325:MFT.PlaylistItem.create({frequency: 'Kids Radio',channel: 'Channel 325',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			386:MFT.PlaylistItem.create({frequency: 'Lite Radio',channel: 'Channel 386',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			724:MFT.PlaylistItem.create({frequency: 'Home FM',channel: 'Channel 724',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			739:MFT.PlaylistItem.create({frequency: 'Highway',channel: 'Channel 739',genre: 'Electronic',title: 'Electro',artist: 'NFS Band'})
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
		},
		
		/** Direct tune dial station matrix */
		directTunes: [
			[1,1,6],
			[1,4,5],
			[3,2,5],
			[3,8,6],
			[7,2,4],
			[7,3,9]
		],
		
		/** Direct tune Data */
		directTuneItems: {
			116:MFT.PlaylistItem.create({frequency: 'Hard Radio',channel: 'Channel 116',genre: 'Rock',title: 'Rock Together',artist: 'The Band'}),
			145:MFT.PlaylistItem.create({frequency: 'Moto FM',channel: 'Channel 145',genre: 'Pop',title: 'Road',artist: 'Mario'}),
			325:MFT.PlaylistItem.create({frequency: 'Kids Radio',channel: 'Channel 325',genre: 'Pop',title: 'Butterfly',artist: 'Kids Band'}),
			386:MFT.PlaylistItem.create({frequency: 'Lite Radio',channel: 'Channel 386',genre: 'Techno',title: 'Sea Sound',artist: 'Mark Elton'}),
			724:MFT.PlaylistItem.create({frequency: 'Home FM',channel: 'Channel 724',genre: 'Rock',title: 'Take it easy',artist: 'No Rules'}),
			739:MFT.PlaylistItem.create({frequency: 'Highway',channel: 'Channel 739',genre: 'Electronic',title: 'Electro',artist: 'NFS Band'})
		}
	}),
	
	browseData : Em.Object.create({
				
		title: 'SIRIUS Channel Guide',
		
		currentPage: 0,
		
		disableScrollbar: false,
		
		items: [
				{
					type:MFT.Button,
					params:{
						text:'All Chanels',
						rightText:	'158',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						text:'Pop',
						rightText:	'13',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Rock',
						rightText:	'24',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Hip-Hop / R&B',
						rightText:	'6',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Dance / Electrinic',
						rightText:	'3',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Classic',
						rightText:	'235',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Retro',
						rightText:	'43',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Trance',
						rightText:	'56',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Jazz',
						rightText:	'201',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						text:'Disco',
						rightText:	'17',
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
						arrow:		true,
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_setCategory',
						buttonTexBindingt:	'MFT.locale.label.view_media_sirius_options_allChannels',
						labelDisabled:		true,
						buttonArrow:		  true,
						buttonDisabled: 	   true
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_sirius_options_parentalLockout',
						buttonTexBindingt:	'MFT.locale.label.view_media_sirius_options_changePIN',
						labelDisabled:		true,
						buttonArrow:		  true,
						buttonDisabled: 	   true
					}							
				},
						
				{
								
					type:		MFT.LablePlusToggleButton,
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

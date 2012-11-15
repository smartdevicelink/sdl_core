/**
 * @name MFT.PlaylistItem
 * 
 * @desc frequency Data Class
 * 
 * @category	Model
 * @filesource	app/model/media/PlaylistItem.js
 * @version	2.0
 *
 * @author 	Igor Zhavoronkin
 */
MFT.PlaylistItem = Em.Object.extend({
	
	/** HD Flag for FM Stations*/
	isHd: 		false,
	
	/** frequency name */
	frequency:'',
	
	/** Song GENRE */
	genre: 		null,
	
	/** Artist name */
	artist:		null,
	
	/** Song title */
	title: null,
	
	/** Logo image path for station */
	logo: '',
	
	/** Track duration*/
	duration: null,
	
	
	copy: function(src) {
		Ember.beginPropertyChanges();
		this.set('isHd', 			src.isHd);
		this.set('frequency', 		src.frequency);
		this.set('genre', 			src.genre);
		this.set('artist', 			src.artist);
		this.set('title', 			src.title);
		this.set('logo', 			src.logo);
		this.set('channel', 			src.channel);
		Ember.endPropertyChanges();
	}
})
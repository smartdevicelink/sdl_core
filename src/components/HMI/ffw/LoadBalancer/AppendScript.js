/**
 * @name FFW.AppendScript
 * 
 * @desc Dynamic JS insert to DOM
 * 
 * @category	Framework
 * @filesource	app/LoadBalancer/AppendScript.js
 * @version	2.0
 *
 * @author	Igor Zhavoronkin
 */

FFW.AppendScript =  Em.Object.create({
	
	// script tag obj 
	script: null,
	
	sciptsQueue: [],
	
	loading: false,
	
	// push script url to loading queue
	pushScript: function( src ){
		if(!src) {
			return;
		}
		
		this.sciptsQueue.push( src );
		
		// start loading if queue are free
		if ( !this.loading ) {
			this.loadNext();
		}		
	},
	
	// onLoad handeler
	// dispatch when script loaded
	// can load next script from queue if necessary
	onLoad: function() {
		this.set('loading', false);
		
		if ( this.sciptsQueue.length ) {
			this.loadNext()
		}
	},
	
	// shift script from queue
	// and call loading
	loadNext: function() {
		this.set('loading', true);
		this.loadScript( this.sciptsQueue.shift() );
	},
	
	// load script
	loadScript: function( url ) {
		
		//Create new script obj
		this.set('script' , document.createElement( 'script' ));
		this.script.type = 'text/javascript';
		
		// Pass current src
		this.script.src = FFW.isAndroid ? url : url +'?time='+Date.now();
		
		//append script to body
		document.body.appendChild(this.script);
	}
});
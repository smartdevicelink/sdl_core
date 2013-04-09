/**
 * @name SDL.ButtonsGroup
 * @desc Component with three buttons
 * @category Controlls
 * @filesource app/controlls/ButtonsGroup.js
 * @version 1.0
 */

SDL.ButtonsGroup = Em.ContainerView.extend( {

    index: null,

    /** Content binding */
    content: null,

    /** Internal value */
    innerValue: null,

    /** Internal range */
    innerRange: null,

    /** Value binding */
    value: null,

    /** Range binding */
    range: null,

    /** Text on buttons */
    labels: '',

    /** Icons on buttons */
    icons: '',

    /** Disable toggle button */
    disabled: false,

    /** Classes of buttons */
    classes: '',

    /** Create delimeters, default 'true' */
    delimeters: true,

    afterRender: function() {
        var element, self = this;

        var controller = this.get( 'controller' ) ? this.get( 'controller' ) : SDL.MCSController;

        /** Creation of containers of buttons and delimiters */
        for( var i = 0; i < this.innerRange; i++ ){
            /** Button */
            element = SDL.Button.create( {
                templateName: 'text',

                classNames: ' button' + i + ' ' + self.classes,
                classNameBindings:
                    [
                        'isSelected:active',
                        'parentView.disabled'
                    ],
                index: i,
                isSelected: function() {
                    if( this._parentView.get( 'innerValue' ) == this.get( 'index' ) ){
                        return true;
                    }else{
                        return false;
                    }
                }.property( 'this._parentView.innerValue' ),
                text: self.labels[i] != null ? self.labels[i] : '',
                icon: self.icons[i] != null ? self.icons[i] : null,
                action: self.action,
                target: self.target ? self.target : self,

                /** Highlight the border of buttons */
                actionDown: function() {
                    if( !this._parentView.disabled ){
                        this._parentView.set( 'value', this.get( 'index' ) );
                        controller.set( 'highlighted', true );
                        this._super();
                    }

                },

                /** Remove the highlight of the border of buttons */
                actionUp: function() {
                    this._super();

                    if( SDL.CAN_SIMULATION_MODE ){
                        controller.model.send( this._parentView.index, 'off' );
                    }
                    controller.set( 'highlighted', false );
                },

                /** Remove the highlight of the border of buttons */
                mouseLeave: function() {

                    if( SDL.CAN_SIMULATION_MODE && this.pressed ){
                        controller.send( this._parentView.index, 'off' );
                    }
                    ;
                    controller.set( 'highlighted', false );

                    this._super();
                }
            } );
            this.get( 'childViews' ).pushObject( element );

            /** Create delimeter if needed */
            if( i < this.innerRange - 1 && this.delimeters ){
                /** Delimiter of buttons */
                element = Em.View.create( {
                    classNames: 'delim delim-' + i
                } );
                this.get( 'childViews' ).pushObject( element );
            }
        }
    },

    beforeRender: function() {

        var self = this;

        // Set range value in innerRange
        if( null != this.content ){
            this.set( 'innerValue', this.content.value );
            this.set( 'innerRange', this.content.range );
        }else{
            this.set( 'innerValue', this.value );
            this.set( 'innerRange', this.range );
        }

        // add observer to value
        this.addObserver( 'value', function() {
            this.set( 'innerValue', this.value );
        } );

        // add observer to innerValue
        this.addObserver( 'innerValue', function() {
            self.toggleThreeButtons();
        } );

        // apply ButtonGroup visualization rule
        self.setRecord();

        // add observer to content record
        this.addObserver( 'content', function() {
            self.setRecord();
        } );

        // view internal call
        this.applyAttributesToBuffer( this.buffer );
    },

    // change record binding
    setRecord: function() {

        var view = this;

        if( null != this.content ){
            if( this.content.observersForKey( 'value' ).length == 0 ){

                this.content.addObserver( 'value', function() {
                    view.set( 'innerValue', view.content.value );
                } );

            }
            view.set( 'innerValue', view.content.value );
        }
    },

    // Toggle Buttons handeler
    toggleThreeButtons: function() {

        if( this.innerRange == this.get( 'index' ) ){

            this.body.set( 'style', 'opacity:' + this.innerValue / this.innerRange + ';' );

        }
    }
} );
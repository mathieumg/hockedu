// CLOUD namespace
var CLOUD = new Object();

CLOUD = {
    currentSubsite: '',
	
	contentLocation: '',

    isLoggedIn: false,

    mainDomain: '',

    changeSubsite: function( subsite )
    {
        document.location.href = 'http://' + subsite + '.' + CLOUD.mainDomain;
    },
	
	errors: {
		errorArray: null,

		lastFocus: null,

		add: function( subPageName, errorText, focusElement ) {
			if( CLOUD.errors.errorArray === null )
			{
				CLOUD.errors.errorArray = new Array( );
			}

			if( ( typeof CLOUD.errors.errorArray[ subPageName ] ) === 'undefined' )
			{
				CLOUD.errors.errorArray.push( subPageName );
				CLOUD.errors.errorArray[ subPageName ] = new Array();
			}
			
			CLOUD.errors.errorArray[ subPageName ].push( errorText );

			if( CLOUD.errors.lastFocus == null )
			{
				CLOUD.errors.lastFocus = focusElement;
			}
		},

		count: function( subPageName ) {
			if( CLOUD.errors.errorArray !== null && ( typeof CLOUD.errors.errorArray[ subPageName ] ) !== 'undefined' )
			{
				return CLOUD.errors.errorArray[ subPageName ].length;
			}
		},

		display: function( subPageName ) {
			if( CLOUD.errors.errorArray[ subPageName ].length > 0 )
			{
				alertCode = '<ul>';

				for( var i = 0; i < CLOUD.errors.errorArray[ subPageName ].length; i++ )
				{
					alertCode += '<li>' + CLOUD.errors.errorArray[ subPageName ][ i ] + '</li>';
				}

				alertCode += '</ul>';

				CLOUD.simpleDialog( 'Erreur', alertCode, function() { $(CLOUD.errors.lastFocus).focus() } );
			}
		},
		
		empty: function( subPageName ) {
			if( CLOUD.errors.errorArray !== null && ( typeof CLOUD.errors.errorArray[ subPageName ] ) !== 'undefined' )
			{
				CLOUD.errors.errorArray[ subPageName ].length = 0;
				CLOUD.errors.lastFocus = null;
			}
		}
	},

    initializePage: function(moduleName) {
        // Are we on a page that needs extra initialization?
        switch( moduleName )
        {
            case 'connexion':
                CLOUD.loginPage.initialize();
                break;
                
            default: 
                // Nothing extra to do for the default site, let's see for the current subsite.
                if( CLOUD.currentSubsite != '' && CLOUD.hasOwnProperty( CLOUD.currentSubsite ) )
                {
                    CLOUD[ CLOUD.currentSubsite ].initializePage( moduleName );
                }
                break;
        }
    },

    initializeSite: function() {
        // Prototype extensions.
		prototypeExtensions();
		
		// Localization.
		$.datepicker.regional['fr'] = {
			clearText: 'Effacer', clearStatus: '',
			closeText: 'Fermer', closeStatus: 'Fermer sans modifier',
			prevText: '<Préc', prevStatus: 'Voir le mois précédent',
			nextText: 'Suiv>', nextStatus: 'Voir le mois suivant',
			currentText: 'Courant', currentStatus: 'Voir le mois courant',
			monthNames: ['Janvier','Février','Mars','Avril','Mai','Juin',
			'Juillet','Août','Septembre','Octobre','Novembre','Décembre'],
			monthNamesShort: ['Jan','Fév','Mar','Avr','Mai','Jun',
			'Jul','Aoû','Sep','Oct','Nov','Déc'],
			monthStatus: 'Voir un autre mois', yearStatus: 'Voir un autre année',
			weekHeader: 'Sm', weekStatus: '',
			dayNames: ['Dimanche','Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi'],
			dayNamesShort: ['Dim','Lun','Mar','Mer','Jeu','Ven','Sam'],
			dayNamesMin: ['Di','Lu','Ma','Me','Je','Ve','Sa'],
			dayStatus: 'Utiliser DD comme premier jour de la semaine', dateStatus: 'Choisir le DD, MM d',
			dateFormat: 'dd/mm/yy', firstDay: 0, 
			initStatus: 'Choisir la date', isRTL: false
		};
		
		$.datepicker.setDefaults($.datepicker.regional['fr']);
		
		// Default settings for ajax requests.
        $.ajaxSetup({
            cache: false,
            dataType: 'json',
            error: function(){
                // There was an error processing the request.
                CLOUD.simpleDialog( 'Erreur', 'Une erreur inconnue est survenue lors de l\'envoi des données.' );
            },
            timeout: 5000,
            type: 'POST'
        });

        // Load external resources such as html parts, domain names, login status, etc.
        $.ajax('http://' + document.location.host + '/ajax/resources', {
            cache: true,
            error: function() {
                CLOUD.initializeSite();
            },
            success: function(json, textStatus) {
                // Sessions.
                CLOUD.isLoggedIn = json.permissions.isLoggedIn;

                CLOUD.currentSubsite = json.subsite;
                CLOUD.mainDomain = json.domainName;
				CLOUD.contentLocation = json.contentLocation;

                // Then call page-specific initializers, if need be.
                $.each( $('#content').attr('class').split(/\s+/), function(index, item){
                    if( item.slice(0, 5) == 'page_' )
                    {
                        CLOUD.initializePage( item.slice(5, item.length) );
                    }
                });

                // Things to do only if we're in the logged in part of the system.
                if( CLOUD.isLoggedIn )
                {
                    CLOUD.siteSelector.initialize();

                    $(window).resize( function() {
                        $('#col2').height( 0 );
                        $('#col2').height( $(document).height() - 100 );
                    });

                    $(window).resize();
                }

            },
            type: 'GET'
        });
    },

    simpleDialog: function( title, message, callback ) {
        return $('<div></div>')
            .html( '<p>' + message + '</p>' )
            .dialog({
                autoOpen: true,
                buttons: { 'Ok': function() {
                                $(this).dialog('close');
                                if( typeof callback !== 'undefined' ) {
                                    callback();
                                }
                            }
                        },
                closeText: 'Fermer',
                dialogClass: 'dialog',
                draggable: false,
                modal: true,
                resizable: false,
                title: title,
                width: 500
            });
    },
    
    advancedDialog: function( title, message, buttons, openEvent ) {		
		if( openEvent === 'undefined' ) { var openEvent = function(){} }
		
		return $('<div></div>')
            .html( '<p>' + message + '</p>' )
            .dialog({
                autoOpen: true,
                buttons: buttons,
				close: function(event, ui)
				{
					$(this).dialog('destroy').remove()
				},
                closeText: 'Fermer',
                dialogClass: 'dialog',
                draggable: false,
                modal: true,
				open: openEvent,
                resizable: false,
                title: title,
                width: 500
            });
    },
    
    animateUpdatedForm: function( selector ) {
        var inputSelector = $(selector);
        var originalBackground = inputSelector.css('background-color');
        
        inputSelector.css('background-color', 'green');
        
        inputSelector.animate({
            backgroundColor: originalBackground
        }, 2000, function() {
        // Animation complete.
        });
    },

    loginPage: {
        ipHandle: null,

        initialize: function() {
            // Create the overlay with the login box.
            $('#loginbox form').submit( function(event) {
                event.preventDefault();

                //CLOUD.simpleDialog( 'Erreur de connexion', 'Tu travailles toujours tout croche!' ).dialog( 'option', 'width', 200 ).dialog( 'option', 'position', 'center' );
                CLOUD.loginPage.validateForm();
            });
        },

        ipDialog: function( ipAddress ) {
            $('<div></div>')
                .html( '<p>Vous tentez de vous connecter d\'une adresse IP qui n\'est pas dans notre système: <strong>' + ipAddress + '</strong>. Désirez-vous recevoir un courriel pour ajouter cette adresse à la liste d\'accès pour <strong>24 heures</strong>?</p>' )
                .dialog({
                    autoOpen: true,
                    buttons: {
                        'Oui': function() { CLOUD.loginPage.sendIpRequest(); $(this).dialog('close'); },
                        'Non': function() { $(this).dialog('close'); }
                        },
                    closeText: 'Fermer',
                    dialogClass: 'dialog',
                    draggable: false,
                    modal: true,
                    resizable: false,
                    title: 'Nouvelle adresse IP',
                    width: 500
                });
        },

        sendIpRequest: function() {
            $.ajax( '/ajax/sendipticket', {
                success: function(data) {
                    // Successfully received the response, what does the data say?
                    switch( data.response )
                    {
                        // IP whitelising request successfully sent.
                        case 'success':
                            CLOUD.simpleDialog( 'Courriel d\'activation envoyé', 'Un courriel contenant un lien pour activer votre accès temporaire a été envoyé à l\'adresse courriel associée à votre compte.' );
                            break;
                        default:
                            // Unable to add to IP whitelist.
                            CLOUD.simpleDialog( 'Erreur', 'Impossible de vous ajouter à la liste d\'accès pour l\'instant, veuillez contacter l\'administrateur.' );
                    }
                },
            });
        },

        validateForm: function() {
            // Validate the form data.
            var username = $('#username').val();
            var password = $('#password').val();
            var system = $('#system').val();

            if( typeof( system ) == 'undefined' )
            {
                system = CLOUD.currentSubsite;
            }

            if( username == '' )
            {
                CLOUD.simpleDialog( 'Erreur', 'Veuillez saisir un nom d\'utilisateur.', function() { $('#username').focus() } );
            }
            else if( password == '' )
            {
                CLOUD.simpleDialog( 'Erreur', 'Veuillez saisir un mot de passe.', function() { $('#password').focus() } );
            }
            else
            {
                $('#loginbox input[type=submit]').attr('disabled','disabled');

                $.ajax( '/ajax/login', {
                    complete: function() {
                        // Re-enable the login button.
                        $('#loginbox input[type=submit]').removeAttr('disabled');
                    },
                    data: ({
                        username: username,
                        password: password,
                        system: system
                    }),
                    success: function(data){
                        // Successfully received the response, what does the data say?
                        switch( data.response )
                        {
                            // When the user has to make a request to whitelist his IP.
                            case 'newip':
                                CLOUD.loginPage.ipDialog( data.ipAddress );
                                break;

                            case 'nosubsite':
                                CLOUD.simpleDialog( 'Erreur de connexion', 'Vous n\'avez pas l\'autorisation de gérer le système que vous avez choisi.' );
                                break;
                            // When the user successfully logged in, just redirect him to the homepage.
                            case 'granted':
                                document.location.href = '/';

                                break;
                            default:
                                // Wrong login information was provided.
                                CLOUD.simpleDialog( 'Erreur de connexion', 'Mauvaise combinaison de nom d\'utilisateur/mot de passe.', function() { $('#username').select() } );
                        }
                    },
                });
            }
        }
    },

    siteSelector: {
        initialize: function() {
            $('#usermenu select').change( function(event) {
                CLOUD.changeSubsite( $(this).val() );
            });
        }
    }
};

// Fires up when the DOM is ready to be manipulated.
$(function() {
    CLOUD.initializeSite();
});

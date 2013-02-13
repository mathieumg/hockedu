CLOUD.globalrecreatif = {
    initializePage: function(moduleName) {
        // Are we on a page that needs extra initialization?
        switch( moduleName )
        {
            case 'client':
                if( $('#client-tabs').length > 0 )
                {                                        
                    $('#client-tabs').tabs({
                        ajaxOptions: {
                            cache: true,
                            dataType: 'text',
                            dataFilter: function ( result ){
                                var data = $.parseJSON( result );
                                return data.htmlcontent;
                            },
                            spinner: '<em>Chargement...</em>',
                            type: 'GET'
                        },
                        cache: true,
                        load: function(e, ui) { CLOUD.globalrecreatif.clientPage.loadTabData( ui.index ); },
                        select: function(event, ui) {
                            window.location.hash = ui.tab.hash;
                        }
                    });
                }
                else
                {
                    CLOUD.globalrecreatif.clientPage.initializeNewClientButtons();
                }
                
                break;
				
			case 'contrat':
				// New contract.
				if( $('#selectedpackage').length > 0 )
				{
					CLOUD.globalrecreatif.contractPage.initializeNewContract();
				}
				
				break;
				
			case 'facture':
				// New invoice.
				if( $('#invoice_create').length > 0 )
				{
					CLOUD.globalrecreatif.invoicePage.initialize();
				}
				
				
				break;
        }
    },
    
    clientPage: {
        focusedInputBuffer: null,
        
        errors: {

            errorArray: null,

            lastFocus: null,

            add: function( subPageName, errorText, focusElement ) {
                CLOUD.globalrecreatif.clientPage.errors.errorArray[ subPageName ].push( errorText );

                if( CLOUD.globalrecreatif.clientPage.errors.lastFocus == null )
                {
                    CLOUD.globalrecreatif.clientPage.errors.lastFocus = focusElement;
                }
            },

            count: function( subPageName ) {
                return CLOUD.globalrecreatif.clientPage.errors.errorArray[ subPageName ].length;
            },

            display: function( subPageName ) {
                if( CLOUD.globalrecreatif.clientPage.errors.errorArray[ subPageName ].length > 0 )
                {
                    alertCode = '<ul>';

                    for( var i = 0; i < CLOUD.globalrecreatif.clientPage.errors.errorArray[ subPageName ].length; i++ )
                    {
                        alertCode += '<li>' + CLOUD.globalrecreatif.clientPage.errors.errorArray[ subPageName ][ i ] + '</li>';
                    }

                    alertCode += '</ul>';

                    CLOUD.simpleDialog( 'Erreur', alertCode, function() { $(CLOUD.globalrecreatif.clientPage.errors.lastFocus).focus() } );
                }
            }
        },
        
        applyMasks: function() {
            $('#lastname').blur( function() {
                $(this).val( ucwords( $(this).val() ) );
            });
            
            $('#firstname').blur( function() {
                $(this).val( ucwords( $(this).val() ) );
            });
            
            $('#drivinglicence').blur( function() {
                $(this).val( $(this).val().toUpperCase() );
            });
            
            $('#address').blur( function() {
                $(this).val( ucwords( $(this).val() ) );
            });
            
            $('#apptsuite').blur( function() {
                $(this).val( $(this).val().toUpperCase() );
            });
            
            $('#city').blur( function() {
                $(this).val( ucwords( $(this).val() ) );
            });
            
            $('#province').change( function() {
               var currentSelectionIndex = $(this)[0].selectedIndex;
               
                if( currentSelectionIndex < 13 )
                {
                    $('#country').val( 'canada' ).trigger('change');
                }
                else
                {
                    $('#country').val( 'unitedstates' ).trigger('change');
                }
            });
            
            $('#country').change( function() {
               var currentValue = $(this).val();
               var currentProvinceSelectionIndex = $('#province')[0].selectedIndex;
               
                if( currentValue == 'canada' )
                {
                    if( currentProvinceSelectionIndex > 12 )
                    {
                        $('#province').val( 'quebec' ).trigger('change');
                    }
                    
                    $('#postalcode').unmask();
                    $('#postalcode').val( '' );
                    $('#postalcode').mask('a9a 9a9');
                }
                else
                {
                    if( currentProvinceSelectionIndex < 13 )
                    {
                        $('#province').val( 'ny' ).trigger('change');
                    }
                    
                    $('#postalcode').unmask();
                    $('#postalcode').val( '' );
                    $('#postalcode').mask('99999');
                }
            });
            
            $('#postalcode').blur( function() {
                if( $(this).val() != '' )
                {
                    $(this).val( $(this).val().toUpperCase() );
                }
            });
            
            if( $('#country').val() == 'canada' )
            {
                $('#postalcode').mask('a9a 9a9');
            }
            else
            {
                $('#postalcode').mask('99999');
            }
            
            $('#telhome').mask('999-999-9999? x99999');
            $('#telcell').mask('999-999-9999? x99999');
            $('#telother').mask('999-999-9999? x99999');
            
            
        },

        initializeNewClientButtons: function() {
            $('#openclientfile').click( function(event) {
                CLOUD.globalrecreatif.clientPage.validateNewClient();
            });

            $('#col1 input').keypress( function(e) {
                if( e.keyCode == 13 )
                {
                    $('#openclientfile').click();
                }
            });
            
            CLOUD.globalrecreatif.clientPage.applyMasks();

            CLOUD.globalrecreatif.clientPage.errors.errorArray = new Array( 'newclient' );
            CLOUD.globalrecreatif.clientPage.errors.errorArray['newclient']= new Array();
        },

        validateNewClient: function() {
            CLOUD.globalrecreatif.clientPage.errors.errorArray['newclient'].length = 0;

            CLOUD.globalrecreatif.clientPage.errors.lastFocus = null;

            var lastName = $('#lastname').val();
            var firstName = $('#firstname').val();
            var clientType = $('#clienttype').val();
            var drivingLicence = $('#drivinglicence').val();
            var enterpriseName = $('#enterprisename').val();
            var address = $('#address').val();
            var apptSuite = $('#apptsuite').val();
            var city = $('#city').val();
            var province = $('#province').val();
            var country = $('#country').val();
            var postalCode = $('#postalcode').mask();
            var email = $('#email').val();
            var telHome = $('#telhome').mask();
            var telCell = $('#telcell').mask();
            var telOther = $('#telother').mask();
            var notes = $('#notes').val();

            $('fieldset input').removeClass('error');

            if( lastName == '' )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque un nom.', '#lastname' );
                $('#lastname').addClass('error');
            }

            if( firstName == '' )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque un prénom.', '#firstname' );
                $('#firstname').addClass('error');
            }

            if( drivingLicence == '' )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque un numéro de permis de conduire.', '#drivinglicence' );
                $('#drivinglicence').addClass('error');
            }

            if( ( clientType == 1 ) && ( enterpriseName == '' ) )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque un nom d\'entreprise.', '#enterprisename' );
                $('#enterprisename').addClass('error');
            }
            else if( ( clientType == 0 ) && ( enterpriseName != '' ) )
            {
                $('#enterprisename').val('');
            }

            if( address == '' )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque une adresse.', '#address' );
                $('#address').addClass('error');
            }

            if( city == '' )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque une ville.', '#city' );
                $('#city').addClass('error');
            }

            if( postalCode == '' )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque un code postal.', '#postalcode' );
                $('#postalcode').addClass('error');
            }

            if( ( telHome == '' ) && ( telCell == '' ) && ( telOther == '' ) )
            {
                CLOUD.globalrecreatif.clientPage.errors.add( 'newclient', 'Il manque un numéro de téléphone.', '#telhome' );
                $('#telhome').addClass('error');
                $('#telcell').addClass('error');
                $('#telother').addClass('error');
            }

            if( CLOUD.globalrecreatif.clientPage.errors.count('newclient') > 0 )
            {
                CLOUD.globalrecreatif.clientPage.errors.display('newclient');
            }
            else
            {
                CLOUD.globalrecreatif.clientPage.submitNewClient();
            }
        },

        submitNewClient: function() {
            var dataToSubmit = new Object();
            
            dataToSubmit.lastName = $('#lastname').val();
            dataToSubmit.firstName = $('#firstname').val();
            dataToSubmit.clientType = $('#clienttype').val();
            dataToSubmit.drivingLicence = $('#drivinglicence').val();
            dataToSubmit.enterpriseName = $('#enterprisename').val();
            dataToSubmit.address = $('#address').val();
            dataToSubmit.apptSuite = $('#apptsuite').val();
            dataToSubmit.city = $('#city').val();
            dataToSubmit.province = $('#province').val();
            dataToSubmit.country = $('#country').val();
            dataToSubmit.postalCode = $('#postalcode').mask().toUpperCase().replace(' ', '');
            dataToSubmit.email = $('#email').val();
            dataToSubmit.telHome = $('#telhome').mask();
            dataToSubmit.telCell = $('#telcell').mask();
            dataToSubmit.telOther = $('#telother').mask();
            dataToSubmit.notes = $('#notes').val();

            $.ajax( '/ajax/client/nouveau', {
                data: dataToSubmit,
                success: function(data) {
                    var newClientId = parseInt( data.clientid );
                    
                    if( newClientId === 0 )
                    {
                        CLOUD.simpleDialog( 'Erreur', 'Une erreur inconnue s\'est produite.' );
                    }
                    else
                    {
                        // Redirect to the page of the newly created client.
                        CLOUD.simpleDialog( 'Ajout effectué', 'Le nouveau clien a bien été ajouté, redirection vers sa page client.', function() { document.location.href = '/client/' + newClientId; } );
                    }
                },
            });
        },
        
        loadTabData: function( tabIndex ) {
            // Which tab do we need to load the data for.
            switch( tabIndex )
            {
                // "Information" tab.
                case 0:
                    $.ajax( '/ajax/client/informations/' + $('#client-tabs').attr('clientid'), {
                        success: function(data) {
                            var clientData = data.clientdata;                        
                            
                            $('#lastname').val( clientData.nom );              
                            $('#firstname').val( clientData.prenom );
                            $('#clienttype').val( clientData.type );
                            $('#drivinglicence').val( clientData.no_permis_conduire );
                            $('#enterprisename').val( clientData.nom_entreprise );
                            $('#address').val( clientData.adresse );
                            $('#apptsuite').val( clientData.appt_suite );
                            $('#city').val( clientData.ville );
                            $('#province').val( clientData.province_etat );
                            $('#country').val( clientData.pays );
                            $('#postalcode').val( clientData.code_postal );
                            $('#email').val( clientData.email );
                            $('#telhome').val( clientData.tel_domicile );
                            $('#telcell').val( clientData.tel_cell );
                            $('#telother').val( clientData.tel_autre );
                            $('#notes').val( clientData.notes );
                            
                            CLOUD.globalrecreatif.clientPage.applyMasks();
                            
                            $('input').focus( function() {
                                CLOUD.globalrecreatif.clientPage.focusedInputBuffer = $(this).val();
                            });
                            
                            $('#lastname').blur( function() {
                                if( $(this).val() != CLOUD.globalrecreatif.clientPage.focusedInputBuffer )
                                {
                                    console.log('Update lastname info');
                                    CLOUD.animateUpdatedForm( this );
                                }
                            });
                        
                            $('#firstname').blur( function() {
                                console.log('Update firstname info');
                            });
                            
                            $('#clienttype').change( function() {
                                console.log('Put check on enterprise name update');
                            });
                            
                            $('#drivinglicence').blur( function() {
                                console.log('Update driving licence info');
                            });
                            
                            $('#enterprisename').blur( function() {
                                console.log('Update enterprise name info if client type is good');
                                
                                CLOUD.advancedDialog( 'Type de client', 'Vous n\'avez pas spécifié de nom d\'entreprise, désirez-vous que le type de client soit changé pour "Particulier"?', { 
                                    'Oui' : function() { $(this).dialog('close'); console.log('changer'); },
                                    'Non' : function() { $(this).dialog('close'); console.log('ne pas changer'); }
                                    });
                            });                           
                            

                        },
                    });
                    break;
                    
                // "Storage" tab.
                case 1:
                    $('#contrats tbody tr').addClass('clickable').each( function() {
                        
                        $(this).click( function() {
                            document.location.href = '/contrat/' + $(this).attr('contratid');
                        });
                    });
                    break;
            }
        }
    },
	
	contractPage: {
		initializeNewContract: function() {
			if( $('#newlotpicker').length > 0 )
			{
				$('#newlotform').hide();
			}
			
			$('#existinglot').change(function() {
				if( $(this).is(':checked') )
				{
					$('#newlotpicker select').focus();
					$('#newlotform').hide();
				}
			});
			
			$('#newlot').change(function() {
				if( $(this).is(':checked') )
				{
					$('#newlotform').show();
				}
				else
				{
					$('#newlotform').hide();
				}
			});
			
			$( '#checkindate' ).datepicker({
				changeMonth: true,
				changeYear: true,
				minDate: 0,
				numberOfMonths: 4,
				onSelect: function(dateText, inst) { 						
					CLOUD.globalrecreatif.contractPage.updateCheckoutEstimate();
				}
			});
			
			$('#selectedpackage').change( function() {
				var currentPackage = $('option:selected', this);
				
				// Make sure we didn't select "Other".
				if( currentPackage.val() != '-1' )
				{
					$('#numberofdays').val( currentPackage.attr('duration') );
					$('#totalprice').autoNumericSet( currentPackage.attr('price') );
					CLOUD.globalrecreatif.contractPage.updateCheckoutEstimate();
				}
			});
			
			$('#numberofdays').focus(function(){
				$(this).select();
			}).mouseup(function(e){e.preventDefault();});
			
			$('#numberofdays').keyup(function(){
				CLOUD.globalrecreatif.contractPage.updateCheckoutEstimate();
				$('#selectedpackage').val('-1');
			});
			
			$('#totalprice').autoNumeric({aSep: ' ', aDec: ',', aSign: ' $', pSign: 's', wEmpty: 'zero'});
			
			
			$('#totalprice').focus(function(){
				$(this).select();
			}).mouseup(function(e){e.preventDefault();});
			
			$('#totalprice').keyup(function(){
				$('#selectedpackage').val('-1');
			});

            $('#createcontractfile').click( function(event) {
                CLOUD.globalrecreatif.contractPage.validateNewContract();
            });
						
			CLOUD.globalrecreatif.contractPage.updateCheckoutEstimate();
		},
		
		validateNewContract: function() {
			var clientId = $('#contractdata').attr('clientid');
			
			CLOUD.errors.empty( 'newcontract' );
			
			var lotType = $('input[name=lottype]:checked').val();
			var lotId = 0;
			var lotDescription = '';
			var numberOfDays = 0;
			var totalPrice = 0;
			
			$('fieldset input').removeClass('error');
			
			if( lotType == 'existing' )
			{
				lotId = $('#existinglotid').val();
			}
			else if( lotType == 'new' || $('#newlotpicker').length === 0 )
			{
				// Validate new lot form.
				lotDescription = $('#vehicledescription').val();
				
				if( lotDescription == '' )
				{
					CLOUD.errors.add( 'newcontract', 'Il manque une description pour le lot.', '#vehicledescription' );
					$('#vehicledescription').addClass('error');
				}
			}
			else
			{
				CLOUD.errors.add( 'newcontract', 'Il faut choisir ou ajouter un lot.', '#existinglotid' );
			}
			
			numberOfDays = parseInt( $('#numberofdays').val(), 10 );
			totalPrice = $('#totalprice').autoNumericGet();
			
			if( isNaN( numberOfDays ) || numberOfDays < 1 )
			{
				CLOUD.errors.add( 'newcontract', 'Il faut que le contrat dure au moins un jour.', '#numberofdays' );
				$('#numberofdays').addClass('error');
			}
			
			if( CLOUD.errors.count('newcontract') > 0 )
            {
                CLOUD.errors.display( 'newcontract' );
            }
            else
            {
                var dataToSubmit = new Object();
				
				dataToSubmit.clientId = clientId;
				dataToSubmit.lotId = lotId;
				
				if( lotId === 0 )
				{
					dataToSubmit.lotDescription = lotDescription;
					dataToSubmit.lotYear = parseInt( $('#vehicleyear').val(), 10 );
					dataToSubmit.lotSerialNumber = $('#vehicleserialno').val();
					dataToSubmit.lotLicencePlate = $('#vehicleplate').val();
					dataToSubmit.lotLength = parseInt( $('#vehiclelength').val(), 10 );
					dataToSubmit.lotWidth = parseInt( $('#vehiclewidth').val(), 10 );
				}
				
				dataToSubmit.startDate = $('#checkindate').datepicker('getDate').getTime() / 1000;
				dataToSubmit.numberOfDays = numberOfDays;
				dataToSubmit.totalPrice = totalPrice;
				
				if( totalPrice == 0.0 )
				{
					CLOUD.advancedDialog( 'Contrat gratuit?', 'Vous ne chargez présentement aucun montant pour ce contrat, désirez-vous tout de même poursuivre?', { 
					'Oui' : function() { $(this).dialog('close'); CLOUD.globalrecreatif.contractPage.submitNewContract( dataToSubmit ); },
					'Non' : function() { $(this).dialog('close'); }
					});
				}
				else
				{
					CLOUD.globalrecreatif.contractPage.submitNewContract( dataToSubmit );
				}
            }
		},
		
		submitNewContract: function( dataToSubmit ) {
            $.ajax( '/ajax/contrat/nouveau', {
                data: dataToSubmit,
                success: function(data) {
                    var newInvoiceId = parseInt( data.invoiceid );
                    
                    if( newInvoiceId === 0 )
                    {
                        CLOUD.simpleDialog( 'Erreur', 'Une erreur inconnue s\'est produite.' );
                    }
                    else
                    {
                        // Redirect to the page of the newly created invoice.
                        CLOUD.simpleDialog( 'Contrat créé', 'Le nouveau contrat a bien été créé, redirection pour compléter la facture.', function() { document.location.href = '/facture/nouvelle/' + newInvoiceId; } );
                    }
                },
            });
        },
		
		updateCheckoutEstimate: function() {
			var numberOfDays = parseInt( $('#numberofdays').val(), 10 );
			var anticipatedCheckout = $('#checkindate').datepicker( 'getDate' );
				
			$('#checkindatetext').html( '<strong>' + $.datepicker.formatDate('dd/mm/yy', anticipatedCheckout) + '</strong> (' + $.datepicker.formatDate('DD d MM yy', anticipatedCheckout) + ')' );
						
			if( numberOfDays > 0 )
			{
				anticipatedCheckout.setDate( anticipatedCheckout.getDate() + numberOfDays );

				$('#estimatedend').html('Date de fin anticipée: <strong>' + $.datepicker.formatDate('dd/mm/yy', anticipatedCheckout) + '</strong> (' + $.datepicker.formatDate('DD d MM yy', anticipatedCheckout) + ')' );
			}
			else
			{
				$('#estimatedend').html('');
			}
		}
	},
	
	invoicePage: {
		initialize: function() {
			if( CLOUD.globalrecreatif.invoicePage.itemCount() != 0 )
			{
				$('.invoice_empty').hide();
			}
			else
			{
				$('#invoice_cancel').prop('disabled', true);
				$('#invoice_create').prop('disabled', true);
			}
			
			$('.invoice tbody tr input').live( 'focus', function(){
				$(this).select();
			}).live('mouseup', function(e){e.preventDefault();});			
			
			
			$('.invoice tbody tr input').live( 'blur', function(){
				CLOUD.globalrecreatif.invoicePage.updatePrice( $(this).parents( 'tr' ).index() );
			});
			
			$('.remove_item').live( 'click', function() {								
				$('input', $(this).parents( 'tr' )).val( 0 );

				CLOUD.globalrecreatif.invoicePage.updatePrice( $(this).parents( 'tr' ).index() );
				
				$(this).parents( 'tr' ).remove();
				
				if( CLOUD.globalrecreatif.invoicePage.itemCount() == 0 )
				{
					$('.invoice_empty').show();
					$('#invoice_cancel').prop('disabled', true);
					$('#invoice_create').prop('disabled', true);
				}
			});
			
			$('.minus_quantity').live( 'click', function() {				
				var target = $('input', $(this).parents( 'tr' ));
				var newQuantity = target.val() - 1;
				
				if( newQuantity < 0 )
				{
					newQuantity = 0;
				}
				
				target.val( newQuantity );
				CLOUD.globalrecreatif.invoicePage.updatePrice( $(this).parents( 'tr' ).index() );
				
				if( newQuantity == 0 )
				{
					var rowToRemove = $(this).parents( 'tr' );
					
					CLOUD.advancedDialog( 'Supprimer l\'article?', 'Vous avez indiqué une quantité de 0 pour cet article, désirez-vous plutôt le supprimer?', { 
					'Oui' : function() { 
						$(this).dialog('close'); 
						
						$('input', rowToRemove).val( 0 );

						CLOUD.globalrecreatif.invoicePage.updatePrice( rowToRemove.index() );
				
						rowToRemove.remove(); 
				
						if( CLOUD.globalrecreatif.invoicePage.itemCount() == 0 )
						{
							$('.invoice_empty').show();
							$('#invoice_cancel').prop('disabled', true);
							$('#invoice_create').prop('disabled', true);
						}
					},
					'Non' : function() { $(this).dialog('close'); }
					});
				}
			});
			
			$('.plus_quantity').live( 'click', function() {
				var target = $('input', $(this).parents( 'tr' ));	
				var newQuantity = parseFloat( target.val() ) + 1;
				
				target.val( newQuantity );

				CLOUD.globalrecreatif.invoicePage.updatePrice( $(this).parents( 'tr' ).index() );
			});
			
			$('#add_article').click( function(e) {
				CLOUD.advancedDialog( 'Ajouter un article', '<p><label for="articledesc">Descriptif de l\'article:</label> <input id="articledesc" type="text" size="30" /></p><p><label for="articleprice">Prix unitaire:</label> <input id="articleprice" type="text" size="8" /></p>', { 
				'Ajouter' : function() { 
					var articleDescription = $('#articledesc').val();
					var articlePrice = $('#articleprice').autoNumericGet();
					
					if( articleDescription === '' )
					{
						CLOUD.simpleDialog( 'Erreur', 'Il faut fournir une description pour l\'article.', function() { $('#articledesc').focus() } );
					}
					else
					{
						var newRow = '<tr><td class="center"><img class="remove_item" src="' + CLOUD.contentLocation + 'img/remove.png" /></td><td>' + articleDescription + '</td><td class="center"><img class="minus_quantity" src="' + CLOUD.contentLocation + 'img/minus.png" /> <input class="center" type="text" value="1" size="1" /> <img class="plus_quantity" src="' + CLOUD.contentLocation + 'img/add.png" /></td><td class="right" price="' + articlePrice + '">' + floatToCurrency( articlePrice ) + '</td><td class="right" price="' + articlePrice + '">' + floatToCurrency( articlePrice ) + '</td></tr>';
						
						$('.invoice tbody tr:last-child').after( newRow );
						
						CLOUD.globalrecreatif.invoicePage.updatePrice( $('.invoice tbody tr:last-child').index() );
						
						$(this).dialog('close');
						
						$('.invoice_empty').hide();
						$('#invoice_cancel').prop('disabled', false);
						$('#invoice_create').prop('disabled', false);
					}						
				},
				'Annuler' : function() { $(this).dialog('close'); }
				}, function() {  
				
					$('#articleprice').val('0,00 $'); 
					$('#articleprice').autoNumeric({aSep: ' ', aDec: ',', aSign: ' $', pSign: 's', wEmpty: 'zero'}); 
					
					$('input', this).focus(function(){
						$(this).select();
					}).mouseup(function(e){e.preventDefault();});
				});
			
				e.preventDefault();
			});
			
			$('#invoice_cancel').click( function() {
				CLOUD.globalrecreatif.invoicePage.completeInvoice( false );
			});
			
			$('#invoice_create').click( function() {
				CLOUD.globalrecreatif.invoicePage.completeInvoice( true );
			});
					
			CLOUD.globalrecreatif.invoicePage.updatePrice( $('.invoice tbody tr:last-child').index() );
		},
		
		updatePrice: function( index ) {
			var row = $('td', $('.invoice tbody tr').get( index ) );
			
			var quantity = 0;
			var quantityBox = $('input', $('.invoice tbody tr').get( index ) );
			
			if( quantityBox.length > 0 )
			{
				quantity = parseFloat( quantityBox.val() );
			}
			else
			{
				quantity = parseFloat( $(row[2]).html() );
			}

			var basePrice = parseFloat( $(row[3]).attr('price') );
			var totalPrice = basePrice * quantity;
			
			$(row[4]).attr('price', totalPrice);
			$(row[4]).html( floatToCurrency( totalPrice ) );
			
			// Update totals.
			var subTotal = 0.0;
			$('.invoice tbody tr').each( function() {
				if( $(this).index() !== 0 )
				{
					subTotal += parseFloat( $('td', this).eq(4).attr('price') );
				}
			});
			
			$('#invoice_subtotal').attr( 'subtotal', subTotal );
			$('#invoice_subtotal').html( floatToCurrency( subTotal ) );
			
			var subTotalCumulative = subTotal;
			
			var tax1 = $('#invoice_tax1');
			var tax1Amount = 0.00;
			var tax1Percentage = 0.00;
			var tax1Overtaxes = false;
			
			if( tax1.length > 0 )
			{
				tax1Percentage = parseFloat( tax1.attr('taxpercent') );
				tax1.attr('taxover') == '1' ? tax1Overtaxes = true : tax1Overtaxes = false;
				tax1Amount = ( tax1Overtaxes ? subTotalCumulative : subTotal ) * tax1Percentage;
				subTotalCumulative += tax1Amount;
			}
			
			$('#invoice_tax1').attr('tax1', tax1Amount);
			$('#invoice_tax1').html( floatToCurrency( tax1Amount ) );
			
			var tax2 = $('#invoice_tax2');
			var tax2Amount = 0.00;
			var tax2Percentage = 0.00;
			var tax2Overtaxes = false;
			
			if( tax2.length > 0 )
			{
				tax2Percentage = parseFloat( tax2.attr('taxpercent') );
				tax2.attr('taxover') == '1' ? tax2Overtaxes = true : tax2Overtaxes = false;
				tax2Amount = ( tax2Overtaxes ? subTotalCumulative : subTotal ) * tax2Percentage;
				subTotalCumulative += tax2Amount;
			}
			
			$('#invoice_tax2').attr('tax2', tax2Amount);
			$('#invoice_tax2').html( floatToCurrency( tax2Amount ) );
			
			$('#invoice_total').attr( 'total', subTotalCumulative );
			$('#invoice_total').html( floatToCurrency( subTotalCumulative ) );
		},
		
		completeInvoice: function( validInvoice ) {
						
			var basketItems = new Array();
			
			$('.invoice tbody tr').each( function() {
				if( $(this).index() !== 0 )
				{
					var basketItem = new Object();
					
					if( $('td', this).eq(1).attr('contractid') )
					{
						basketItem['description'] = $('td a', this).eq(0).html();
						basketItem['contractid'] = parseInt( $('td', this).eq(1).attr('contractid'), 10);
					}
					else
					{
						basketItem['description'] = $('td', this).eq(1).html();
						basketItem['contractid'] = 0;
					}
					
					if( $('td img', this).length > 0 )
					{
						basketItem['modifiable'] = 1;
					}
					else
					{
						basketItem['modifiable'] = 0;
					}
					
					basketItem['baseprice'] = parseFloat( $('td', this).eq(3).attr('price') );
					basketItem['totalprice'] = parseFloat( $('td', this).eq(4).attr('price') );
					
					var quantityBox = $('input', this);
		
					if( quantityBox.length > 0 )
					{
						basketItem['quantity'] = parseFloat( quantityBox.val() );
					}
					else
					{
						basketItem['quantity'] = parseFloat( $('td', this).eq(2).html() );
					}
					
					basketItems.push( basketItem );
				}
			});
			
			var dataToSubmit = new Object();
			
			dataToSubmit.tax1 = 0.00;
			dataToSubmit.tax2 = 0.00;
			
			dataToSubmit.invoiceId = parseInt( $('.invoice').eq(0).attr('invoiceid'), 10 );
			dataToSubmit.basketItems = basketItems;
			dataToSubmit.subtotal = parseFloat( $('#invoice_subtotal').attr( 'subtotal' ) );
			
			if( $('#invoice_tax1').length > 0 )
			{
				dataToSubmit.tax1 = parseFloat( $('#invoice_tax1').attr( 'tax1' ) );
			}
			
			if( $('#invoice_tax2').length > 0 )
			{
				dataToSubmit.tax2 = parseFloat( $('#invoice_tax2').attr( 'tax2' ) );
			}
			
			dataToSubmit.total = parseFloat( $('#invoice_total').attr( 'total' ) );
			dataToSubmit.validInvoice = validInvoice ? 1 : 0;
			
			CLOUD.globalrecreatif.invoicePage.submitInvoice( dataToSubmit );
		},
		
		submitInvoice: function( dataToSubmit ) {		
			$.ajax( '/ajax/facture', {
                data: dataToSubmit,
                success: function(data) {
                    var invoiceId = parseInt( data.invoiceid );
                    
                    if( invoiceId === 0 )
                    {
                        CLOUD.simpleDialog( 'Erreur', 'Une erreur inconnue s\'est produite.' );
                    }
                    else
                    {
                        if( data.validated === 1 )
						{					
							// Redirect to the page of the newly created invoice.
							CLOUD.simpleDialog( 'Facture crée', 'La facture a bien été créée et est maintenant considérée payée.', function() { document.location.href = '/facture/' + invoiceId; } );
						}
						else
						{
							// Redirect to the page of the newly created invoice.
							CLOUD.simpleDialog( 'Facture annulée', 'La facture a bien été annulée.', function() { document.location.href = '/facture/' + invoiceId; } );
						}
                    }
                }
            });
		},
		
		itemCount: function() {
			return ( $('.invoice tbody tr').length - 1 );
		}
	}
};

{if isset($NoIdGiven)}
    <p>Aucun ID de contrat spécifié.</p>
{elseif isset($ContractDoesNotExist)}
    <p>Aucun contrat avec le ID spécifié n'existe.</p>
{elseif isset($NoClientIdGiven)}
    <p>Vous devez spécifier un numéro de client valide pour le nouveau contrat.</p>
{elseif isset($NewContract)}
<h1>Création d'un contrat</h1>
<fieldset>
    <legend>Lot associé au contrat</legend>
    
	{if $ClientLots|@count gt 0}
	<div id="newlotpicker">
		<p><input type="radio" name="lottype" value="existing" id="existinglot"><label for="existinglot">Utiliser un lot existant: <select id="existinglotid">
			{foreach from=$ClientLots item=lot}
				<option value="{$lot.id_lot}">#{$lot.id_lot} - {$lot.description} {$lot.annee} - {$lot.longueur}' X {$lot.largeur}'</option>
			{/foreach}
		</select></label></p>
		<p><input type="radio" name="lottype" value="new" id="newlot"><label for="newlot">Enregistrer un nouveau lot pour ce client</label></p>
	</div>
	{else}
	<h4>Enregistrement d'un nouveau lot</h4>
	{/if}
	<div id="newlotform">
	    <p>
			<label for="vehicledescription">Description :</label> <input type="text" id="vehicledescription" name="vehicledescription" size="25" maxlength="45"> 
			<label for="vehicleyear">Année :</label> <select name="vehicleyear" id="vehicleyear">{html_options values=$AllowedYears output=$AllowedYears}</select> 
		</p>
		<p>
			<label for="vehicleserialno">Numéro de série :</label> <input type="text" id="vehicleserialno" name="vehicleserialno" size="35" maxlength="11"> 
			<label for="vehicleplate">Immatriculation :</label> <input type="text" id="vehicleplate" name="vehicleplate" size="25" maxlength="35"> 
		</p>
		<p>
			<label for="vehiclelength">Longueur en pieds : </label><select name="vehiclelength" id="vehiclelength">{html_options values=$AllowedLengths output=$AllowedLengths selected=15}</select><label for="vehiclewidth">Largeur en pieds :</label><select name="vehiclewidth" id="vehiclewidth">{html_options values=$AllowedWidths output=$AllowedWidths selected=8}</select>
		</p>
	</div>
</fieldset>

<fieldset id="contractdata" clientid="{$ClientId}">
    <legend>Information sur le contrat</legend>
	
	<p>Date de début : <span id="checkindatetext"></span></p>
	
	<div id="checkindate"></div> 
	
	<p>
		<label for="selectedpackage">Forfait: 
			<select id="selectedpackage">
				{foreach from=$PackageList item=forfait}
					<option value="{$forfait.id_forfait}" duration="{$forfait.nb_jours}" price="{$forfait.prix}">{$forfait.description} - {$forfait.prix} $</option>
				{/foreach}
					<option value="-1" selected="selected">Autre</option>
			</select>
		</label>
	</p>

	<p>
		<label for="numberofdays">Durée (en jours) :</label> <input type="text" id="numberofdays" name="numberofdays" size="6" maxlength="3" value="0"> 
		<span id="estimatedend"></span>
	</p>
	
	<p>
		<label for="totalprice">Prix (avant taxes) :</label> <input class="right" type="text" id="totalprice" name="totalprice" size="8" value="0.00 $"> 
	</p>
	
</fieldset>

<p class="right"><input id="createcontractfile" type="button" value="Créer le contrat et générer la facture" /></p>
{else} 
<fieldset>
    <legend>Informations sur le contrat #{$ContractId}</legend>
    
    <p>
        État : <strong><span{if $ContractStatus.statuscode eq 2} class="ongoing_time"{elseif $ContractStatus.statuscode eq 3} class="expired_time"{/if}>{$ContractStatus.statustext}{if $ContractStatus.statuscode eq 1}</span> ({$ContractStatus.daydifference} jour{if $ContractStatus.daydifference gt 1}s{/if} avant le début){elseif $ContractStatus.statuscode eq 2}</span> ({$ContractStatus.daydifference} jour{if $ContractStatus.daydifference gt 1}s{/if} restant{if $ContractStatus.daydifference gt 1}s{/if}){elseif $ContractStatus.statuscode eq 3} de {$ContractStatus.daydifferenceAbs} jour{if $ContractStatus.daydifferenceAbs gt 1}s{/if}</span>{else}</span>{/if}</strong> 
    </p>
    <p>
        Date de création: <strong>{$ContractData.date_creation|date_format:"%d/%m/%Y - %H:%M"}</strong>
    </p>
    <p>
        Date de début: <strong>{$ContractData.check_in_time|date_format:"%d/%m/%Y"}</strong>
    </p>
    <p>
        Durée prévue: <strong>{$ContractData.nb_jours} jours</strong>
    </p>
    <p>
        Sortie anticipée: <strong>{$ContractData.anticipated_checkout|date_format:"%d/%m/%Y"}</strong>
    </p>
</fieldset>

<fieldset>
    <legend>Informations sur le lot #{$ContractData.id_lot}</legend>
    
    <p>
       <strong>{$ContractData.description} {$ContractData.annee}</strong>
    </p>
    
    <p class="right"><a href="/lot/{$ContractData.id_lot}">Consulter la fiche du lot #{$ContractData.id_lot}</a></p>
</fieldset>

<fieldset>
    <legend>Informations sur le client #{$ContractData.id_client}</legend>
    
    <p>
       <strong>{$ContractData.prenom} {$ContractData.nom}</strong>
    </p>
    
    <p class="right"><a href="/client/{$ContractData.id_client}">Consulter la fiche du client #{$ContractData.id_client}</a></p>
</fieldset>

{/if}

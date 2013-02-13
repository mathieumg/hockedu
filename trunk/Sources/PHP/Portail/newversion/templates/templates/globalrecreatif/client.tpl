{if isset($NoIdGiven)}
    <p>Aucun ID de client spécifié.</p>
{elseif isset($ClientDoesNotExist)}
    <p>Aucun client avec le ID spécifié n'existe.</p>
{elseif isset($NewClient)}
{include file="globalrecreatif/client_informations.tpl"}
    
<p class="right"><input id="openclientfile" type="button" value="Ouvrir le dossier" /></p>
{else}    
    <div id="client-tabs" clientid="{$ClientId}">
        <ul>
            <li><a title="informations" href="{$CompleteUrl}ajax/client/informations"><span>Informations</span></a></li>
            <li><a title="entreposage" href="{$CompleteUrl}ajax/client/storage/{$ClientId}"><span>Entreposage</span></a></li>
            <li><a title="facturation" href="{$CompleteUrl}ajax/client/invoicing/{$ClientId}"><span>Vente et facturation</span></a></li>
            <li><a title="notes" href="{$CompleteUrl}ajax/client/notes/{$ClientId}"><span>Notes</span></a></li>
        </ul>
    </div>
{/if}

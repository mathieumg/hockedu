<h2>Entreposage client</h2>

<p class="right"><a href="/contrat/nouveau/{$ClientId}" class="imglink"><img src="{$ImagesURL}/add.png" /> <span>Nouveau contrat</span></a></p>

<table id="contrats">
    <caption>Liste des contrats d'entreposage</caption>
    <colgroup>
        <col width="10%" />
        <col width="25%" />
        <col width="15%" />
        <col width="15%" />
        <col width="15%" />
        <col width="20%" />
    </colgroup>
    <thead>
        <tr>
            <th># lot</th>
            <th>Description</th>
            <th>Début contrat</th>
            <th>Fin contrat</th>
            <th>Temps restant</th>
            <th>Statut</th>
        </tr>
    </thead>
    <tbody>
        {if $StorageList|@count neq 0}
        {foreach from=$StorageList item=stored}
        <tr contratid="{$stored.id_contrat}">
            <td>{$stored.id_lot}</td>
            <td>{$stored.description} {$stored.annee}</td>
            <td>{$stored.check_in_time|date_format:"%d/%m/%Y"}</td>
            <td>{$stored.anticipated_checkout|date_format:"%d/%m/%Y"}</td>
            <td{if $stored.status.code eq 2} class="ongoing_time"{elseif $stored.status.code eq 3} class="expired_time"{/if}>{if $stored.status.code eq 2}{$stored.day_difference} jours{elseif $stored.status.code eq 3}{$stored.day_difference} jours{/if}</td>
            <td>{$stored.status.text}</td>
        </tr>
        {/foreach}
        {else}
        <tr>
            <td colspan="6" class="center">Aucun contrat trouvé pour ce client.</td>
        </tr>
        {/if}
    </tbody>
</table>

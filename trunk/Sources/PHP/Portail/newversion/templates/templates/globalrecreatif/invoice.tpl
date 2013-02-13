{if isset($NoIdGiven)}
    <p>Aucun ID de facture spécifié.</p>
{elseif isset($InvoiceDoesNotExist)}
    <p>Aucune facture avec le ID spécifié n'existe.</p>
{elseif isset($NoPreInvoiceIdGiven)}
    <p>Vous devez spécifier un numéro de facture valide pour la nouvelle facture.</p>
{else} 

<h1>{if isset($IsPreInvoice)}Création d'une facture{else}Facture #{$InvoiceData.id_facture}{/if}{if $InvoiceData.statut eq 2} - <span class="invoice_paid">PAYÉE</span>{elseif $InvoiceData.statut eq 3} - <span class="invoice_canceled">ANNULÉE</span>{/if}</h1>

<address>
{if $InvoiceData.type eq 0}{$InvoiceData.prenom} {$InvoiceData.nom}{else}{$InvoiceData.nom_entreprise}{/if}<br />
{$InvoiceData.adresse}{if isset($InvoiceData.appt_suite)} appartement/suite {$InvoiceData.appt_suite}{/if}<br />
{$InvoiceData.ville}, {$InvoiceData.province_etat}<br />
{if $InvoiceData.pays eq "unitedstates"}U.S.A.{elseif $InvoiceData.pays eq "canada"}Canada{/if}<br />
{$InvoiceData.code_postal}<br />
{if isset($InvoiceData.tel_domicile)}{$InvoiceData.tel_domicile}{elseif isset($InvoiceData.tel_cell)}{$InvoiceData.tel_cell}{/if}
</address>

<p class="right">Facture émise le {$InvoiceData.date|date_format:"%d/%m/%Y"}</p>

<table class="invoice" invoiceid="{$InvoiceData.id_facture}">
    <colgroup>
        {if isset($IsPreInvoice)}<col width="10%" />{/if}
        <col width="{if isset($IsPreInvoice)}45{else}55{/if}%" />
        <col width="15%" />
        <col width="15%" />
		<col width="15%" />
    </colgroup>
    <thead>
        <tr>
            {if isset($IsPreInvoice)}<th></th>{/if}
            <th>Description</th>
            <th class="center">Quantité</th>
            <th class="right">Prix unitaire</th>
			<th class="right">Prix</th>
        </tr>
    </thead>
    <tbody>
		{if isset($IsPreInvoice)}
		<tr class="invoice_empty">
            <td colspan="5" class="center">Aucun article dans cette facture.</td>
        </tr>	
		{/if}		
		
		{if $InvoiceData.items|@count neq 0}
        {foreach from=$InvoiceData.items item=product}
		<tr>
            {if isset($IsPreInvoice)}<td class="center">{if $product.modifiable eq 1}<img class="remove_item" src="{$ImagesURL}/remove.png" />{/if}</td>{/if}
            <td{if $product.contractid neq 0} contractid="{$product.contractid}"{/if}>{if $product.contractid neq 0}<a href="/contrat/{$product.contractid}">{/if}{$product.description}{if $product.contractid neq 0}</a>{/if}</td>
            <td class="center">{if $product.modifiable eq 1 and isset($IsPreInvoice)}<img class="minus_quantity" src="{$ImagesURL}/minus.png" /> <input class="center" type="text" value="{$product.quantity}" size="1" /> <img class="plus_quantity" src="{$ImagesURL}/add.png" />{else}{$product.quantity}{/if}</td>
            <td class="right" price="{$product.baseprice}">{'%n'|money_format:$product.baseprice}</td>
			<td class="right" price="{$product.totalprice}">{'%n'|money_format:$product.totalprice}</td>
        </tr>
        {/foreach}
		{/if}
    </tbody>

</table>

<p>{if isset($IsPreInvoice)}<a href="" class="imglink" id="add_article"><img src="{$ImagesURL}/add.png" /> <span>Ajouter un article</span></a>{/if}</p>

<div class="right">

<p>Sous-total : <strong id="invoice_subtotal" subtotal="{$InvoiceData.soustotal}">{'%n'|money_format:$InvoiceData.soustotal}</strong></p>
{if $InvoiceData.tax1_name neq ""}<p>{$InvoiceData.tax1_name} ({$InvoiceData.tax1_percent} %) : <strong id="invoice_tax1" tax1="{$InvoiceData.taxl}" taxpercent="{$InvoiceData.tax1_percentcalc}" taxover="{$InvoiceData.tax1_overtaxes}">{'%n'|money_format:$InvoiceData.tax1}</strong></p>{/if}
{if $InvoiceData.tax2_name neq ""}<p>{$InvoiceData.tax2_name} ({$InvoiceData.tax2_percent} %) : <strong id="invoice_tax2" tax2="{$InvoiceData.tax2}" taxpercent="{$InvoiceData.tax2_percentcalc}" taxover="{$InvoiceData.tax2_overtaxes}">{'%n'|money_format:$InvoiceData.tax2}</strong></p>{/if}
<p>Total : <span id="invoice_total" total="{$InvoiceData.total}">{if $InvoiceData.statut eq 3}<del>{/if}{'%n'|money_format:$InvoiceData.total}{if $InvoiceData.statut eq 3}</del>{/if}</span></p>

{if isset($IsPreInvoice)}<p><input id="invoice_cancel" type="button" value="Annuler la facture" /> <input id="invoice_create" type="button" value="Compléter la facture" /></p>
{else}<p class="right"><a href="/facture/{$InvoiceData.id_facture}/pdf" class="imglink"><img src="{$ImagesURL}/pdf.gif" /> <span>Version PDF</span></a></p>{/if}

</div>

{/if}

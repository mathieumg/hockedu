<h1>Browse Fan Maps</h1>

<ul class="pages">
{foreach item=page from=$finalpages}
	{if $page neq $currentpage}
		<li><a href="{$SITE_URL}browsemaps-{$page}{if $sortmode neq ""}-{$sortmode}{/if}{if $sortorder neq ""}-{$sortorder}{/if}.html">{$page}</a></li>
	{else}
		<li class="currentpage">{$page}</li>
	{/if}
{foreachelse}
	<li class="currentpage">1</li>
{/foreach}
</ul>

<form action="browsemaps-{$currentpage}.html" method="post">
  <p><label for="sortparam">Sort by: </label>
    <select name="sortparam" id="sortparam" style="width: 150px">
     <option value="recent"{if $sortmode eq "" and $sortorder eq ""} selected="selected"{/if}>Most recent</option>
     <option value="rating"{if $sortmode eq "r"} selected="selected"{/if}>Best rating</option>
     <option value="namea"{if $sortmode eq "a" and $sortorder eq ""} selected="selected"{/if}>Author (A-Z)</option>
     <option value="namez"{if $sortmode eq "a" and $sortorder eq "d"} selected="selected"{/if}>Author (Z-A)</option>
    </select>
    <input type="submit" value="Ok" />
  </p>
</form>

<table width="100%">
	<thead>
		<tr>
			<th width="45%">Name</th>
			<th width="10%">Author</th>
			<th width="25%">Description</th>
			<th width="10%">Rating</th>
			<th width="15%">Download</th>
		</tr>
	</thead>
	<tfoot>
		<tr>
			<th>Name</th>
			<th>Author</th>
			<th>Description</th>
			<th>Rating</th>
			<th>Download</th>
		</tr>
	</tfoot>
	<tbody>
	{foreach item=maps from=$results}
		<tr>
			<td class="tdmapname"><a href="{$SITE_URL}reviewmaps-{$maps.id}.html">{$maps.name|truncate:25}</a></td>
			<td class="tdauthor">{$maps.author|sslashes|truncate:18}</td>
			<td class="tddescription">{$maps.description|sslashes|wordwrap:30:"<br />":true}</td>
			<td class="tdrating">{$maps.avgrating}</td>
			<td class="tddownload"><a href="{$SITE_URL}play-{$maps.id}.html">Play</a><br /><a href="{$SITE_URL}download-{$maps.id}.html">Get code</a></td>
		</tr>
	{foreachelse}
		<tr>
			<td colspan="4">No fan maps were found.</td>
		</tr>
	{/foreach}
	</tbody>
</table>

<ul class="pages">
{foreach item=page from=$finalpages}
	{if $page neq $currentpage}
		<li><a href="{$SITE_URL}browsemaps-{$page}{if $sortmode neq ""}-{$sortmode}{/if}{if $sortorder neq ""}-{$sortorder}{/if}.html">{$page}</a></li>
	{else}
		<li class="currentpage">{$page}</li>
	{/if}
{foreachelse}
	<li class="currentpage">1</li>
{/foreach}
</ul>


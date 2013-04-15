<h1>User maps</h1>

<table width="80%">
	<thead>
		<tr>
			<th width="20%">#</th>
			<th width="50%">Name</th>
			<th width="30%">Author</th>
		</tr>
	</thead>
	<tfoot>
		<tr>
			<th>#</th>
			<th>Name</th>
			<th>Author</th>
		</tr>
	</tfoot>
	<tbody>
	{foreach item=map from=$MapsList}
		<tr>
			<td>{$map.id}</td>
            <td>{$map.name}</td>
			<td>{$map.author}</td>
		</tr>
	{foreachelse}
		<tr>
			<td colspan="3">No maps were found.</td>
		</tr>
	{/foreach}
	</tbody>
</table>

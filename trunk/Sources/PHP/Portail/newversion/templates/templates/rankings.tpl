<h1>User rankings</h1>

<table width="50%">
	<thead>
		<tr>
			<th width="20%">Rank</th>
			<th width="50%">Username</th>
			<th width="30%">Matches won</th>
		</tr>
	</thead>
	<tfoot>
		<tr>
			<th>Rank</th>
			<th>Username</th>
			<th>Matches won</th>
		</tr>
	</tfoot>
	<tbody>
	{foreach item=user from=$UserRankings}
		<tr>
			<td>{$user@iteration}</td>
            <td><a href="/user/{$user.id}">{$user.username}</a></td>
			<td>{$user.win_count}</td>
		</tr>
	{foreachelse}
		<tr>
			<td colspan="2">No users were found.</td>
		</tr>
	{/foreach}
	</tbody>
</table>

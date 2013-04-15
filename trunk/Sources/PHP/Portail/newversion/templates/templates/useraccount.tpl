<h1>My account</h1>

<h2>My maps</h2>

<table width="80%">
	<thead>
		<tr>
			<th width="20%">#</th>
			<th width="70%">Name</th>
			<th width="10%">Public</th>
		</tr>
	</thead>
	<tfoot>
		<tr>
			<th>#</th>
			<th>Name</th>
			<th>Public</th>
		</tr>
	</tfoot>
	<tbody>
	{foreach item=map from=$MapsList}
		<tr>
			<td>{$map.id}</td>
            <td>{$map.name}</td>
			<td>{if $map.is_public eq 1}Yes{else}No{/if}</td>
		</tr>
	{foreachelse}
		<tr>
			<td colspan="3">No maps were found.</td>
		</tr>
	{/foreach}
	</tbody>
</table>
<br /><br />

<h2>My achievements</h2>

{foreach item=achievement from=$AchievementsList}
<div class="glossy achievement{if $achievement.unlocked eq 1}_completed{/if}">
    <div class="achievement_completion">
    {*
        <div class="achievement_completion_percentage">33%</div>
        <div class="achievement_completion_ratio">3/9 users</div>
        *}
    </div>
    <div>
        <div class="achievement_name">{$achievement.name}</div>
        <span class="achievement_description">{$achievement.description}</span>
    </div>
</div>
{foreachelse}
    <p>No achievements were found.</p>
{/foreach}

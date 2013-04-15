<h1>Gameplay achievements</h1>

{foreach item=achievement from=$AchievementsList}
<div class="glossy achievement">
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

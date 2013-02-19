<p><a href="/">Back to home</a></p>

{if isset($FormErrors)}
<p style="font-weight:bold">
The following error{if $FormErrors|@count gt 1}s{/if} happened while processing your request:
<ul>
	{foreach from=$FormErrors item=error}
    <li style="color:red;font-weight:bold">{$error}</li>
	{/foreach}
</ul>
</p>
{elseif isset($RegistrationSuccess)}
<p style="color:green;font-weight:bold">Registration successful!</p>
{/if}

{if !isset($RegistrationSuccess)}
<form method="post">
<p>
	<label for="username">Username : </label>
	<input type="text" id="username" name="username" />
</p>
<p>
	<label for="password">Password : </label>
	<input type="password" id="password" name="password" />
</p>
<p>
	<label for="passwordconf">Password confirmation : </label>
	<input type="password" id="passwordconf" name="passwordconf" />
</p>
<p>
	<label for="email">Email : </label>
	<input type="text" id="email" name="email" size="30" />
</p>
<p>
	<input type="submit" name="submit" value="TAKE THAT!" />
</p>
</form>
{/if}

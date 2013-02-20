<p><a href="/">Back to home</a></p>

{if isset($FormError) and $FormError eq true}
<p class="error"><strong>Some errors occured during the validation of the form, they are highlighted below.</strong></p>
{/if}

{if isset($FormCompleted)}
<p><strong>Registration completed! You have not received an email to activate your account because we couldn't care less about it, don't come complaining about your lost password, however.</strong></p>
{else}
<form method="post">
<p>
	<label for="username">Username : </label>
	<input{if isset($UsernameError)} class="highlightfield"{/if} type="text" id="username" value="{if isset($GoodUsername)}{$GoodUsername}{/if}" name="username" maxlength="30" /> <span class="mandatory">*</span>
	{if isset($UsernameError)}<br /><span class="error">{$UsernameError}</span>{/if}
</p>
<p>
	<label for="password">Password : </label>
	<input{if isset($PasswordError)} class="highlightfield"{/if} type="password" id="password" value="{if isset($GoodPassword)}{$GoodPassword}{/if}" name="password" /> <span class="mandatory">*</span>
	{if isset($PasswordError)}<br /><span class="error">{$PasswordError}</span>{/if}
</p>
<p>
	<label for="passwordconf">Password confirmation : </label>
	<input{if isset($PasswordConfirmationError)} class="highlightfield"{/if} type="password" id="passwordconf" value="{if isset($GoodPasswordConfirmation)}{$GoodPasswordConfirmation}{/if}" name="passwordconf" /> <span class="mandatory">*</span>
	{if isset($PasswordConfirmationError)}<br /><span class="error">{$PasswordConfirmationError}</span>{/if}
</p>
<p>
	<label for="email">Email : </label>
	<input{if isset($EmailError)} class="highlightfield"{/if} type="text" id="email" value="{if isset($GoodEmail)}{$GoodEmail}{/if}" name="email" size="30" maxlength="255" /> <span class="mandatory">*</span>
	{if isset($EmailError)}<br /><span class="error">{$EmailError}</span>{/if}
</p>
<p>
	<input type="submit" name="submit" value="Register" />
</p>
</form>
{/if}

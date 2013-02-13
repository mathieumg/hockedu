<h1>Remove a Fan Map</h1>

	<div class="itembox">
	
		{if $success neq ""}
			<h2>Map successfully removed!</h2>
			
			<p>{$success}</p>
		{else}
	
		<h2>Please enter the removal code associated with the map you want to remove.</h2>
		
		{if $error neq ""}
			<p class="error">{$error}</p>
		{/if}
		
		<form method="post" action="{$SITE_URL}removemaps.html">		
			<p>
				<label for="removalcode">Removal code :</label>
				<input type="text" name="removalcode" size="20" maxlength="255" />
			</p>
			<p>
				<img src="{$SITE_URL}captcha.png" /><br />
				<label for="captchacode">Please fill in what is shown on the above image :</label>
				<input type="text" name="captchacode" size="10" maxlength="10" />
			</p>
			<p>
				<input name="go" type="submit" value="Remove the map!" />
			</p>
		</form>
		{/if}
	</div>


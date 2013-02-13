<h1>Upload a Fan Map</h1>

	<div class="itembox">
	
		{if $success neq ""}
			<h2>Map successfully uploaded!</h2>
			
			<p>{$success}</p>
		{else}
	
		<h2>Please fill in the required fields</h2>
		
		{if $error neq ""}
			<p class="error">{$error}</p>
		{/if}
		
		<form method="post" action="{$SITE_URL}uploadmaps.html">		
			<p>
				<label for="authorname">Author Name :</label>
				<input type="text" name="authorname" value="{$authorname}" size="20" maxlength="255" /><sup>*</sup>
			</p>
			<p>
				<label for="authoremail">Author Email :</label>
				<input type="text" name="authoremail" value="{$authoremail}" size="30" maxlength="255" /> (If you want to receive the removal code, will not be stored)
			</p>
			<p>
				<label for="mapname">Map Name :</label>
				<input type="text" name="mapname" value="{$mapname}" size="35" maxlength="255" /><sup>*</sup>
			</p>
			<p>
				<label for="mapdescription">Map Description :</label>
				<input type="text" name="mapdescription" value="{$mapdescription}" size="45" /><sup>*</sup>
			</p>
			<p>
				<label for="mapcode">Map Code :</label>
				<input type="text" name="mapcode" value="{$mapcode}" size="55" /><sup>*</sup>
			</p>
			<p>
				<img src="{$SITE_URL}captcha.png" /><br />
				<label for="captchacode">Please fill in what is shown on the above image :</label>
				<input type="text" name="captchacode" size="10" maxlength="10" /><sup>*</sup>
			</p>
			<p>
				<input name="go" type="submit" value="Submit your map!" /> <input type="reset" value="Reset the form" />
			</p>
		</form>
		{/if}
	</div>


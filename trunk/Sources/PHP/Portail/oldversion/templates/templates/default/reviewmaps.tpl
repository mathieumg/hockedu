<h1>Reviews and rating for a Fan Map</h1>

{foreach item=maps from=$results}
	<h2>Map details</h2>
	<div class="itembox">
		<p><strong>Map name</strong>: {$maps.name}</p>
		<p><strong>Map author</strong>: {$maps.author}</p>
		<p><strong>Map rating</strong>: {$maps.avgrating} ({$maps.ratings_count} votes)</p>
		<p><strong>Map description</strong>: {$maps.description|wordwrap:60:"<br />":true}</p>
		<p><strong>Map code</strong>: <a href="{$SITE_URL}play-{$maps.id}.html">Play</a> || <a href="{$SITE_URL}download-{$maps.id}.html">Get</a></p>
    <p><strong>Map preview</strong>: <br \><object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=7,0,19,0" width="500" height="375" title="Smileys war">
    <param name="movie" value="http://www.smileys-war.com/flashfiles/previewmap.swf" />
    <param name="quality" value="high" />
    <param name="flashvars" value="mapcode_out={$maps.mapcode}">
    <embed src="http://www.smileys-war.com/flashfiles/previewmap.swf" flashvars="mapcode_out={$maps.mapcode}" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" width="500" height="375"></embed>
  </object></p>
  </div>
	
	<h2>Map reviews</h2>
	{foreach item=reviews from=$reviewsresults}
		<div class="itembox">
			<p>{$reviews.review|wordwrap:60:" ":true}</p>
			<p class="newsdetails">Submitted on: {$reviews.reviewdate|date_format:"%d/%m/%Y"} - Author: {$reviews.authorname} - Score: {$reviews.score}</p>
		</div>
	{foreachelse}
		<div class="itembox">There are no reviews for this map.</div>
	{/foreach}
	
	<h2>Rate and review the map!</h2>
	<div class="itembox">
	{if $success neq ""}
		<h3>Map successfully reviewed!</h3>
		
		<p>{$success}</p>
	{else}
		<h3>Please fill in the required fields</h2>
		
		{if $error neq ""}
			<p class="error">{$error}</p>
		{/if}
		
		<form method="post" action="{$SITE_URL}reviewmaps-{$maps.id}.html">		
			<p>
				<label for="authorname">Name :</label>
				<input type="text" name="authorname" value="{$authorname}" size="20" maxlength="60" /><sup>*</sup>
			</p>
			<p>
				<label for="mapscore">Score :</label>
				{if $mapscore neq ""}
					{$mapscore}<input type="hidden" name="mapscore" value="{$mapscore}" />
				{else}
					<select name="mapscore">
					   <option value="1">1</option>
					   <option value="2">2</option>
					   <option value="3">3</option>
					   <option value="4">4</option>
					   <option value="5" selected="selected">5</option>
					   <option value="6">6</option>
					   <option value="7">7</option>
					   <option value="8">8</option>
					   <option value="9">9</option>
					   <option value="10">10</option>
					</select><sup>*</sup>
				{/if}
			</p>
			<p>
				<label for="mapreview">Map review :</label><br />
				<textarea name="mapreview" cols="60" rows="5">{$mapreview}</textarea><sup>*</sup>
			</p>
			<p>
				<img src="{$SITE_URL}captcha.png" /><br />
				<label for="captchacode">Please fill in what is shown on the above image :</label>
				<input type="text" name="captchacode" size="10" maxlength="10" /><sup>*</sup>
			</p>
			<p>
				<input name="go" type="submit" value="Submit your review!" /> <input type="reset" value="Reset the form" />
			</p>
		</form>
	{/if}
	</div>
{foreachelse}
	<div class="itembox">This map does not exist.</div>
{/foreach}

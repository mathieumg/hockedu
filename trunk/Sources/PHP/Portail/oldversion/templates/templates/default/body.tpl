<body>
		<div id="logo"></div>
		<div id="container">
			{* Include the menu at the top *}
      {assign var="topmenu" value="true"}
			{include file="menu.tpl"}
      <div class="ad">
        <script type="text/javascript"><!--
  			google_ad_client = "pub-3923660808771546";
  			google_ad_width = 728;
  			google_ad_height = 90;
  			google_ad_format = "728x90_as";
  			google_ad_type = "text_image";
  			//2007-08-06: Jeux, Flash, Smileys, Action, Macromedia
  			google_ad_channel = "6394768316+4244309556+6625079485+7496481408+8156412032";
  			google_color_border = "344c28";
  			google_color_bg = "8dae55";
  			google_color_link = "000000";
  			google_color_text = "000000";
  			google_color_url = "80FF00";
  			google_ui_features = "rc:6";
  			//-->
  			</script>
  			<script type="text/javascript"
  			  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
  			</script>
      </div>
			<div id="content">
				{* Include the current page *}
				{include file="$TemplateToInclude"}
			</div>
			{* Include the menu at the bottom *}
      {assign var="topmenu" value="false"}
			{include file="menu.tpl"}
		</div>
		<p id="copyright">Interactive Pixels &copy; All rights reserved - 2007-2009</p>
		<div class="ad">
      <script type="text/javascript"><!--
			google_ad_client = "pub-3923660808771546";
			google_ad_width = 728;
			google_ad_height = 90;
			google_ad_format = "728x90_as";
			google_ad_type = "text_image";
			//2007-08-06: Jeux, Flash, Smileys, Action, Macromedia
			google_ad_channel = "6394768316+4244309556+6625079485+7496481408+8156412032";
			google_color_border = "000000";
			google_color_bg = "29331a";
			google_color_link = "FFFF66";
			google_color_text = "FFFFFF";
			google_color_url = "80FF00";
			google_ui_features = "rc:6";
			//-->
			</script>
			<script type="text/javascript"
			  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
			</script>
		</div>
</body>

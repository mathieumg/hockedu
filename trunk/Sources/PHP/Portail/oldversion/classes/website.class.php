<?php
/*
+------------------------------------------------------------------------------
|   Smileys-War
|   ==================================================
|   by
|     Louis-Simon Ménard
|     Mathieu M-Gosselin
|   (C) 2007 - Interactive-Pixels
+------------------------------------------------------------------------------
|   > classes/website.class.php : "website" class file
|   > Description: Mainframe of the website, manages every action related to the website itself
|   > Creation date: 15th August 2006
|   > Last modification date: 21st October 2006
|   > File author(s): mathieumg <mathieumg@qc-net.com>
+------------------------------------------------------------------------------
*/

class website
{
	// Properties
	
	/*
		float fSiteStartTime
			private property
		
		Designed to contain the precise time at which the page loading was initiated.
	*/
	private $fSiteStartTime;
	
	/*
		array mixed aConfig
			private property
		
		Contains a list of all the properties in the configuration file.
	*/
	private $aConfig;

	/*
		object Smarty oSmarty
			private property
		
		Instance of the Smarty template engine.
	*/
	private $oSmarty;
	
	/*
		array string aSiteModules
			private property
		
		Contains a list of the permitted pages and their respective permissions.
	*/
	private $aSiteModules;
	
	/*
		string sPageToInclude
			private property
		
		Contains the filename (without the extension) of the page to load.
	*/
	private $sPageToInclude;
	
	/*
		string sPageToInclude
			private property
		
		Contains the filename (without the extension) of the template to load.
	*/
	private $sTemplateToInclude;
	
	/*
		string sCurrentLanguage
			private property
		
		Contains the filename (without the extension) of the language to load.
	*/
	public $sCurrentLanguage;
	
	/*
		array string aLanguageStrings
			private property
		
		Contains all the translated text strings for the current language.
	*/
	private $aLanguageStrings;
	
	private $bShowTopBanner;
	
	private $sLeftMenuToInclude;
	
	private $sVisualPath;
	
	
	// Methods
	
	function __construct()
	{
		// Report all errors (warnings, criticals, etc.)
		//error_reporting(E_ALL);
		//ini_set('error_reporting', E_ALL);
		
		// Register the exact time at which the page loading was initiated.
		$this->fSiteStartTime = microtime();
		
		// Define security constant
		define("IN_SWAR", true);
	}
		
	/*
		boolean bLoadConfig(void)
			protected method
			
			@return boolean: If the configuration file loading went well or not.
			
		Load the website configuration from the approriate file.
	*/
	public function bLoadConfig()
	{
		// Does the configuration file exists?
		if( file_exists("config/config.inc.php") == true )
		{
			// Yes, load it and save it into the class
			require_once("config/config.inc.php");
			$this->aConfig = $aSettings;
			return true;
		}
		else
		{
			// Nope, doesn't exist!
			return false;
		}
	}
	
	/*
		void vLoadModules(void)
			public method
			
		Load main modules for the website (template engine, language files, etc.).
	*/
	public function vLoadModules()
	{
		// Start the session
		//session_start();
		
		// Initiate the template system
		require_once(SMARTY_DIR . "Smarty.class.php");
		$this->oSmarty = new Smarty;
		//$this->oSmarty->debugging = true;

		$this->oSmarty->template_dir = SMARTY_TEMPLATES_DIR . "templates/" . "default" . "/";
		$this->oSmarty->compile_dir = SMARTY_TEMPLATES_DIR . "templates_c/";
		$this->oSmarty->config_dir = SMARTY_TEMPLATES_DIR . "configs/";
		$this->oSmarty->cache_dir = SMARTY_TEMPLATES_DIR . "cache/";
    $this->oSmarty->register_modifier("sslashes", "stripslashes");
		
		// Load the language file
		//$this->vLoadLanguage();
		
		// Define permissions and what pages they are allowed to access - Flags are a: administrator, s: super moderator, m: moderator, u: user, g: guest
		$this->vPermsAddPage("home", "homepage", "asmug");
		$this->vPermsAddPage("features", "features", "asmug");
		$this->vPermsAddPage("play", "play", "asmug");
		$this->vPermsAddPage("mapeditor", "mapeditor", "asmug");
		$this->vPermsAddPage("custommaps", "custommaps", "asmug");
		$this->vPermsAddPage("browsemaps", "browsemaps", "asmug");
		$this->vPermsAddPage("reviewmaps", "reviewmaps", "asmug");
		$this->vPermsAddPage("uploadmaps", "uploadmaps", "asmug");
		$this->vPermsAddPage("removemaps", "removemaps", "asmug");
		$this->vPermsAddPage("support", "support", "asmug");
		$this->vPermsAddPage("about", "about", "asmug");
		$this->vPermsAddPage("403", "error403", "asmug");
		$this->vPermsAddPage("404", "error404", "asmug");
	}
	
	/*
		void vLoadLanguages(void)
			protected method
			
		Load the file for the current language. Also manage language changes and saving.
	*/
	protected function vLoadLanguage()
	{	
		//Asking for a language change?
		if( isset($_GET["newlang"]) == true )
		{
			// Yes, is it a valid language?		
			if( $this->vValidateLang($_GET["newlang"]) == true )
			{
				// Yes, save it and go back to the previous page or the homepage
				$this->setCurrentLanguage($_GET["newlang"]);
				
				// Previous page in memory and valid?
				if( isset($_SERVER["HTTP_REFERER"]) == true && strrpos($_SERVER["HTTP_REFERER"], SITE_URL) != false )
				{
					// Yes, go to it
					$this->vChangePage($_SERVER["HTTP_REFERER"]);
				}
				else
				{
					// No, go to the homepage
					$this->vChangePage(SITE_URL);
				}
			}
		}
		// Defined in a cookie?
		elseif( isset($_COOKIE["language"]) == true )
		{			
			// Yes, is it a valid language?	
			if( $this->vValidateLang($_COOKIE["language"]) == true )
			{
				//Yes, save it and update the cookie expiration date to 1 year later.
				$this->setCurrentLanguage($_COOKIE["language"]);
			}
		}
		//Defined in a session var?
		elseif( isset($_SESSION["language"]) == true )
		{
			// Yes, is it a valid language?	
			if( $this->vValidateLang($_SESSION["language"]) == true )
			{
				// Yes, save it.
				$this->setCurrentLanguage($_SESSION["language"]);
			}
		}
		// Undefined
		else
		{
			// Get languages preferences from the user's browser
			
			$iMatchingLanguages = 0; //Will store how many valid languages found
			
			
			if( isset($_SERVER["HTTP_ACCEPT_LANGUAGE"]) == true )
			{
				$aBrowserLanguages = explode(",", $_SERVER["HTTP_ACCEPT_LANGUAGE"]);
				
				// Go through each one of the languages to find the first one that is valid
				foreach( $aBrowserLanguages as $aBrowserLanguagesValue ) 
				{
					$sStrippedLanguageValue = strtolower( substr( chop( $aBrowserLanguagesValue ), 0, 2 ) );
					
					// Is it a valid language?	
					if( $this->vValidateLang($sStrippedLanguageValue) == true )
					{
						// Yes, save it.
						$this->setCurrentLanguage($sStrippedLanguageValue);
						$iMatchingLanguages++;
						break;
					}
				}
			}
			
			// Check if a language has finally been found or not
			if( $iMatchingLanguages == 0 )
			{
				// No language found, use the default language from the config file, is it a valid language?	
				if( $this->vValidateLang($this->aConfig["lang_default"]) == true )
				{
					// Yes, save it.
					$this->setCurrentLanguage($this->aConfig["lang_default"]);
				}
				else
				{
					// No, we ran out of languages to show, kill the site with a fatal error.
					die("We found out that /lang/" . $this->aConfig["lang_default"] . ".lang.php was inexistant or invalid.");
				}
			}
		}
		
		//Include the language file
		require_once(ROOT_DIR . "/lang/" . $this->sCurrentLanguage . ".lang.php");
		$this->aLanguageStrings = $aLangStrings;
	}
	
	/*
		void vChangePage(string $sUrl)
			public method
			
			@param sUrl: New URL to go to.
			
		Redirect the browser to the new URL.
	*/
	public function vChangePage($sUrl)
	{
		// Modify the template to show that we are changing page
		$this->sTemplateToInclude = "changingpage";
		
		// Redirect to the new page
		header("HTTP/1.1 301 Moved Permanently");
		header("Location: " . $sUrl);
	}
	
	/*
		void vValidateLang(string $sShortName)
			protected method
			
			@param sShortName: Short name of the language to check (usually 2 letters).
			
		Check if wheter the language in question can be used or not.
	*/
	protected function vValidateLang($sShortName)
	{
		// Check if the passed language is a valid language for this site
		if( array_key_exists($sShortName, $this->aConfig["allowed_languages"]) == true )
		{		
			//Yes, return true
			return true;
		}
		else
		{
			// No, return false
			return false;
		}
	}
	
	/*
		void setCurrentLanguage(string $sShortName)
			public method
			
			@param sShortName: Short name of the language to set (usually 2 letters).
			
		Set the language used on the website by this user.
	*/
	public function setCurrentLanguage($sShortName)
	{
		// Yes, save it in a cookie and in a session(will auto-update if already there)
		setcookie("language", $sShortName, time() + 31536000 );
		$_SESSION["language"] = $sShortName;
		
		// Set the current language to the new filename
		$this->sCurrentLanguage = $this->aConfig["allowed_languages"][$sShortName];
	}
	
	/*
		void vPermsAddPage(string $sName, string $sFileName, string $sPerms)
			private method
			
			@param sName: Name of the page to display.
			@param sFileName: Name of the php file located in the /includes/ folder.
			@param sPerms: Flags indicating who is allowed to access the page, and who is not.
			
		Load main modules for the website (template engine, language files, etc.).
	*/
	private function vPermsAddPage($sName, $sFileName, $sPerms)
	{
		$this->aSiteModules[$sName] = Array($sFileName, $sPerms);
	}
	
	/*
		void vStartSite(void)
			protected method
			
		Load the current page if the permission is granted.
	*/
	public function vStartSite()
	{
		// Has a page been asked for?
		if( isset($_GET["module"]) == true )
		{
			// Yes, is this a legit page, shall we load it? Do the check.
			$this->sPageToInclude = $this->sLegitPage($_GET["module"]);
		}
		else
		{
			// No, load the homepage.
			$this->sPageToInclude = "homepage";
		}
		
		// Default the template to the 404 error just in case it is not mentionned in the included file
		$this->sTemplateToInclude = "error404";
		
		// Set the banner to the default state
		//$this->bShowTopBanner = $this->aConfig["topbanner_show"];
		
		// Set the basepath for the current path displayer
		//$this->vAddVisualPath("Smileys War", SITE_URL);
		
		// Load the good page
		require_once(ROOT_DIR . "includes/" . $this->sPageToInclude . ".inc.php");
	}
	
	private function vAddVisualPath($sName, $sUrl)
	{
		// Is the path already set?
		if( isset($this->sVisualPath) == true )
		{
			// Yes, then add a bracket
			$this->sVisualPath .= " > ";
		}
		
		$this->sVisualPath .= "<a href=\"" . $sUrl . "\">" . $sName . "</a>";
	}

	private function vLanguageValues( )
	{
		// Get arguments passed to the function
		$aFunctionArgs = func_get_args();
		$sLanguageVar = $aFunctionArgs[0];
		unset($aFunctionArgs[0]);
		
		// Move the arguments array by 1 on the left
		$aStringValues = $this->aDecalArray($aFunctionArgs, -1);
		
		// Put the good values in the language string
		$this->aLanguageStrings[$sLanguageVar] = vsprintf($this->aLanguageStrings[$sLanguageVar], $aStringValues);
	}
	
	private function aDecalArray( $aArray, $iDecal )
	{
		// Decal the array elements
		foreach($aArray as $iKey => $mValue)
		{
			$aNewArray[$iKey + $iDecal] = $aArray[$iKey];
		}
		
		// Return the decaled array
		return $aNewArray;
	}
	
	/*
		void sLegitPage(string $sRequest)
			protected method
			
			@param sRequest: Name of the page the user requested to display.

			@return string: Name of the php file to load.
			
		Do some checks to see if the page requested exists (if not throw a 404 error) and if the user has enough permissions to display/access it (if not throw a permissions error).
	*/
	protected function sLegitPage($sRequest)
	{
			// Does the requested page exist in the list of allowed pages?
			if( isset($this->aSiteModules[$sRequest]) )
			{
				// Yes, does the user have enough permissions?
				if( $this->bPermsAllow($this->aSiteModules[$sRequest][1]) == true )
				{
					// Yes, return the page to include it.
					return $this->aSiteModules[$sRequest][0];
				}
				else
				{
					// No, return a permission error page.
					return "noperms";
				}
			}
			else
			{
				//No, return an error 404 (page not found)
				return "error404";
			}
	}
	
	/*
		boolean bPermsAllow(string $sFlags)
			privatemethod
			
			@param sFlags: Flags that indicates what pages can be accessed by whom.

			@return boolean: Wheter the user has enough permissions or not.
			
		Check if the current user has enough permissions to match the passed flags or not.
	*/
	private function bPermsAllow($sFlags)
	{
		if( isset($sFlags) == true )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/*
		void vDisplaySite()
			public method
			
		Display the website (ask the smarty engine to render the final output).
	*/
	public function vDisplaySite()
	{
		// Provide the template system with the site URL (for links and paths)
		$this->oSmarty->assign("SITE_URL", SITE_URL);
		
		// Modifiy the language selector depending on the number of languages
		
		/*
		$iLangCount = count($this->aConfig["allowed_languages"]);
		
		if( $iLangCount < 2 )
		{
			// Only one language, no need to display a selector
			$sLangSelectorCode = "";
		}
		elseif( $iLangCount == 2 )
		{
			// Two languages, find out what language is not the one currently used		
			foreach($this->aConfig["allowed_languages"] as $sCurrentLanguage) 
			{
				if($sCurrentLanguage != $this->sCurrentLanguage)
				{
					// We've got the language not currently used, render the menu
					require(ROOT_DIR . "/lang/" . $sCurrentLanguage . ".lang.php");
					$sLangSelectorCode = "<a href=\"language-" . $aLangStrings["Lng_ShortName"] . ".html\"><img src=\"img/language_" . $this->aLanguageStrings["Lng_ShortName"] . ".jpg\" alt=\"" . $aLangStrings["Lng_Name"] . "\" /></a>";
					break;
				}
			}
		}
		elseif( $iLangCount > 2 )
		{
			// More than 2 languages, make a drop-down menu to list them
			$sLangSelectorCode = "<select>";
			
			// Go through each language and default the current one
			foreach($this->aConfig["allowed_languages"] as $sCurrentLanguage) 
			{
					require(ROOT_DIR . "/lang/" . $sCurrentLanguage . ".lang.php");
					$sLangSelectorCode .= "<option value=\"" . $aLangStrings["Lng_ShortName"] . "\"";
					
					// Is it the current one?
					if($sCurrentLanguage == $this->sCurrentLanguage)
					{
						// Yes, make it the selected choice
						$sLangSelectorCode .= " selected=\"selected\"";
					}
					
					// Close the option tag
					$sLangSelectorCode .= ">" . $aLangStrings["Lng_Name"] . "</option>";
			}
			
			// Close the select tag
			$sLangSelectorCode .= "</select>";
		}
		
		$this->oSmarty->assign("LangSelectorCode", $sLangSelectorCode);
		
		// Indicate to the template engine if wether we have to show the top banner or not
		$this->oSmarty->assign("ShowTopBanner", $this->bShowTopBanner);
		
		// Show the good left menu
		$this->oSmarty->assign("LeftMenuInclude", "leftmenu/" . $this->sLeftMenuToInclude . ".tpl");
		*/
		
		// Transmit the current page path to the template engine
		$this->oSmarty->assign("CurrentPagePath", $this->sVisualPath);
		
		// Assign the name of the template that will be included in the main template (current page)
		$this->oSmarty->assign("TemplateToInclude", $this->sTemplateToInclude . ".tpl");
		
		// Compute the time it took to load the page
		//$this->vLanguageValues("LoadingTime", $this->getSiteLoadingTime());
	
		// Bring the language strings into the templates
		//$this->oSmarty->assign("lang", $this->aLanguageStrings);
		
		// Display the site
		$this->oSmarty->display("index.tpl");
	}
	
	/*
		float fTimeDiff(float $fTimeStart, float $fTimeEnd)
			private method
			
			@param fTimeStart: microtime(); of the shortest time.
			@param fTimeEnd: microtime(); of the longest time.
			
			@return float: Time difference between the shortest time and the longest one.
			
		Get the difference between two micro times.
	*/
	private function fTimeDiff($fTimeStart, $fTimeEnd)
	{
		// Split seconds and micro-seconds
		list($iMicro1, $iTime1) = explode(' ', $fTimeStart);
		list($iMicro2, $iTime2) = explode(' ', $fTimeEnd);
		
		// How many seconds between the two times
		$iTimeDiff = $iTime1 - $iTime2;
		
		// How many micro-seconds between the two
		if ($iMicro1 > $iMicro2)
		{
			// If the second micro-time is greater than the first one, there is an extra second we need to get rid of
			$iTimeDiff--;
			$iMicroDiff = 1 + $iMicro2 - $iMicro1;
		}
		else
		{
			// Else, we just make the difference
			$iMicroDiff = $iMicro2 - $iMicro1;
		}
		
		// Add seconds to micro-seconds and return
		return ($iMicroDiff + $iTimeDiff);
	}
	
	/*
		float getSiteLoadingTime(void)
			public method
			
			@return float: Site loading time.
			
		Retrieve the time it took to load the page.
	*/
	private function getSiteLoadingTime()
	{
		return $this->fTimeDiff($this->fSiteStartTime, microtime());
	}

}

?>

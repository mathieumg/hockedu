<!DOCTYPE html>
<html>
    <head>
        <title>Gestion Groupe Berpa - </title>
        <meta http-equiv="Content-Type" content="charset=utf-8" />
        <link href="{$StylesURL}common.css" media="screen" rel="stylesheet" type="text/css" />
        {if isset($Subsite)}<link href="{$StylesURL}{$Subsite}/style.css" media="screen" rel="stylesheet" type="text/css" />{/if}
        <link href="{$StylesURL}jquery-ui.css" media="screen" rel="stylesheet" type="text/css" />
    </head>

    <body>
        <div id="content" class="page_{$ModuleName}">
            {if isset($IsLoggedIn)}
            <div id="header">
                <div id="usermenu">
                    <select>
                        {foreach from=$AllowedSubsites key=address item=subsite}
                            <option value="{$address}"{if $address eq $Subsite} selected="selected"{/if}>{$subsite}</option>
                        {/foreach}
                    </select>
                    | <strong>{$UserInformation.prenom} {$UserInformation.nom}</strong>
                    | <a href="{$CompleteUrl}deconnexion">Déconnexion</a></div>
                <div><p><img src="{$ImagesURL}{$Subsite}/logo.png" /></p></div>
            </div>

            <div id="colmask">
                <div id="colmid">
                    <div id="colright">
                        <div id="col1wrap">
                            <div id="col1pad">
                                <div id="col1">
                                <!-- Column 1 start -->
                                {* Include the current page *}
                                {include file="$TemplateToInclude"}
                                <!-- Column 1 end -->
                                </div>
                            </div>
                        </div>
                        <div id="col2">
                            <!-- Column 2 start -->
                            {* Include the left menu for the current subsite *}
                            {include file="$Subsite/menu_left.tpl"}
                            <!-- Column 2 end -->
                        </div>
                        <div id="col3">
                            <!-- Column 3 start -->
                            <h2>Droite</h2>
                            <!-- Column 3 end -->
                        </div>
                    </div>
                </div>
            </div>

            <div id="footer">
                {* Include the footer *}
                {include file="footer.tpl"}
            </div>
            {else}
                {* Include the current page *}
                {include file="$TemplateToInclude"}
            {/if}
        </div>

        <script src="{$JavascriptURL}tools.js"></script>
        <script src="{$JavascriptURL}jquery.js"></script>
        <script src="{$JavascriptURL}jquery-ui.js"></script>
        <script src="{$JavascriptURL}jquery.maskedinput.js"></script>
		<script src="{$JavascriptURL}jquery.autonumeric.js"></script>
        <script src="{$JavascriptURL}common.js"></script>
        {if isset($Subsite)}<script src="{$JavascriptURL}{$Subsite}.js"></script>{/if}
    </body>
</html>

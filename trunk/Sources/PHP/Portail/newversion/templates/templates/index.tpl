<!DOCTYPE html>
<html>
    <head>
        <title>Hockedu</title>
        <meta http-equiv="Content-Type" content="charset=utf-8" />
		<link href="{$StylesURL}common.css" media="screen" rel="stylesheet" type="text/css" />
    </head>

    <body>
        <div id="topbar">{if isset($IsLoggedIn)}Logged in as: <a href="/user/{$UserInformation.id}"><strong>{$UserInformation.username}</strong></a> | <a href="/myaccount">My account</a> | <a href="/logout">Logout</a>{else}<a href="/login">Login</a> | <a href="/register">Register</a>{/if}</div>
        <div id="logo"></div>
        <div id="container">
            {* Include the menu at the top *}
            {assign var="topmenu" value="true"}
            {include file="menu.tpl"}
            <div id="content" class="page_{$ModuleName}">
                {* Include the current page *}
                {include file="$TemplateToInclude"}
            </div>
            {* Include the menu at the bottom *}
            {assign var="topmenu" value="false"}
            {* include file="menu.tpl" *}
        </div>
        <p id="copyright"><img src="{$ImagesURL}razorsharp_small.png" /><br /><br />Hockedu by RazorSharp Technologies &copy; All rights reserved - 2013</p>
    </body>
</html>

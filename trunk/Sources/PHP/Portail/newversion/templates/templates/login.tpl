<div id="loginbox">
    <fieldset> 
        <form action="" method="post">
            <p class="center"><img src="{$ImagesURL}{if isset($Subsite)}{$Subsite}/{/if}logo.png" /></p>
            <h2>Système de gestion{if isset($SubsiteName)} - {$SubsiteName}{/if}{if isset($BrowserTitle)} - {$BrowserTitle}{/if}</h2>
            <p>
                <label for="username">Nom d'utilisateur :</label>
                <input type="text" id="username" name="username" size="15"> 

                <label for="password">Mot de passe :</label>
                <input type="password" id="password" name="password" size="15"> 
            </p>
            {if !isset($Subsite)}
            <p>
                <label for="system">Système :</label>
                <select id="system" name="system">
                    {foreach from=$SubsitesList key=address item=subsite}
                        <option value="{$address}">{$subsite}</option>
                    {/foreach}
                </select>
            </p>
            {/if}
            <p class="center">
                <input type="submit" value="Connexion"> 
            </p>
        </form>
    </fieldset> 
</div>

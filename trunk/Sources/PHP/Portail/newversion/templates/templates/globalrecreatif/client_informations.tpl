<fieldset>
    <legend>Informations générales</legend>
    
    <p>
        <label for="lastname">Nom :</label> <input type="text" id="lastname" name="lastname" size="25" maxlength="255"> 
        <label for="firstname">Prénom :</label> <input type="text" id="firstname" name="firstname" size="25" maxlength="255"> 
        <label for="clienttype">Type de client:</label> <select id="clienttype" name="clienttype" ><option value="0">Particulier</option><option value="1">Entreprise</option></select>
    </p>
    <p>
        <label for="drivinglicence">No. permis de conduire :</label> <input type="text" id="drivinglicence" name="drivinglicence" size="25" maxlength="11"> 
        <label for="enterprisename">Nom de l'entreprise :</label> <input type="text" id="enterprisename" name="enterprisename" size="25" maxlength="255"> 
    </p>
</fieldset>

<fieldset>
    <legend>Coordonnées</legend>
    
    <p>
        <label for="address">Adresse :</label> <input type="text" id="address" name="address" size="35" maxlength="255"> 
        <label for="apptsuite">Appartement/suite :</label> <input type="text" id="apptsuite" name="apptsuite" size="5" maxlength="5"> 
        <label for="city">Ville :</label> <input type="text" id="city" name="city" size="20" maxlength="255"> 
    </p>
    <p>
        <label for="province">Province :</label>
        <select id="province" name="province" >
            <option value="quebec">Québec</option>
            <option value="ontario">Ontario</option>
            <option value="alberta">Alberta</option>
            <option value="britishcolumbia">Colombie-Britannique</option>
            <option value="pei">Île-du-prince-Édouard</option>
            <option value="manitoba">Manitoba</option>
            <option value="newbrunswick">Nouveau Brunswick</option>
            <option value="novascotia">Nouvelle-Écosse</option>
            <option value="nunavut">Nunavut</option>
            <option value="saskatchewan">Saskatchewan</option>
            <option value="newfoundland">Terre-Neuve</option>
            <option value="northwestterritories">Territoires du Nord-Ouest</option>
            <option value="yukon">Yukon</option>
            <option value="al">Alabama</option> 
            <option value="ak">Alaska</option> 
            <option value="az">Arizona</option> 
            <option value="ar">Arkansas</option> 
            <option value="ca">California</option> 
            <option value="co">Colorado</option> 
            <option value="ct">Connecticut</option> 
            <option value="de">Delaware</option> 
            <option value="dc">District Of Columbia</option> 
            <option value="fl">Florida</option> 
            <option value="ga">Georgia</option> 
            <option value="hi">Hawaii</option> 
            <option value="id">Idaho</option> 
            <option value="il">Illinois</option> 
            <option value="in">Indiana</option> 
            <option value="ia">Iowa</option> 
            <option value="ks">Kansas</option> 
            <option value="ky">Kentucky</option> 
            <option value="la">Louisiana</option> 
            <option value="me">Maine</option> 
            <option value="md">Maryland</option> 
            <option value="ma">Massachusetts</option> 
            <option value="mi">Michigan</option> 
            <option value="mn">Minnesota</option> 
            <option value="ms">Mississippi</option> 
            <option value="mo">Missouri</option> 
            <option value="mt">Montana</option> 
            <option value="ne">Nebraska</option> 
            <option value="nv">Nevada</option> 
            <option value="nh">New Hampshire</option> 
            <option value="nj">New Jersey</option> 
            <option value="nm">New Mexico</option> 
            <option value="ny">New York</option> 
            <option value="nc">North Carolina</option> 
            <option value="nd">North Dakota</option> 
            <option value="oh">Ohio</option> 
            <option value="ok">Oklahoma</option> 
            <option value="or">Oregon</option> 
            <option value="pa">Pennsylvania</option> 
            <option value="ri">Rhode Island</option> 
            <option value="sc">South Carolina</option> 
            <option value="sd">South Dakota</option> 
            <option value="tn">Tennessee</option> 
            <option value="tx">Texas</option> 
            <option value="ut">Utah</option> 
            <option value="vt">Vermont</option> 
            <option value="va">Virginia</option> 
            <option value="wa">Washington</option> 
            <option value="wv">West Virginia</option> 
            <option value="wi">Wisconsin</option> 
            <option value="wy">Wyoming</option>
        </select>
        
        <label for="country">Pays :</label>
        <select id="country" name="country" >
            <option value="canada">Canada</option>
            <option value="unitedstates">États-Unis</option>
        </select>
        <label for="postalcode">Code postal :</label> <input type="text" id="postalcode" name="postalcode" size="10" maxlength="10"> 
    </p>
    <p>
        <label for="telhome">Téléphone (domicile) :</label> <input type="text" id="telhome" name="telhome" size="15" maxlength="30"> 
        <label for="email">Courriel :</label> <input type="text" id="email" name="email" size="35" maxlength="255"> 
    </p>
    <p>
        <label for="telcell">Téléphone (mobile) :</label> <input type="text" id="telcell" name="telcell" size="15" maxlength="30"> 
    </p>
    <p>
        <label for="telother">Téléphone (autre) :</label> <input type="text" id="telother" name="telother" size="15" maxlength="30"> 
    </p>
</fieldset>

<fieldset>
    <legend><label for="notes">Notes</label></legend>
    
    <textarea id="notes" name="notes" cols="70" rows="10"></textarea>
</fieldset>

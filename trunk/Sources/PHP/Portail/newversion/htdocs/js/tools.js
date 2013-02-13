// Supplement some native javascript objects.
function prototypeExtensions() {
	String.prototype.replaceAt=function(index, char) {
		return this.substr(0, index) + char + this.substr(index+char.length);
	}
}

function ucfirst (str) {
    // Makes a string's first character uppercase  
    // 
    // version: 1109.2015
    // discuss at: http://phpjs.org/functions/ucfirst
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   bugfixed by: Onno Marsman
    // +   improved by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: ucfirst('kevin van zonneveld');
    // *     returns 1: 'Kevin van zonneveld'
    str += '';
    var f = str.charAt(0).toUpperCase();
    return f + str.substr(1);
}

function ucwords (str) {
    // Uppercase the first character of every word in a string  
    // 
    // version: 1109.2015
    // discuss at: http://phpjs.org/functions/ucwords
    // +   original by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
    // +   improved by: Waldo Malqui Silva
    // +   bugfixed by: Onno Marsman
    // +   improved by: Robin
    // +      input by: James (http://www.james-bell.co.uk/)
    // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // *     example 1: ucwords('kevin van  zonneveld');
    // *     returns 1: 'Kevin Van  Zonneveld'
    // *     example 2: ucwords('HELLO WORLD');
    // *     returns 2: 'HELLO WORLD'
    //return (str + '').replace(/^([a-z])[a-z]{3,}|\s+([a-z])/g, function ($1) {       
    return (str + '').replace(/^([a-z])|\s+([a-z])/g, function ($1) {       
        return $1.toUpperCase();
    });
}

function floatToCurrency( amount )
{
	if( amount === '' || amount === null || amount === 'undefined' )
	{
		return  '0,00 $';
	}
	
	amount = parseFloat( amount ).toFixed(2);
	
	var formattedAmountSrc = amount.toString();
	var formattedAmountDst = new String();
	var splittedAmount = formattedAmountSrc.split('.');
	var currentPosition = 0;
	
	for( var i = 0; i < splittedAmount[0].length; i++ )
	{
		formattedAmountDst += splittedAmount[0].charAt( i );
		
		currentPosition = formattedAmountSrc.length - 1 - i;
		
		if( ( currentPosition != 3 ) && ( currentPosition % 3 == 0 ) )
		{
			formattedAmountDst += ' ';
		}
	}
	
	formattedAmountDst += ',';
	formattedAmountDst += splittedAmount[1];
	formattedAmountDst += ' $';
	
    return formattedAmountDst;
}

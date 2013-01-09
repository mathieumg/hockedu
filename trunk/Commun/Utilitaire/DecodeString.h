//////////////////////////////////////////////////////////////////////////////
/// @file DecodeString.h
/// @author Michael Ferris
/// @date 2011-08-14
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstring>
#include <string>
#include <limits.h>
#include <stdexcept>

class NumberFormatException: public std::logic_error
{
public:
	NumberFormatException(std::string erreur):std::logic_error(erreur){}

};

enum formating{NONE,SPACE,COMMA};

///////////////////////////////////////////////////////////////////////////
/// @class DecodeString
/// @brief Utilitaire de convertion de string en int et vice-versa
///			Algorithme provenant de la librairie standard de Java
///
/// @author Michael Ferris
/// @date 2011-08-14
///////////////////////////////////////////////////////////////////////////
class DecodeString
{
public:
	static int decode(std::string nm)
	{
		int radix = 10;
		int index = 0;
		bool negative = false;
		int result;
		std::string startswith;

		startswith = nm;

		if (nm.length() == 0)
			throw NumberFormatException("Zero length string");
		char firstChar = nm[0];
		// Handle sign, if present
		if (firstChar == '-') {
			negative = true;
			index++;
			startswith.erase(0,1);
		} 
		else if (firstChar == '+')
		{
			index++;
			startswith.erase(0,1);
		}
		// Handle radix specifier, if present
		int size = (int)nm.length();
		if(!strncmp("0x",startswith.c_str(),2) || !strncmp("0X",startswith.c_str(),2) )
		{
			index += 2;
			radix = 16;
			startswith.erase(0,2);
		} else if (!strncmp("#",startswith.c_str(),1)) {
			index++;
			radix = 16;
			startswith.erase(0,1);
		} else if (!strncmp("0",startswith.c_str(),1) && size > 1 + index) {
			index++;
			radix = 8;
			startswith.erase(0,1);
		}
		if (!strncmp("-",startswith.c_str(),1) || !strncmp("+",startswith.c_str(),1))
			throw new NumberFormatException(
			"Sign character in wrong position");
		try {
			result = valueOf(startswith, radix);
			result = negative ? -result	: result;
		} catch (NumberFormatException e) {
			// If number is Integer.MIN_VALUE, we'll end up here. The next line
			// handles this case, and causes any genuine format error to be
			// rethrown.
			std::string constant = negative ? ("-" + startswith)
				: startswith;
			result = valueOf(constant, radix);
		}
		return result;
	}
	static int getDigit( char s, int radix ) 
	{
		int result = 0;
		char temp = toupper(s);

		if(s>'9' && temp <= 'F' && radix == 16)
		{
			temp -= 'A';
			temp += 10;
			result = (int)temp;
		}
		else if(s >= '0' && s <= '9')
		{
			s -= 48;
			result = (int)s;
		}
		else
			throw NumberFormatException("Character does not represent a number");
		return result;
	}
	static std::string toString(long long integer, formating format)
	{
		std::string result;
		bool negative=false;
		if(integer <0)
			negative = true;
		long long leftTo = integer;
		long long temp;

		if(integer == 0)
			return "0";

		int i=0;
		while(negative ? leftTo<0 : leftTo>0)
		{
			temp = leftTo / 10;
			temp = leftTo - (temp*10);
			leftTo -= temp;
			leftTo /= 10;
			temp = negative ? -temp : temp;

			char digit = '0'+(int)temp;
			std::string stringTemp;
			stringTemp.push_back(digit);
			if(format != NONE)
			{
				if(i%3 == 0 && i != 0)
				{
					if(format == SPACE)
						stringTemp.push_back(' ');
					else if(format == COMMA)
						stringTemp.push_back(',');
				}
			}

			stringTemp += result;
			result = stringTemp;
			i++;
		}

		result = negative ? "-"+result : result;

		return result;
	}
	static std::string toString(long long integer)
	{
		std::string result;
		bool negative=false;
		if(integer <0)
			negative = true;
		long long leftTo = integer;
		long long temp;

		if(integer == 0)
			return "0";

		int i=0;
		while(negative ? leftTo<0 : leftTo>0)
		{
			temp = leftTo / 10;
			temp = leftTo - (temp*10);
			leftTo -= temp;
			leftTo /= 10;
			temp = negative ? -temp : temp;

			char digit = '0'+(int)temp;
			std::string stringTemp;
			stringTemp.push_back(digit);
			stringTemp += result;
			result = stringTemp;
			i++;
		}

		result = negative ? "-"+result : result;

		return result;
	}
	static std::string toString(float floating_num,int digits)
	{
		std::string result;
		result += toString((int)floating_num);
		result += ".";
		int i = 0;
		int temp;
		for(; i<digits; i++)
		{
			temp = (int)(floating_num*powl(10,i+1));
			temp = temp%10;
			result += toString(temp);
		}
		int temp2 = (int)(floating_num*powl(10,i+1));
		temp2 = temp2%10;
		if(temp2 >= 5)
		{
			result.pop_back();
			result += toString(temp+1);
		}


		return result;
	}

private:
	static int valueOf(std::string s, int radix)
	{
		return parseInt(s, radix);
	}
	static int parseInt(std::string s, int radix)
	{
		if (s.length() == 0){
			throw NumberFormatException("Zero length string");
		}

		if (radix < 2) {
			std::string erreur = "radix";
			erreur+= radix+48;
			erreur+=" less than Character.MIN_RADIX";
			throw NumberFormatException(erreur);
		}

		if (radix > 16) {
			std::string erreur = "radix";
			erreur+= radix+48;
			erreur+=" greater than Character.MAX_RADIX";
			throw NumberFormatException(erreur);
		}

		int result = 0;
		bool negative = false;
		int i = 0, len = (int)s.length();
		int limit = -INT_MAX;
		int multmin;
		int digit;

		if (len > 0) {
			char firstChar = s[0];
			if (firstChar < '0') { // Possible leading "+" or "-"
				if (firstChar == '-') {
					negative = true;
					limit = INT_MIN;
				} else if (firstChar != '+')
					throw NumberFormatException("Wrong format of string");

				if (len == 1) // Cannot have lone "+" or "-"
					throw NumberFormatException("Wrong format of string");
				i++;
			}
			multmin = limit / radix;
			while (i < len) {
				// Accumulating negatively avoids surprises near MAX_VALUE
				digit = getDigit(s[i],radix);
				if (digit < 0) {
					throw NumberFormatException("Wrong format of string");
				}
				if (result < multmin) {
					//throw NumberFormatException("Wrong format of string");
				}
				result *= radix;
				if (result < limit + digit) {
					throw NumberFormatException("Wrong format of string");
				}
				result -= digit;
				i++;
			}
		} else {
			throw NumberFormatException("Wrong format of string");
		}
		return negative ? result : -result;
	}

};


/**
0859             * Decodes a {@code String} into an {@code Integer}.
0860             * Accepts decimal, hexadecimal, and octal numbers given
0861             * by the following grammar:
0862             *
0863             * <blockquote>
0864             * <dl>
0865             * <dt><i>DecodableString:</i>
0866             * <dd><i>Sign<sub>opt</sub> DecimalNumeral</i>
0867             * <dd><i>Sign<sub>opt</sub></i> {@code 0x} <i>HexDigits</i>
0868             * <dd><i>Sign<sub>opt</sub></i> {@code 0X} <i>HexDigits</i>
0869             * <dd><i>Sign<sub>opt</sub></i> {@code #} <i>HexDigits</i>
0870             * <dd><i>Sign<sub>opt</sub></i> {@code 0} <i>OctalDigits</i>
0871             * <p>
0872             * <dt><i>Sign:</i>
0873             * <dd>{@code -}
0874             * <dd>{@code +}
0875             * </dl>
0876             * </blockquote>
0877             *
0878             * <i>DecimalNumeral</i>, <i>HexDigits</i>, and <i>OctalDigits</i>
0879             * are defined in <a href="http://java.sun.com/docs/books/jls/second_edition/html/lexical.doc.html#48282">&sect;3.10.1</a> 
0880             * of the <a href="http://java.sun.com/docs/books/jls/html/">Java 
0881             * Language Specification</a>.
0882             * 
0883             * <p>The sequence of characters following an optional
0884             * sign and/or radix specifier ("{@code 0x}", "{@code 0X}",
0885             * "{@code #}", or leading zero) is parsed as by the {@code
0886             * Integer.parseInt} method with the indicated radix (10, 16, or
0887             * 8).  This sequence of characters must represent a positive
0888             * value or a {@link NumberFormatException} will be thrown.  The
0889             * result is negated if first character of the specified {@code
0890             * String} is the minus sign.  No whitespace characters are
0891             * permitted in the {@code String}.
0892             *
0893             * @param     nm the {@code String} to decode.
0894             * @return    an {@code Integer} object holding the {@code int}
0895             *		   value represented by {@code nm}
0896             * @exception NumberFormatException  if the {@code String} does not
0897             *            contain a parsable integer.
0898             * @see java.lang.Integer#parseInt(java.lang.String, int)
0899             */
/*
public static Integer decode(String nm)
throws NumberFormatException {
int radix = 10;
int index = 0;
boolean negative = false;
Integer result;
0906
if (nm.length() == 0)
throw new NumberFormatException("Zero length string");
char firstChar = nm.charAt(0);
// Handle sign, if present
if (firstChar == '-') {
negative = true;
index++;
} else if (firstChar == '+')
index++;
0916
// Handle radix specifier, if present
if (nm.startsWith("0x", index) || nm.startsWith("0X", index)) {
index += 2;
radix = 16;
} else if (nm.startsWith("#", index)) {
index++;
radix = 16;
} else if (nm.startsWith("0", index) && nm.length() > 1 + index) {
index++;
radix = 8;
}
0928
if (nm.startsWith("-", index) || nm.startsWith("+", index))
throw new NumberFormatException(
"Sign character in wrong position");
0932
try {
result = Integer.valueOf(nm.substring(index), radix);
result = negative ? new Integer(-result.intValue())
: result;
} catch (NumberFormatException e) {
// If number is Integer.MIN_VALUE, we'll end up here. The next line
// handles this case, and causes any genuine format error to be
// rethrown.
String constant = negative ? ("-" + nm.substring(index))
: nm.substring(index);
result = Integer.valueOf(constant, radix);
}
return result;
}*/
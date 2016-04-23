// stringFuncs.hpp
// utility functions for strings
#ifndef STRING_FUNCS_HPP
#define STRING_FUNCS_HPP

#include <string>
#include <tuple>
#include <vector>

namespace strfn
	{
		/* check if a string contains a phrase */
		std::pair<unsigned int, bool> doesStringContain(const std::string &text, const std::string &phrase);

		/* find and return a phrase starting at a certain key, and going until endword / end of phrase */
		std::string findStringKeyword(const std::string &text, const std::string &phrasem, const char endWord = ' ');

		/* split the string into two at a certain character */
		std::pair<std::string, std::string> splitString(const std::string &text, const char splitChar);

		/* converts a string into an vector at each word OR each special character */
		std::vector<std::string> stringToArray(const std::string &text, const char splitChar = ' ');

		/* finds all data inside a certain phrase with start and end points for parsing
		   If entered (player{pos}, player, {, }) as arguments, it will return 'pos'*/
		std::string findDataInside(const std::string &text, const std::string &phrase, const char start = '{', const char end = '}');
	}

#endif // !STRING_FUNCS_HPP

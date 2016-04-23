#include "strFuncs.hpp"
#include <algorithm>

std::pair<unsigned int, bool> strfn::doesStringContain(const std::string &text, const std::string &phrase)
	{
		if (!phrase.empty() && !text.empty()) 
			{
				unsigned int startPos = 0;

				std::string checkText = text;
				std::transform(checkText.begin(), checkText.end(), checkText.begin(), ::tolower);

				std::string checkPhrase = phrase;
				std::transform(checkPhrase.begin(), checkPhrase.end(), checkPhrase.begin(), ::tolower);

				for (unsigned int i = 0; i < checkText.size(); i++)
					{
						if (checkPhrase[0] == checkText[i])
							{
								startPos = i;
								bool found = true;
								for (unsigned int j = 0; j < checkPhrase.size(); j++)
									{
										if (checkPhrase[j] != checkText[i++])
											{
												found = false;
												break;
											}

										if (i >= checkText.size())
											{
												break;
											}
									}

								if (found)
									{
										return std::make_pair(startPos, true);
									}
							}
					}
			}
		return std::make_pair(0, false);
	}

std::string strfn::findStringKeyword(const std::string &text, const std::string &phrase, const char endWord)
	{
		auto phr = strfn::doesStringContain(text, phrase);
		if (phr.second)
			{
				std::string checkText = text;
				std::transform(checkText.begin(), checkText.end(), checkText.begin(), ::tolower);

				bool checkForEnd = false;
				if (endWord != ' ')
					{
						checkForEnd = true;
					}

				std::string phraseText = "";
				for (int i = phr.first; i < checkText.size(); i++)
					{
						if (checkForEnd)
							{
								if (checkText[i] == endWord)
									{
										return phraseText;
									}
							}

						phraseText += text[i];
					}

				return phraseText;
			}

		return "";
	}

std::pair<std::string, std::string> strfn::splitString(const std::string &text, const char splitChar)
	{
		std::string firstPhrase = "";
		std::string secondPhrase = "";

		bool foundSplit = false;

		for (unsigned int i = 0; i < text.size(); i++)
			{
				if (text[i] == splitChar && !foundSplit)
					{
						foundSplit = true;
						i++;
					}

				if (!foundSplit)
					{
						firstPhrase += text[i];
					}
				else
					{
						secondPhrase += text[i];
					}
			}

		return std::make_pair(firstPhrase, secondPhrase);
	}

std::vector<std::string> strfn::stringToArray(const std::string &text, const char splitChar)
	{
		std::vector<std::string> allWords;
		std::string currentWord = "";

		for (unsigned int i = 0; i < text.size(); i++)
			{
                currentWord += text[i];
				if (text[i] == splitChar || i + 1 >= text.size())
					{
                        if (i + 1 < text.size())
                            {
                                currentWord.pop_back();
                            }
						allWords.push_back(currentWord);
						currentWord.clear();
					}
			}

		return allWords;
	}

std::string strfn::findDataInside(const std::string &text, const std::string &phrase,
								  const char start, const char end)
	{
		std::string data = "";

		bool foundNewStart = false;
		bool foundEnd = false;

		auto strContain = doesStringContain(text, phrase);
		if (strContain.second)
			{
				unsigned int startPos = strContain.first;
				for (unsigned int i = startPos; i < text.size(); i++)
					{
						if (text[i] == start)
							{
								startPos = i + 1;
								break;
							}
					}

				for (unsigned int i = startPos; i < text.size(); i++)
					{
						if (!foundEnd) 
							{
								if (text[i] == start)
									{
										foundNewStart = true;
									}
								else if (text[i] == end)
									{
										if (!foundNewStart)
											{
												foundEnd = true;
											}
										else
											{
												foundNewStart = false;
											}
									}
								
								if (!foundEnd) 
									{
										data += text[i];
									}
							}
						else
							{
								break;
							}
					}
			}

		return data;
	}

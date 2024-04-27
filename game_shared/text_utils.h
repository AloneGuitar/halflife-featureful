#pragma once
#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <vector>
#include <string>
#include "parsetext.h"

struct WordBoundary
{
	WordBoundary() : wordStart(0), wordEnd(0) {}
	unsigned int wordStart;
	unsigned int wordEnd;
};

typedef std::vector<WordBoundary> WordBoundaries;

template<typename It>
WordBoundaries SplitIntoWordBoundaries(It begin, It end)
{
	std::vector<WordBoundary> boundaries;

	const unsigned int len = end - begin;

	bool searchingForWordStart = true;
	for (It it = begin; it != end; ++it)
	{
		if (searchingForWordStart && !IsSpaceCharacter(*it))
		{
			WordBoundary boundary;
			boundary.wordStart = it - begin;
			boundaries.push_back(boundary);
			searchingForWordStart = false;
		}

		if (!searchingForWordStart && IsSpaceCharacter(*it))
		{
			boundaries.back().wordEnd = it - begin;
			searchingForWordStart = true;
		}
	}
	if (!searchingForWordStart) {
		boundaries.back().wordEnd = len;
	}

	return boundaries;
}

inline WordBoundaries SplitIntoWordBoundaries(const std::string &message) {
	return SplitIntoWordBoundaries(message.begin(), message.end());
}

inline WordBoundaries SplitIntoWordBoundaries(const char* message) {
	return SplitIntoWordBoundaries(message, message + strlen(message));
}

#endif

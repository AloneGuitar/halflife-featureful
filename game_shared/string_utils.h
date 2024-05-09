#pragma once
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <cstddef>
#include <cstring>

inline char *strncpyEnsureTermination(char *dest, const char *src, size_t n) {
	char* result = strncpy(dest, src, n);
	dest[n-1] = '\0';
	return result;
}

template <size_t N>
char* strncpyEnsureTermination(char (&dest)[N], const char* src)
{
	return strncpyEnsureTermination(dest, src, N);
}

inline bool IsValidIdentifierCharacter(char c) {
	return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}
inline bool IsSpaceCharacter(char c) {
	return c == ' ' || c == '\r' || c == '\n';
}

#endif

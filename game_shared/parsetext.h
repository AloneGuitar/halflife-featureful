#pragma once
#ifndef PARSETEXT_H
#define PARSETEXT_H

bool IsValidIdentifierCharacter(char c);
void SkipSpaceCharacters(const char* text, int& i, const int length);
void SkipSpaces(const char* text, int& i, const int length);
void ConsumeNonSpaceCharacters(const char* text, int& i, const int length);
void ConsumeLine(const char* text, int& i, const int length);
bool ReadIdentifier(const char* text, int &i, char* identBuf, unsigned int identBufSize);
bool ParseInteger(const char* valueText, int& result);
bool ParseColor(const char* valueText, int& result);
bool ParseBoolean(const char* valueText, bool& result);

#endif

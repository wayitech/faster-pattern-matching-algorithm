#pragma once

void BadChar(const char *pPattern, int nLen, int nBadArray[]);
void BadChar2(const char *pPattern,int nLen, int nBadArray[]);

bool GoodSuffix(const char *pPattern, int nLen, int nArray[]);

int BadCharSearch(const char* pDest, int nDLen, const char* pPattern, int nPLen, int *nArray);
int GoodSuffixSearch(const char* pDest, int nDLen, const char* pPattern, int nPLen, int *nArray);
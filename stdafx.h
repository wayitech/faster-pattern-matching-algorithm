// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

template <class T>
bool CompareTwoArray(T A1, T A2, int nLen)
{
    for (int i = 0; i < nLen; i++)
    {
        if (A1[i] != A2[i])
        {
            printf("mismatch!\r\n");
            return false;
        }
    }
    printf("match ok!\r\n");
    return true;
}

// TODO: reference additional headers your program requires here
//void ZBlock(const char* pattern, unsigned int length, unsigned int zvalues[])  
//{  
//    unsigned int i, j, k;  
//    unsigned int l, r;  
//
//    l = r = 0;  
//    zvalues[0] = 0;  
//
//    for(i = 1; i < length; ++i)  
//    {  
//        if(i >= r)  
//        {  
//            j = 0;  
//            k = i;  
//            zvalues[i] = 0;  
//
//            while(k < length && pattern[j] == pattern[k])  
//            {  
//                ++j;  
//                ++k;  
//            }  
//
//            if(k != i)  
//            {  
//                l = i;  
//                r = k - 1;  
//                zvalues[i] = k - i;  
//            }  
//        }  
//        else  
//        {  
//            if(zvalues[i - l] >= r - i + 1)  
//            {  
//                j = r - i + 1;  
//                k = r + 1;  
//
//                while(k < length && pattern[j] == pattern[k])  
//                {  
//                    ++j;  
//                    ++k;  
//                }  
//
//                l = i;  
//                r = k - 1;  
//                zvalues[i] = k - i;  
//            }  
//            else  
//            {  
//                zvalues[i] = zvalues[i - l];  
//            }  
//        }  
//    }  
//}  
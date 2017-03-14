// BmMatch.cpp : Defines the entry point for the console application.
//
/*
基于最长不重复字符串的比较算法
2014年
wayitech
*/
#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "string"
#include <Windows.h>
#include "BmAlgorithm.h"

//以下代码来自csdn july的博客，用于学习和测试
void BuildBadCharacterShift(char const* pattern, int plen, int* shift)  
{  
    for( int i = 0; i < 256; i++ )     
        *(shift+i) = plen;  
    while ( plen >0 )          
    {     
        *(shift+(unsigned char)*pattern++) = --plen;          
    }     
}  
int search_badcharacter(char const* src, int slen, char const* patn, int plen, int* shift)  
{     
    int s_idx = plen, p_idx;    
    int skip_stride;      
    if (plen == 0)        
        return -1;  
    while (s_idx <= slen)//计算字符串是否匹配到了尽头   
    {     
        p_idx = plen;     
        while (src[--s_idx] == patn[--p_idx])//开始匹配       
        {     
            //if (s_idx < 0)   
            //Return -1;      
            if (p_idx == 0)       
            {             
                return s_idx;         
            }     
        }  
        skip_stride =  shift[(unsigned char)src[s_idx]];  
        s_idx += (skip_stride>plen-p_idx ? skip_stride: plen-p_idx)+1;     
    }     
    return -1;    
}  

void  BuildGoodSuffixShift(char const* pattern, int plen, int* shift)  
{     
    shift[plen-1] = 1;            // 右移动一位    
    char end_val = pattern[plen-1];  
    char const* p_prev, const* p_next, const* p_temp;  
    char const* p_now = pattern + plen - 2;            // 当前配匹不相符字符,求其对应的shift  
    bool isgoodsuffixfind = false;                    // 指示是否找到了最好后缀子串,修正shift值  
    for( int i = plen -2; i >=0; --i, --p_now)         
    {     
        p_temp = pattern + plen -1;   
        isgoodsuffixfind = false;  
        while ( true )        
        {         
            while (p_temp >= pattern && *p_temp-- != end_val);          
            // 从p_temp从右往左寻找和end_val相同的字符子串    
            p_prev = p_temp;       // 指向与end_val相同的字符的前一个     
            p_next = pattern + plen -2;             // 指向end_val的前一个  
            // 开始向前匹配有以下三种情况      
            //第一：p_prev已经指向pattern的前方,即没有找到可以满足条件的最好后缀子串  
            //第二：向前匹配最好后缀子串的时候,p_next开始的子串先到达目的地p_now,   
            //需要判断p_next与p_prev是否相等,如果相等,则继续住前找最好后缀子串     
            //第三：向前匹配最好后缀子串的时候,p_prev开始的子串先到达端点pattern, 这个可以算是最好的子串  

            if( p_prev < pattern  && *(p_temp+1) != end_val )         // 没有找到与end_val相同字符      
                break;  

            bool  match_flag = true;        //连续匹配失败标志    
            while( p_prev >= pattern && p_next > p_now )        
            {  
                if( *p_prev --!= *p_next-- )          
                {     
                    match_flag = false;      //匹配失败   
                    break;    
                }  
            }  

            if( !match_flag )  
                continue;          //继续向前寻找最好后缀子串  
            else      
            {     
                //匹配没有问题, 是边界问题  
                if( p_prev < pattern || *p_prev != *p_next)    
                {  
                    // 找到最好后缀子串  
                    isgoodsuffixfind = true;  
                    break;    
                }  
                // *p_prev == * p_next  则继续向前找    
            }     
        }  
        shift[i] = plen - i + p_next - p_prev;  
        if( isgoodsuffixfind )  
            shift[i]--;               // 如果找到最好后缀码，则对齐，需减修正  
    }  
}  
int search_goodsuffix(char const* src, int slen, char const* patn, int plen, int* shift)  
{  
    int s_idx = plen, p_idx;    
    int skip_stride;  
    if (plen == 0)    
        return -1;  

    while (s_idx <= slen)//计算字符串是否匹配到了尽头   
    {     
        p_idx = plen;  
        while (src[--s_idx] == patn[--p_idx])//开始匹配   
        {     
            //if (s_idx < 0)   
            //return -1;  
            if (p_idx == 0)   
            {         
                return s_idx;     
            }  
        }  
        skip_stride =  shift[p_idx];  
        s_idx += skip_stride +1;  
    }  
    return -1;  
}  
//以上代码来自csdn july的博客，用于学习和测试

void ZBlock(const char* pattern, unsigned int length, unsigned int zvalues[])  
{  
    unsigned int i, j, k;  
    unsigned int l, r;  

    l = r = 0;  
    zvalues[0] = 0;  

    for(i = 1; i < length; ++i)  
    {  
        if(i > r)  
        {  
            j = 0;  
            k = i;  
            zvalues[i] = 0;  

            while(k < length && pattern[j] == pattern[k])  
            {  
                ++j;  
                ++k;  
            }  

            if(k != i)  
            {  
                l = i;  
                r = k - 1;  
                zvalues[i] = k - i;  
            }  
        }  
        else  
        {  
            if(zvalues[i - l] >= r - i + 1)  
            {  
                j = r - i + 1;  
                k = r + 1;  

                while(k < length && pattern[j] == pattern[k])  
                {  
                    ++j;  
                    ++k;  
                }  

                l = i;  
                r = k - 1;  
                zvalues[i] = k - i;  
            }  
            else  
            {  
                zvalues[i] = zvalues[i - l];  
            }  
        }  
        printf(" zvalues[%d] = %d, left is %d, right is %d\r\n", i,  zvalues[i], l, r);
    }  
}  
void ZBox(const char* pattern, unsigned int length, unsigned int zvalues[])  
{
    zvalues[0] = 0;
    int l = 0;
    int r = 0;
    for (int i = 1; i < length; i++)
    {
        if (i > r)
        {
            int n = 0;
            for ( ; pattern[n] == pattern[i+n]; n++);
            if (0 != n)
            {
                r = i+n-1;
                l = i;
            }
            zvalues[i] = n;
        }
        else
        {
            if (zvalues[i-l] < r-i+1)
            {
                zvalues[i] = zvalues[i-l];
            }
            else
            {
                int n = 1;
                for (;pattern[r-i+n] == pattern[r+n]; n++);
                zvalues[i] = r-i+n;
                r += n-1;
                l = i;
            }
        }
        printf(" zvalues2[%d] = %d, left is %d, right is %d\r\n", i,  zvalues[i], l, r);
    }
}

void ForceZBox(const char* pattern, unsigned int length, unsigned int zvalues[])  
{
    zvalues[0] = 0;
    for (int i = 1; i < length; i++)
    {
        int b1 = 0, b2 = i;
        for ( ; b2 < length && pattern[b1] == pattern[b2]; b1++, b2++);
        zvalues[i] = b1;
        printf(" zvalues3[%d] = %d\r\n", i,  zvalues[i]);
    }
}

int SearchWithZBox(const char* dest, const char* pattern)
{
    int nDlen = strlen(dest);
    int nPlen = strlen(pattern);

    char *str = new char[nDlen+nPlen+1];
    if (!str)
    {
        goto Exit0;
    }
    int *Z = new int[nDlen+nPlen+1];
    if (!Z)
    {
        goto Exit0;
    }
    memcpy(str, pattern, nPlen+1);
    memcpy(str+nPlen+1, dest, nDlen);

    //create z box
    Z[0] = 0;
    int l = 0;
    int r = 0;
    for (int i = 1; i < nDlen+nPlen+1; i++)
    {
        if (i > r)
        {
            int n = 0;
            for (; str[i+n] == str[n]; n++);
            if (n > 0)
            {
                l = i;
                r = i+n-1;
            }
            Z[i] = n;
        }
        else
        {
            if (Z[i-l] < r-i+1)
            {
                Z[i] = Z[i-l];
            } 
            else
            {
                int n = 1;
                for (int s = r-i; str[s+n] == str[r+n]; n++);
                Z[i] = r-i+n;
                l = i;
                r = r+n-1; 
            }
        }
        if (Z[i] == nPlen)
        {
            printf("Find at %d\r\n", i - nPlen - 1);
        }
        assert (Z[i] <= nPlen);
    }
Exit0:
    if (str)
    {
        delete str;
        str = NULL;
    }
    if (Z)
    {
        delete Z;
        Z = NULL;
    }
    return -1;
}

void NBox2(const char *pPattern, int nLen, unsigned int *pNBox)
{
    pNBox[nLen-1] = 0;
    int l = nLen-1;
    int r = nLen-1;
    for (int i = nLen-2; i >= 0; i--)
    {
        if (i < l)
        {
            int n = 0;
            for ( ; i>=n && pPattern[i-n] == pPattern[nLen-1-n]; n++);
            if (n > 0)
            {
                r = i;
                l = i-n+1;
            }
            pNBox[i] = n;
        }
        else
        {
            if (pNBox[i+nLen-1-r] < i-l+1)
            {
                pNBox[i] = pNBox[i+nLen-1-r];
            }
            else
            {
                int n = 1;
                for (int s = l+nLen-1-i; l>=n && pPattern[l-n] == pPattern[s-n]; n++);
                pNBox[i] = -(l-n-i);
                l = l-n+1;
                r = i;
            }
        }
        printf("nValues[%d] = %d left = %d, right = %d\r\n", i, pNBox[i], l, r);
    }
}

void NBox(const char *p, unsigned int *nValues)
{
    int nLen = strlen(p);
    int l = nLen-1;
    int r = nLen-1;
    nValues[nLen-1] = 0;
    for (int i = nLen-2; i >= 0; i--)
    {
        if (i < l)
        {
            int n = 0;
            for (int b = nLen-1; i>=n && p[i-n] == p[b-n]; n++);
            if (n > 0)
            {
                l = i - n + 1;
                r = i;
            }
            nValues[i] = n;
        }
        else
        {
            if (nValues[i+nLen-1-r] < i-l+1)
            {
                nValues[i] = nValues[i+nLen-1-r];
            }
            else
            {
                int n = 1;
                for (int b = l+nLen-1-i; l>=n && p[l-n] == p[b-n]; n++);
                nValues[i] = i-l+n;
                l = l-n+1;
                r = i;
            }
        }
        printf("nValues[%d] = %d left = %d, right = %d\r\n", i, nValues[i], l, r);
    }
}

int test()
{
    unsigned int zvalues[200];
    unsigned int zvalues2[200];
    unsigned int zvalues3[200];//        1         2         3         4
    unsigned int nValues[200]; //        1         2         3         4
    unsigned int nValues2[200];//        1         2         3         4
    int shift[256];          //01234567890123456789012345678901234567890123456789
    int shift2[256];
    const char dest[] =       "xxxbaaaabaaaxbbaaabaaababaabaabaaabcxbaabaaabaaabcdaaxb";
    const char pattern[] =    "abcd";
    const char pattern2[] =   "aaabaaaab";
    const char pattern3[] =   "baaaabaaca";
    BuildGoodSuffixShift(pattern3, strlen(pattern3), shift);
    GoodSuffix(pattern3, strlen(pattern3), shift2);

    //BuildBadCharacterShift(pattern, strlen(pattern), shift);
    //printf("Find at %d\r\n", search_badcharacter(dest, strlen(dest), pattern, strlen(pattern), shift));
    printf("Find at %d\r\n", search_goodsuffix(dest, strlen(dest), pattern3, strlen(pattern3), shift));
    int shiftBad[256];
    BadChar(pattern3, strlen(pattern3), shiftBad);
    printf("Find at %d\r\n", BadCharSearch(dest, strlen(dest), pattern3, strlen(pattern3), shiftBad));
    int shiftGood[256];
    GoodSuffix(pattern3, strlen(pattern3), shiftGood);
    printf("Find at %d\r\n", GoodSuffixSearch(dest, strlen(dest), pattern3, strlen(pattern3), shiftGood));
    printf("%s\r\n", pattern2);
    ZBlock(pattern2, strlen(pattern2), zvalues);
    ZBox(pattern2, strlen(pattern2), zvalues2);
    ForceZBox(pattern2, strlen(pattern2), zvalues3);
    CompareTwoArray(zvalues, zvalues2, strlen(pattern2));
    CompareTwoArray(zvalues3, zvalues2, strlen(pattern2));
    SearchWithZBox(dest, pattern2);
    SearchWithZBox(dest, "xb");
    NBox(pattern3, nValues);
    NBox2(pattern3, strlen(pattern2), nValues2);
    CompareTwoArray(zvalues3, nValues, strlen(pattern2));
    CompareTwoArray(nValues2, nValues, strlen(pattern2));
    NBox2("kekkek", strlen("kekkek"), nValues);
    for (int i = 0; i < strlen("kekkek")-1; i++)
    {
        printf("NBox[%d] = %d\r\n", i, nValues[i]);
    }
    const char * p = NULL;
    p = pattern;
   //system("pause");

	return 0;
}


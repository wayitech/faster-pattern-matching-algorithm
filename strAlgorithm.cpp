/*
基于最长不重复字符串的比较算法
2014年
wayitech
*/

#include "StdAfx.h"
#include "BmAlgorithm.h"
#include <string.h>
#include <time.h>

void BadChar(const char *ppattern, int nLen, int nBadArray[])
{
    for (int i = 0; i < 256; i++)
    {
        nBadArray[i] = -1;
    }
    for (int i = 0; i < nLen; i++)
    {
        nBadArray[(unsigned char)ppattern[i]] = i;
    }
}

void BadChar2(const char *ppattern,int nLen, int nBadArray2[])
{
    for (int i = 0; i < nLen; i++)
    {
        nBadArray2[i] = -1;
    }
    for (int i = nLen-1; i >= 0 ; i--)
	{        
		int j = i-1;
		for( ; j >=0 ; j-- )
		{
			if( (unsigned char)ppattern[j] == (unsigned char)ppattern[i] )
			{
				nBadArray2[i] = i -j;
				break;
			}
		}
		//nBadArray2[i] = -1;
	}
}

void NBox(const char *ppattern, int nLen, int *pNBox)
{
    if (0 == nLen)
    {
        return;
    }
    pNBox[nLen-1] = 0;
    int l = nLen-1;
    int r = nLen-1;
    for (int i = nLen-2; i >= 0; i--)
    {
        if (i < l)
        {
            int n = 0;
            for ( ; i >= n && ppattern[i-n] == ppattern[nLen-1-n]; n++);
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
                for (int s = l+nLen-1-i; l >= n && ppattern[l-n] == ppattern[s-n]; n++);
                pNBox[i] = -(l-n-i);
                l = l-n+1;
                r = i;
            }
        }
    }
}

/************************************************************************/
/* 求最长不重复子串                                                     */
/************************************************************************/

char* lnorepstr(char*  s, int *pnPos)
{
    char A[26];
    int  i, j;
    int  maxi = 0, maxlen = 0;
    int  len = strlen(s);

//    for(i = 0; i < 26; i++)
//        A[i] = -1;

    memset(A, -1, 26);
    for (i = 0; i < len; i++)
    {
        A[s[i] - 'a'] = 1;
        for(j = i+1 ; j < len; j++)
        {
            if(A[s[j]-'a'] == -1) //该字符没有出现
            {
                A[s[j]-'a'] = 1;
                if(j - i> maxlen)
                {
                    maxlen = j - i;
                    maxi = i;
                }
            }
            else
                break;
        }
        memset(A, -1, 26);
    }
	*pnPos = maxi;
   // printf("longest no repeat string: %.*s\n", maxlen+1, &s[maxi]);//
	char strRet[128];
	strncpy(strRet, &s[maxi], maxlen+1);
	strRet[maxlen+1] = '\0';
	return &strRet[0];
}

//
//void ZBox(const char* pattern, unsigned int length, unsigned int zbox[])    
//{  
//    zbox[0] = 0;  
//    unsigned int left = 0;  
//    unsigned int right = 0;  
//    for (unsigned int i = 1; i < length; i++)  
//    {  
//        if (i > right)//情况3   
//        {  
//            int n = 0;  
//            for ( ; pattern[n] == pattern[i+n]; n++);  
//            if (0 != n)  
//            {  
//                right = i+n-1;  
//                left = i;  
//            }  
//            zbox[i] = n;  
//        }  
//        else  
//        {  
//            if (zbox[i-left] < right-i+1)//情况1   
//            {  
//                zbox[i] = zbox[i-left];  
//            }  
//            else//情况2   
//            {  
//                int n = 1;  
//                for (;pattern[right-i+n] == pattern[right+n]; n++);  
//                zbox[i] = right-i+n;  
//                right += n-1;  
//                left = i;  
//            }  
//        }  
//        printf(" zbox[%d] = %d, left is %d, right is %d\r\n", i,  zbox[i], left, right);  
//    }  
//}  


bool GoodSuffix(const char *ppattern, int nLen, int nGoodArray[])
{
    if (0 == nLen)
    {
        return false;
    }
    //不直接求好后缀数组，因为直接求的话时间复杂度是O（n的平方）
    //我们先计算出N-BOX数组
    int *pNBox = new int[nLen];
    if (!pNBox)
    {
        return false;
    }

    NBox(ppattern, nLen, pNBox);
    
    //根据N-BOX确定好后缀的值，首先全部初始化为-1
    for (int i = 0; i < nLen; i++)
    {
        nGoodArray[i] = -1;
    }
    //第一种情况：完全匹配L'(i)
    int nMax = 0;
    for (int i = 0; i < nLen; i++)
    {
        nGoodArray[nLen-1-pNBox[i]] = i;
        if (pNBox[i] == i+1)
        {
            nMax = i+1;
        }
    }
    nGoodArray[nLen-1] = -1;
    //第二种情况：部分匹配l'(i) 前缀和后缀的匹配
    if (nMax != 0)
    {
        for (int i = 0; i < nLen-1-nMax; i++)
        {
            if (nGoodArray[i] == -1)
            {
                nGoodArray[i] = nMax-1;
            }
        }
    }
    nGoodArray[nLen-1] = nLen-2;
    if (pNBox)
    {
        delete pNBox;
        pNBox = NULL;
    }
    return true;
}

int BadCharSearch(const char* pDest, int nDLen, const char* ppattern, int nPLen, int *nBadArray)
{
    int nDstart = nPLen-1;
    while (nDstart < nDLen)
    {
        int i = 0;
        for (; i <= nPLen-1 && pDest[nDstart-i] == ppattern[nPLen-1-i]; i++);
        if (i > nPLen-1)
        {
            return nDstart - (nPLen-1);
        }
        int nIdx = nBadArray[pDest[nDstart-i]];
        if (nIdx >= nPLen-1-i)//if nIdx is greater than or equal to mismatch position, just move pattern[i] to right 1 character 
        {
            nDstart = nDstart + 1;
        }
        else
        {
            nDstart = nDstart + (nPLen-1-i - nIdx);
        }
    }
    return -1;
}

int GoodSuffixSearch(const char* pDest, int nDLen, const char* ppattern, int nPLen, int *nGoodArray)
{
    int nDstart = nPLen-1;
    while (nDstart < nDLen)
    {
        int i = 0;
        for (; i <= nPLen-1 && pDest[nDstart-i] == ppattern[nPLen-1-i]; i++);
        if (i > nPLen-1)
        {
            return nDstart - (nPLen-1);
        }
        int nIdx = nGoodArray[nPLen-1-i];
        nDstart = nDstart + (nPLen-1 - nIdx);
    }
    return -1;
}

//最长不重复字符串算法
int NewSearch(const char* pDest, int nDLen,
			  const char* ppattern, int nPLen,
			  const char* pStrmost, int nLenmost,
			  int nmostStart, int *nGoodArray, 
			  int *nBadArray, int *nBadArray2)
{
	//算法本身有三个比较点
	//1.末位字符比较
	//2.最长不重复字符串从末位比较

	if( 0 == nPLen )
	{
		return  -1;
	}

	int nDstart = nPLen - 1 ;
	int nStrPattern = nmostStart + nLenmost - 1;//最长子串末位字符位置
	int nEndmostLen = nPLen - nStrPattern -1 ;    //最长子串末位字符距离模式串尾长度
	int lenMost = nLenmost+nmostStart;
	int i = 0;
	int j = 0;
	int nIdxBad = 0;
	int nShiftBad = 0;
	int nShiftBad2 = 0;		int sum = 0;


	if(1)
	{   //sunday算法
		while( nDstart < nDLen )
		{
			i = 0;
			sum++;
			//比较规则字符串
			for(; i <= nPLen - 1 && pDest[nDstart-i] == ppattern[nPLen-i-1]; i++);	

			//nIdxBad    = nBadArray[pDest[nDstart]];
			if (i >= nPLen-1)
			{
				return nDstart - (nPLen-1);
			}
			else  //字符不符合，则按照sunday算法跳转
			{
				nIdxBad     = nBadArray[pDest[nDstart+1]];	
				nShiftBad   = nPLen - nIdxBad;

				//nDstart += (nShiftGood > nShiftBad) ? nShiftGood : nShiftBad;		
				nDstart += nShiftBad;
				continue;
			}
		}
	}
	else
	{

		while( nDstart < nDLen )
		{
			i =0; sum++;
			j = (pDest[nDstart] == ppattern[nPLen - 1])?1:0;
			nIdxBad    = nBadArray[pDest[nDstart]];
			if(nIdxBad == -1)
			{
				nDstart += nPLen;
				continue;
			}
			else if(j != 1)  //末位字符不符合，则按照sunday算法跳转
			{
				nShiftBad2 = nPLen-1-nIdxBad;
				nIdxBad = 0;

				nIdxBad     = nBadArray[pDest[nDstart+1]];	
				nShiftBad   = nPLen - nIdxBad;

				//nDstart += (nShiftGood > nShiftBad) ? nShiftGood : nShiftBad;		
				nDstart += (nShiftBad > nShiftBad2)?nShiftBad:nShiftBad2;
				continue;
			}
			//比较最长不重复字符串
			for(; i <= nStrPattern - 1 && pDest[nDstart-nEndmostLen-i] == ppattern[nStrPattern-i]; i++);
			if( (i > nStrPattern - 1) )
			{//初步匹配成功,最长字符串及之前匹配成功
				i = 0;
				for(; i < nEndmostLen &&  pDest[nDstart-i-1] == ppattern[nPLen-2-i];i++); //比较最长字符串之后的模式字串

				if(i == nEndmostLen)
				{			
					return nDstart-(nPLen-1);				
				} 
				else
				{
					nIdxBad     = nBadArray[pDest[nDstart+1]];	
					nShiftBad   = nPLen - nIdxBad;

					nDstart += nLenmost>nShiftBad?nLenmost:nShiftBad;  //根据最长不重复串长度和坏字符的最大值跳转
				}
			}
			else
			{
				nIdxBad = nBadArray2[pDest[nDstart-nEndmostLen-i]];
				nShiftBad2 = nStrPattern -nIdxBad ;  //最长不重复字符串坏字符跳转

				nIdxBad = 0;

				nIdxBad     = nBadArray[pDest[nDstart+1]];	
				nShiftBad   = nPLen - nIdxBad;

				//nDstart += (nShiftGood > nShiftBad) ? nShiftGood : nShiftBad;		
				nDstart += (nShiftBad > nShiftBad2)?nShiftBad:nShiftBad2;
			}
		}
	}
	return -1;
}

int BMSearch(const char* pDest, int nDLen, 
             const char* ppattern, int nPLen, 
             int *nGoodArray, int *nBadArray)
{
    if (0 == nPLen)
    {
        return -1;
    }
    int nDstart = nPLen-1;
	int sum =0 ;
    while (nDstart < nDLen)
    {
		sum++;
        int i = 0;
        for (; i <= nPLen-1 && pDest[nDstart-i] == ppattern[nPLen-1-i]; i++);
        if (i > nPLen-1)
        {
            return nDstart - (nPLen-1);
        }
        int nIdxGood    = nGoodArray[nPLen-1-i];
        int nShiftGood  = nPLen-1 - nIdxGood;

        int nIdxBad     = nBadArray[pDest[nDstart-i]];
        int nShiftBad   = (nIdxBad >= nPLen-1-i) ? 1 : nPLen-1-i - nIdxBad;

        nDstart += (nShiftGood > nShiftBad) ? nShiftGood : nShiftBad;
    }
    return -1;
}

void TestBMSearch()
{
    int         nFind;
    int         nGoodArray[100] = {0};
    int         nBadArray[256]  = {0};
	int         nBadArray2[256]  = {0};

                               //        1         2         3         4
                               //0123456789012345678901234567890123456789012345678901234
    const char  dest[]      =   "abcxxxbaaxueriwqjskdfcksejdfdj.,jsdfeskdfcksejxfvweofrdsjfw9erbbxeoirdslfjksdfkdsfkjskdfcksejskfejsdfksjfjksdfaabaaasdfsdfdsfsdfxbbaaabcdaajsdfeskdfcksejdfsdfdsfesdkeaaxbbxasdfdsfesdbxa.cxb";
    char  pattern[][20] = {
		"eskdfcksejdfsdfdsfe",
		"abcdaa",
		"abcda",
		"jfks",  
		"jsdfeskdfcksejdf",
        "abcda",
        "xxx",
        "ax",
        "xb",
        "bddd",
        "sdfdsfesdke",
        "aaabaaaab",
        "baaaabaaa",
    };



	{
		for (int i = 0; i < 1;i++)//sizeof(pattern)/sizeof(pattern[0]); i++)
		{
			BadChar(pattern[i], strlen(pattern[i]), nBadArray);
			GoodSuffix(pattern[i], strlen(pattern[i]), nGoodArray);

			//if (-1 != nFind)
			//{
			//    printf("Found    \"%s\" at %d \t%s\r\n", pattern[i], nFind, dest+nFind);
			//}
			//else
			//{
			//    printf("Found    \"%s\" no result.\r\n", pattern[i]);
			//}
			int nPos;
			char pCh[128];
			char pCh2[128];
			strcpy(pCh, lnorepstr(pattern[i], &nPos));
			strncpy(pCh2, pattern[i], strlen(pCh)+nPos);
			pCh2[strlen(pCh)+nPos] = '\0';
			BadChar(pCh2, strlen(pCh2), nBadArray2);
			int kTemp = 0;
			int tEnd = 0;
			double t = 0.00;

			int tStart;
			//	tStart = clock();

			//for(;kTemp < 56200;kTemp++)
			//{
			//	nFind = NewSearch(dest, strlen(dest),pattern[i], strlen(pattern[i]), pCh, strlen(pCh),nPos,nGoodArray, nBadArray, nBadArray2); 
			//}	

			// tEnd = clock();			
			// t = (double)(tEnd - tStart)/CLOCKS_PER_SEC;

			printf("%lf seconds is used\n", t);
			
			tStart = clock();
			for(;kTemp < 56200;kTemp++)
			{
			nFind = BMSearch(dest, strlen(dest), pattern[i], strlen(pattern[i]), nGoodArray, nBadArray);
			}
			tEnd = clock();

			t = (double)(tEnd - tStart)/CLOCKS_PER_SEC;
			printf("%lf seconds is used\n", t);

			if (-1 != nFind)
			{
				//printf("Found    \"%s\" at %d \t%s\r\n", pattern[i], nFind, dest+nFind);
			}   
		}	

	}		

}
int test();

//NewSearch为新提出的根据最长不重复字符串的改进Sunday算法
//BmSearch为BM算法 ，NewSearch内有Sunday算法实现

int _tmain(int argc, _TCHAR* argv[])
{
    TestBMSearch();
	//test();
	printf("Test Searchend\r\n");
    return 0;
}
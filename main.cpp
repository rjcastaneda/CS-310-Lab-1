/*
 * CS310 Lab1: Software Decode
 *
 * Programmer Notes:
 * Only 1st instance of signals will be found.
 * Program uses Naive methods of array traversals and searches. (AKA inefficient)
 * If time allowed it, Some improvements that could be made is combining the function for pattern one and two into one
 * and making the in-order checker use the overflowcheck function.
 *
 */

#include <iostream>
#include <ostream>

using namespace std;
bool overflowCheck(unsigned short int one, unsigned short two)
{
    int overflow = 0;
    unsigned short int temp = 0, tempTwo = 0;
    temp = one;
    tempTwo = two;
    int count = 0;
    while(temp != tempTwo)              //Loop counts the gap difference of the supposed overflow
    {
        temp++;
        count++;
        if(count > 5000)               //If the gap is huge (threshold 5000), then it's not overflow
        {
            return false;
        }
    }

    overflow =  one + count;

    if(overflow > 65535)            //There is overflow if the count + the supposed overflow is over 65535 (Max for unsigned int)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool inOrder(unsigned short int data[], int size) //Checks if data is inOrder and if there's overflow (An error check)
{

    if(size == 2)                   //If their are only two elements in data, there is not info to determine if there is a pattern.
    {
        return false;
    }

    //Basically overflow check function again, except modified to also make sure array is in-order.

    int overflowChk = 0;
    unsigned short int temp = 0, tempTwo = 0;

    for (int x = 0; x < size-1; x++)
    {
        if(data[x] > data [x+1])
        {
            temp = data[x];
            tempTwo = data[x+1];
            int count = 0;
            while(temp != tempTwo)
            {
                temp++;
                count++;
                if(count > 5000)
                {
                    return false;
                }
            }

            overflowChk =  data[x] + count;

            if(overflowChk > 65535)
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}


bool patternOne(unsigned short int data[],int length, int gapPtrn, int *retIndex)           //Searches for 1st kind of pattern in array 110
{
    int firstElement = 0, secondElement = 0, thirdElement = 0;
    unsigned short int search = 0;
    bool signalOne = false, signalTwo = false;                  //Signal One indicates that a 11 code is found, Signal Two indicates 111 pattern is found.

    for(int x = *retIndex; x < length; x++)                     //Searches for 11 code, indicating start of pattern.
    {
        if(signalOne)
        {
            break;
        }
        else
        {
            search = data[x] + gapPtrn;                 //The number to be searched within the gap pattern.
        }

        for(int y = x; y <= length; y++)
        {
            if(data[y] == search)                       //If two elements within gap distance is found end the loop.
            {
                firstElement = x;                       //Element variables contain the index of the pattern.
                secondElement = y;
                signalOne = true;
                break;
            }
            if(data[y] > search)                        //If the number in the index is greater than the search, check for overflow first, then possibly break.
            {
                if(overflowCheck(data[y],search))
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if(signalOne && (secondElement < length))
    {
        for(int x = secondElement; x < length; x++)
        {
            if(signalTwo)
            {
                break;
            }
            else
            {
                search = data[x] + gapPtrn;
            }

            for(int y = x; y < length; y++)
            {
                if(data[y] == search)
                {
                    signalTwo = true;
                    thirdElement = y;
                    break;
                }
                if(data[y] > search)
                {
                    if(overflowCheck(data[y],search))
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    if(signalTwo && (thirdElement < length)) //If you can still search array, and a 111 is present, continue to search.
    {
        unsigned short int temp[length];
        for(int x = thirdElement; x < length; x++)
        {
            temp[x] = data[x];
        }
        return patternOne(temp, length, gapPtrn, retIndex);
    }

    *retIndex = firstElement + *retIndex;

    return signalOne && !signalTwo;         //A 110 pattern is found if signalOne is activated (true) and signalTwo is not.
}

//patterTwo function works the same way as patternOne function, just the return logic is different.
bool patternTwo(unsigned short int data[],int length, int gapPtrn, int *retIndex)           //Searches for 2nd kind of pattern in array, 111.
{
    int firstElement = 0, secondElement = 0, thirdElement = 0;
    unsigned short int search = 0;
    bool signalOne = false, signalTwo = false;


    for(int x = *retIndex; x < length; x++)
    {
        if(signalOne)
        {
            break;
        }
        else
        {
            search = data[x] + gapPtrn;
        }

        for(int y = 0; y <= length; y++)
        {
            if(data[y] == search)
            {
                firstElement = x;
                secondElement = y;
                signalOne = true;
                break;
            }
            if(data[y] > search)
            {
               if(overflowCheck(data[y],search))
               {
                   continue;
               }
               else
               {
                   break;
               }
            }
        }
    }

    if(signalOne && (secondElement < length - 1))
    {
        for(int x = secondElement; x < length; x++)
        {
            if(signalTwo)
            {
                break;
            }
            else
            {
                search = data[x] + gapPtrn;
            }

            for(int y = 0; y < length; y++)
            {
                if(data[y] == search)
                {
                    signalTwo = true;
                    thirdElement = y;
                    break;
                }
                if(data[y] > search)
                {
                    if(overflowCheck(data[y],search))
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    if(signalOne && !signalTwo && (thirdElement < length)) //If you can still search array, when a 110 is found, continue to look for 111.
    {
        unsigned short int temp[length];
        for(int x = secondElement; x < length; x++)
        {
            temp[x] = data[x];
        }
        return patternTwo(temp, length, gapPtrn, retIndex);
    }

    *retIndex = firstElement + *retIndex;
    return signalOne && signalTwo;                      //A 111 pattern is found if signalOne and signalTwo are activated (true)
}

void softwareDecode(unsigned short int data[], int arrSize, int gapSpace) //Detects the patterns, and prints the index when a pattern starts
{
    cout<<"Data set is: ";
    for(int x = 0; x < arrSize; x++)
    {
        if(x == arrSize-1)
        {
            cout<<data[x]<<'.';
        }
        else
        {
            cout<<data[x]<<','<<' ';
        }
    }
    cout<<endl;

        if(!inOrder(data,arrSize))
        {
            cout<<"Fail: Data is not In-Order or not enough info."<<endl;
            return;
        }

        int index = 0;
        int indexTwo = 0;

        if(patternOne(data, arrSize, gapSpace, &index))
        {
            cout<<"There is a time signal 110 at index "<< index << endl;
        }
        else
        {
            cout<<"Fail: time signal 110 not found."<<endl;
        }

       if (patternTwo(data, arrSize, gapSpace, &indexTwo))
        {
            cout<<"There is a time signal 111 at index "<< indexTwo << endl;
        }
        else
        {
            cout<<"Fail: time signal 111 not found."<<endl;
        }
        cout<<endl;
}

int main() //Driver
{
    unsigned short int dataVals[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,39,40,50};          //Insert data values into array here.
    int gap = 20;                                                         //The size of the gap for detecting the pattern.
    int size = sizeof(dataVals)/sizeof(unsigned short int);
    softwareDecode(dataVals,size,gap);

    return 0;
}

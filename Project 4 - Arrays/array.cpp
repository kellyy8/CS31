//
//  main.cpp
//  array
//
//  Created by Kelly Yu on 10/28/21.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int reduplicate(string a[], int n);
int locate(const string a[], int n, string target);
int locationOfMax(const string a[], int n);
int circleLeft(string a[], int n, int pos);
int enumerateRuns(const string a[], int n);
int flip(string a[], int n);
int locateDifference(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int locateAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

int main(){
       
    string c[1]={"b"};
    assert(separate(c, 1, "b") == 0);
    cerr << "All tests succeeded" << endl;
    
}

int reduplicate(string a[], int n){
    //check for bad arguments
    if (n < 0)
        return -1;
    
    for (int i=0; i<n ; i++)
        a[i] += a[i];
    
    return n;
}

int locate(const string a[], int n, string target){
    //check for bad arguments
    if (n < 0)
        return -1;
    
    for (int i=0; i<n ; i++){
        if(a[i] == target)
            return i;                   // a string is found
    }

    return -1;                          //no such string found
}

int locationOfMax(const string a[], int n){
    //check for bad arguments
    //additionally, return -1 if we examine no elements in the array
    if (n<=0)
        return -1;
    
    int i=0;
    string maxStr=a[0];
    int positionNum = 0;            //assuming element at 0 is the greatest
    
    //check '>' for an array whose strings are NOT all identical
    //find the largest string
    for(i=1; i<n; i++){
        if(a[i] > maxStr)
            maxStr=a[i];                //store largest string
    }
    
    //find the position number of largest string
    //one loop occurs for an array whose strings are all identical
    for(i=0; i<n; i++){
        if(a[i]==maxStr){
            positionNum = i;
            break;
        }
    }

    return positionNum;
}

int circleLeft(string a[], int n, int pos){
    //check for bad arguments
    //if there are no elements, then every position is out of bounds
    //if there is 1 element, there's nothing after the 1st element to shift left;
        //that 1 element just stays in the same place; so, n>=1
    //position of eliminated element must be in bounds
    if(n <= 0 || pos >= n || pos < 0)
        return -1;
    
    string strAtPos = a[pos];           //temp var to store soon-to-be-eliminated item
    
    for(int i=pos; i<n-1 ; i++){         //shift values starting from indicated position
        a[i] = a[i+1];
    }
        
    a[n-1] = strAtPos;                  //last element replaced by eliminated item
    
    return pos;               //return original position of eliminated item
}

int enumerateRuns(const string a[], int n){
    //check for bad arguments
    if (n < 0)
        return -1;
    
    if (n==0)
        return 0;               //empty array yields no string sequences
    else if(n==1)
        return 1;               //array with one string only has one sequence
                                //no other string to compare array's only string with

    int numOfSequences=1;       //first element = start of first sequence; minimum of 1 seq
    for(int i=0; i<n-1 ; i++){
        if(a[i] != a[i+1])
            numOfSequences++;
    }
    
    return numOfSequences;      //return number of string sequences in array
}

int flip(string a[], int n){
    //check for bad arguments
    if (n < 0)
        return -1;

    for(int i=0; i<n/2 ; i++){
        string tempVal = a[i];
        a[i] = a[n-1-i];
        a[n-1-i] = tempVal;
    }
    
    return n;                   //return number of elements in array
}

int locateDifference(const string a1[], int n1, const string a2[], int n2){
    //check for bad arguments
    if (n1 < 0 || n2 < 0)
        return -1;

    int numValuesChecked=0;
    
    //make sure loop only checks interesting elements
    //number of iterations match the smaller n1 and n2
    if(n1 >= n2)
        numValuesChecked=n2;
    else
        numValuesChecked=n1;
    
    for(int i=0; i<numValuesChecked ; i++)
        if(a1[i] != a2[i])
            return i;

    //return smaller of n1 and n2 if a1[] and a2[] are identical up n1 or n2 # of elements
    return numValuesChecked;
}

int subsequence(const string a1[], int n1, const string a2[], int n2){
    //check for bad arguments
    if (n1 < 0 || n2 < 0 || n2 > n1)   //if n2>n1, a1[] never contains all of a2[]
        return -1;
    
    if (n2 == 0)
        return 0;        //0 elements = subsequence of any sequence starting at position 0
    
    int numElementsFound=0;
    int startIndex=0, i=0;
    
    for(startIndex=0; (startIndex + (n2-1)) < n1 ; startIndex++){
        i = startIndex;                       //so we don't change startIndex when i++
        for(int j=0; j < n2; i++, j++){
            //check if subsequence starts at startIndex
            if(a2[j] == a1[i])
                numElementsFound++;
            
            //fail --> end nested loop
            //so far not good; reset counter since only partial subsequence is found
            //restart the loop at a new startIndex
            else{
                numElementsFound=0;
                break;}
            
            //success --> end whole function
            //so far so good; check if whole subsequence is found yet
            if(numElementsFound == n2)
                return startIndex;
        }
    }
    
    return -1;              //a1 DOES NOT contain a2 as a contiguous subsequence
}

int locateAny(const string a1[], int n1, const string a2[], int n2){
    //check for bad arguments
    //if a1 is empty array, nothing in a2[] will be found in a1[] & vice versa
    if (n1 <= 0 || n2 <= 0)
        return -1;
    
    int positionNum=n1;        //stores the smallest position number of all the matches
                               //all position numbers in a1[] will be less than n1
    
    //don't check only portion of n1; scan through all of n1 for each element in a2[]
    if (n1 >= n2){
        for (int i=0; i<n2; i++){
            for (int j=0; j<n1 ; j++){
                if(a2[i]==a1[j] && j < positionNum){
                    //adjust smallest position number for every match if necessary
                    positionNum = j;}
            }
        }
    }
    //don't check only portion of n2; scan through all of n2 for each element in a1[]
    else{
        for (int i=0; i<n1; i++){
            for (int j=0; j<n2 ; j++){
                if(a1[i]==a2[j] && i < positionNum)
                    positionNum = i;
            }
        }
    }
    
    //found matches if positionNum has been replaced by a valid index number
    if (positionNum != n1)
        return positionNum;
    
    return -1;          //no element of a1 is equal to any element of a2
}

int separate(string a[], int n, string separator){
    //check for bad arguments
    if (n < 0)
        return -1;
    
//    //if no elements exist in a[], then no elements are < to separator
//    if (n==0)
//        return n;

    string tempVal="";
    int replaceHere=0;      //keeps track of what element we are on throughout replacements
    
    //move everything less than separator to the beginning of array
    for (int i=0; i<n; i++){
        if(a[i] < separator){
            tempVal = a[replaceHere];
            a[replaceHere] = a[i];
            a[i] = tempVal;         //element holds onto the value that was replaced
            replaceHere++;}
    }
    
    //position number of first element >= separator
    int positionNum = replaceHere;
    
    //everything equal to separator follows elements less than separator
    for (int i=replaceHere; i<n; i++){
        if(a[i] == separator){
            tempVal = a[replaceHere];
            a[replaceHere] = a[i];
            a[i] = tempVal;
            replaceHere++;}
    }

    //positionNum is assigned to replaceHere; if replaceHere = n,then all elements < separator
    //if no elements are not < separator,  return n
    if (positionNum == n)
        return n;
    
    //position number of first element >= separator
    return positionNum;
}

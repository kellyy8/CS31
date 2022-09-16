//
//  main.cpp
//  decrypt
//
//  Created by Kelly Yu on 11/12/21.
//

#include <iostream>
#include <cstring>
using namespace std;

bool decrypt(const char ciphertext[], const char crib[]);
bool checkPattern(const char cribDup[], const char textDup[], int startHere);

//void runtest(const char ciphertext[], const char crib[]){
//             cout << "====== " << crib << endl;
//             bool result = decrypt(ciphertext, crib);
//             cout << "Return value: " << result << endl;
//}

int main() {
    
    char aCrib[20]="aeggh tt";
    char aText[20]="yelln uu ... \n";
    decrypt(aText, aCrib);
    
//    cout.setf(ios::boolalpha); // output bools as "true"/"false"
//    runtest("Hirdd ejsy zu drvtry od.\nO'z fodvtrry.\n", "my secret");
//    runtest("Hirdd ejsy zu drvtry od.\nO'z fodvtrry.\n", "shadow");
//    runtest("RNTO GX NMKLFWN\n", "BANK OF AMERICA");
//    runtest("Rswjo qgx Tmeuo sgjsy jds vqgf vo jds vqzf xbby.\nUdbyjo iqcju cg wybgj cg jds esjqiqo zqy\nXbg'j rsj jds jsrrsy jycn jds ucrsgj qrqyt.\nZU 31 cu zdqrrsgecge!\n", "silent alarm");
//
//    runtest("F gspt fe! zyxZYXzyx--Abca abCa    bdefg## $$dsptrqtj6437 wvuWVUwvu\n\n8 9\n", "hush hush until november");
//    runtest("Kpio't dmpbl-boe-ebhhfs opwfm", "s cloak and");
//
//    //cipher //crib
//    runtest("yelln uu ... \n", "aeggh tt");

}

bool decrypt(const char ciphertext[], const char crib[]){
    int lenOfCrib = strlen(crib);
    if(lenOfCrib==0 || lenOfCrib > 90)      //empty crib or a crib with more than 90 characters = false
        return false;
    
//---REFORMATTING THE CRIB
    int j=0;                            //sets up next element to store copy of Crib & counts copyCrib len
    char copyCrib[110];                 //to store a copy of Crib with alpha & 1 space in between format
    
    for(int i=0; i < lenOfCrib; i++){
        if(isalpha(crib[i]) && isalpha(crib[i+1])){             //for consecutive alpha's, store 1st alpha
            copyCrib[j] = tolower(crib[i]);               //set all alpha's to lowercase for comparing later
            j++;
        }
        else if(isalpha(crib[i]) && (crib[i+1]=='\0')){         //for cribs who end with an alpha
            copyCrib[j] = tolower(crib[i]);
            j++;
            copyCrib[j] = '\0';                                  //end the character array
            j++;
            break;
        }
        else if(isalpha(crib[i]) && !(isalpha(crib[i+1]))){     //store 1 space for non-chars in btwn alphas
            copyCrib[j] = tolower(crib[i]);
            j++;
            copyCrib[j] = ' ';
            j++;
        }
    }
        
    //for cribs who end with non-alpha, last char in copy is a space
    //change the space to null character to end the array
    if(copyCrib[j-1] == ' '){
        copyCrib[j-1] = '\0';
    }
    

//---REFORMATTING THE CIPHERTEXT
    int lenOfText = strlen(ciphertext);
    int k=0;                    //sets up next element to store ciphertext copy & counts copy's length
    char copyText[70*90+100];       //stores copy of ciphertext with same format as copy of crib
    
    for(int i=0; i<lenOfText ; i++){
        if(isalpha(ciphertext[i])){                             //store all alpha's
            copyText[k] = tolower(ciphertext[i]);
            k++;
        }
        else if(ciphertext[i] == '\n'){                         //maintain the newline characters
            copyText[k] = '\n';
            k++;
        }
        else if (!(isalpha(ciphertext[i])) && isalpha(ciphertext[i+1])){
            copyText[k] = ' ';                                 //store 1 space between non-alpha's
            k++;
        }
    }
    
    //add null char to the copy of ciphertext
    //because the copy of ciphertext's last character is always an alpha or newline
    copyText[k] = '\0';
    k++;
    
//---CHECKING IF CRIB'S LENGTH FORMAT EXISTS IN CIPHERTEXT
    int cribPos=0, textPos=0;
    int charsMatched=0;
    int startIndex=0;                                //where in ciphertext we start checking for crib
    int matchStart=-1;
    bool matchFound=false;
    
    //length of the REFORMATTED CRIB
    int lenOfCopyCrib = strlen(copyCrib);
           
    for(textPos=0; copyText[textPos] != '\0'; textPos++){
        //if they are both letters
        if(isalpha(copyCrib[cribPos]) && isalpha(copyText[textPos])){
            charsMatched++;
            cribPos++;
        }
        //if they are both spaces
        else if(copyCrib[cribPos] == copyText[textPos]){
            charsMatched++;
            cribPos++;
        }
        //if match is lost
        else{
            charsMatched=0;                 //find the whole crib again
            cribPos=0;
            startIndex++;                   //restart at the next index from where we previously started
            textPos=startIndex;
            textPos--;                      //offset the increment of textPos when for loop runs again
        }
        
        
        //check if word sizes matches
        //messages in copy of ciphertext should END as space, newline, or null
        
        if(charsMatched==lenOfCopyCrib && !(isalpha(copyText[textPos+1]))){
            //where match starts in copy of ciphertext
            matchStart = textPos - (lenOfCopyCrib-1);

            //for words that start in middle of ciphertext
            //if an alpha exists before 1st letter of 1st word, length is wrong; not a match
            if(matchStart != 0 && isalpha(copyText[matchStart-1])){
                charsMatched=0;
                cribPos=0;
                startIndex++;
                textPos=startIndex;
                textPos--;
            }
            //check if PATTERN OF REPEATED CHARACTERS match
            else if(checkPattern(copyCrib, copyText, matchStart)){
                //complete match found; can perform decryption now
                matchFound = true;
                break;
            }
            //no match found; reset the search for crib like for any other lost match
            else{
                charsMatched=0;
                cribPos=0;
                startIndex++;
                textPos=startIndex;
                textPos--;
            }
        }
    }
           
    //CREATE COPY OF ORIGINAL CIPHERTEXT WITH ALL LETTERS LOWERCASED
    char textToDecrypt[70*90+100];                  //max 90*70 characters + 80 newlines + null character
    for(int i=0; i<(lenOfText + 1) ; i++){                  //length of text+1 to copy the null char too
        textToDecrypt[i] = tolower(ciphertext[i]);
    }
    
    //if match is found, decrypt 'textToDecrypt' using the "key":
    //for every letter in copyCrib, identify the corresponding letter in copyText
    //for every letter in 'textToDecrypt' == corresponding letter in copyText
    //change the letter in 'textToDecrypt' to the letter in copyCrib (UPPERCASE DECRYPTED LETTER TOO)
    
    if(matchFound==true){
        for(int i=0; copyCrib[i] != '\0' ; i++){
            if(isalpha(copyCrib[i])){
                for(int j=0; textToDecrypt[j] != '\0'; j++){
                    if(textToDecrypt[j] == copyText[i+matchStart]){
                        textToDecrypt[j] = toupper(copyCrib[i]);
                    }
                }
            }
        }
    }
    
    //if no match, then return false; nothing to decrypt
    else{
        return false;
    }
    
    //if match is found, decryption is performed
    //we can print out the (partially) decrypted ciphertext
    cout << textToDecrypt;
    return true;
}
    
//first time pattern of repeated letters is broken, return false
//return true if pattern matches == crib is officially found in the ciphertext
bool checkPattern(const char cribDup[], const char textDup[], int startHere){
    int lenOfCribCopy = strlen(cribDup);
    int firstAppear;
    
    //90 characters, max 90 element #'s (if all 90 characters are the same letter)
    int cribPositions[100];
    int textPositions[100];
    
    for(char findCribLetter = 'a'; findCribLetter <= 'z' ; findCribLetter++){
        //-1 = letter never found since no element # is negative
        firstAppear=-1;

        //if findCribLetter exists in crib copy, store position of its first appearance
        for(int j=0; j<lenOfCribCopy; j++){
            if(tolower(cribDup[j]) == findCribLetter){
                firstAppear=j;
                break;
            }
        }
        
        //ONLY if findCribLetter is found, create arrays for comparison
        if(firstAppear!=-1){
    
        //USE FIRST INT ARRAY to store all positions where findCribLetter is in crib
            int lenOfCrPos=0;
            for(int k=0; k<lenOfCribCopy; k++){
                if(tolower(cribDup[k])==findCribLetter){
                    cribPositions[lenOfCrPos]=k;
                    lenOfCrPos++;
                }
            }

            //determine findTextLetter
            //use matchStart --> startHere to make sure we are searching
            //for letters corresponding to copy of crib
            char findTextLetter = tolower(textDup[firstAppear+startHere]);

        //USE SECOND INT ARRAY to store all positions where findTextLetter is in ciphertext
            int lenOfTPos=0;
            for(int k=startHere; k<(lenOfCribCopy+startHere); k++){
                if(tolower(textDup[k])==findTextLetter){
                    textPositions[lenOfTPos]=k-startHere;        //store corresponding position of crib copy
                    lenOfTPos++;
                }
            }

            //if a letter does not repeat the same # of times, return false
            if(lenOfCrPos != lenOfTPos){
                return false;
            }
                
            //if a letter does not repeat in same places, return false
            for(int i=0; i<lenOfCrPos && i <lenOfTPos; i++){
                if(cribPositions[i] != textPositions[i])
                    return false;
            }
        } //if
    }//outer for loop
            
    return true;
}

#include "util.h"
#include <iostream>
using namespace std;

// Primitive helper utilities, clamping, and screen control

int clampInt(int val, int minVal, int maxVal){
    if(val>maxVal)
        return maxVal;
    else if(val<minVal)
        return minVal;
    else{
        return val;
    }
}
void trimWhitespace(char* str){
    int start =0;
    while (str[start] != '\0' &&
       (str[start] == '\r' || str[start] == '\n' ||
        str[start] == '\t' || str[start] == ' ')){
            ++start;
        }
        int len = 0;
    while (str[len] != '\0') len++;
        int end = len - 1;
    while (end >= start &&
       (str[end] == ' ' || str[end] == '\t' ||
        str[end] == '\n' || str[end] == '\r'))
    {
     end--;
    }
    int i=0;
    for(int j=start; j<=end;j++){
        str[i] = str[j];
        i++;
    }
    str[i] = '\0';
}
bool stringEquals(const char* a, const char* b){
    int i=0;

    while(a[i]!='\0' && b[i]!='\0'){
        if(a[i]!=b[i]){
            return false;
        }
        i++;
    }
    if (a[i] == b[i]) {
    return true;
    } else {
    return false;
    }
}
void printHeader(const char* title){
    cout<<"\n===="<<title<<"====\n";
}
void clearScreen(){
    cout << "\033[2J\033[1;1H";
}
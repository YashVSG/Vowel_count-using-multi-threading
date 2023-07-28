#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>

using namespace std;

#define NUM_THREADS 20

int a = 0, e = 0, i = 0, o = 0, u = 0;
HANDLE myMutex;

DWORD WINAPI vowelCount(LPVOID args) {
    ifstream inFile;
    string *p = (string *)args;
    string file = *p;
    delete p;
    
    WaitForSingleObject(myMutex, INFINITE);
    inFile.open(file.c_str());
    
    if (!inFile) {
        cout << "Unable to open " << file << endl;
        exit(1);
    }
    
    char vowel;
    while (inFile >> noskipws >> vowel) {
        if (vowel == 'a' || vowel == 'A')
            a++;
        else if (vowel == 'e' || vowel == 'E')
            e++;
        else if (vowel == 'i' || vowel == 'I')
            i++;
        else if (vowel == 'o' || vowel == 'O')
            o++;
        else if (vowel == 'u' || vowel == 'U')
            u++;
    }
    
    inFile.close();
    ReleaseMutex(myMutex);
    return 0;
}

int main() {
    HANDLE threadArray[NUM_THREADS];
    stringstream fileName;
    string *str_p;
    str_p = new string;
    myMutex = CreateMutex(NULL, FALSE, NULL);
    
    for (int n = 0; n < NUM_THREADS; n++) {
        /*
         TODO: Currently, this is hard coded in, files should be given by the user.
         fileName << "/directory/file";
         fileName << n + 1 << ".txt";
         */
        str_p = new string(fileName.str());
        threadArray[n] = CreateThread(NULL, 0, vowelCount, (LPVOID)str_p, 0, NULL);
        fileName.str("");
        fileName.clear();
    }
    
    WaitForMultipleObjects(NUM_THREADS, threadArray, TRUE, INFINITE);
    
    CloseHandle(myMutex);
    for (int n = 0; n < NUM_THREADS; n++) {
        CloseHandle(threadArray[n]);
    }
    
    cout << "A: " << a << endl;
    cout << "E: " << e << endl;
    cout << "I: " << i << endl;
    cout << "O: " << o << endl;
    cout << "U: " << u << endl;
    return 0;
}

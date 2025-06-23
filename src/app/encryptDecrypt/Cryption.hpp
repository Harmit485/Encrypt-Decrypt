#ifndef CRYPTION_HPP
#define CRYPTION_HPP

#include <string>

using namespace std;

int executeCryption(const string& taskData);
char encrypt(char ch, int eKey);
char decrypt(char ch, int dKey);

#endif
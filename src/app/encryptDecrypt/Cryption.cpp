#include <iostream>
#include <string>
#include <iomanip>
#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../fileHandling/ReadEnv.cpp"

char encrypt(char ch, int eKey){
    return (ch + eKey)%256;
}

char decrypt(char ch, int dKey){
    return (ch - dKey + 256)%256;
}

int executeCryption(const string& taskData){
    Task task = Task::fromString(taskData);

    ReadEnv env;
    string envKey = env.getEnv();
    int edKey = stoi(envKey);

    if(task.action == Action::ENCRYPT){
        char ch;

        // Reads, encrypt and overwrites each byte in-place one by one.
        while(task.stream.get(ch)){
            ch = encrypt(ch,edKey);
            task.stream.seekp(-1, ios::cur);
            task.stream.put(ch);
        }
        task.stream.close();
    }
    else if(task.action == Action::DECRYPT){
        char ch;

        // Reads, decrypt and overwrites each byte in-place one by one.
        while(task.stream.get(ch)){
            ch = decrypt(ch,edKey);
            task.stream.seekp(-1, ios::cur);
            task.stream.put(ch);
        }
        task.stream.close();
    }
    else{
        throw runtime_error("Invalid action type");
    }

    return 0;
}
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include "src/app/processes/ProcessManagement.hpp"
#include "src/app/processes/Task.hpp"

using namespace std;  
namespace fs = filesystem;            

int main(int argc, char* argv[]){
    string directory;
    string act; 

    cout << "Enter the directory path : ";
    getline(cin, directory);

    cout << "Enter the action (encryption/decryption) : ";
    getline(cin, act);

    auto start = chrono::high_resolution_clock::now();
    
    try{
        if(fs::exists(directory) && fs::is_directory(directory)){
            ProcessManagement processManagement;

            for(const auto& entry : fs::recursive_directory_iterator(directory)){
                if(entry.is_regular_file()){
                    string path = entry.path().string();
                    
                    IO io(path);
                    fstream stream = move(io.getFileStream());

                    if(stream.is_open()){
                        Action action = (act == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);
                        auto task = make_unique<Task> (move(stream), action, path);
                        
                        // cout << "Executing the Task : " << task->toString() << '\n';
                        processManagement.submitToQueue(move(task));
                    }
                    else{
                        cout << "Unable to open the file : " << path << '\n';
                    }
                }
            }
        }
        else{
            cout << "Invalid directory path" << '\n';
        }

    }catch(const fs::filesystem_error& e){
        cout << "Filesystem error : " << e.what() << '\n';
    }

    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> elapsed = end - start;

    cout << "Total execution time using [ Multithreading Version ] : " << elapsed.count() << " seconds\n";

    return 0;
}
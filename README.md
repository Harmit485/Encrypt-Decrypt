# 🔐 Encrypt-Decrypt

A **high-performance C++ file encryption and decryption system** with support for both **multithreading** and **multiprocessing**. It can efficiently process hundreds of files using shared memory and semaphores, making it ideal for benchmarking concurrent file operations.

![Architecture](https://github.com/Harmit485/Encrypt-Decrypt/blob/main/Performance/EnDe.png)

---

## 🚀 Features

- 🔄 **Encrypts and decrypts entire directories**
- ⚙️ **Sequential, Multithreaded & Multiprocessed execution**
- 🧠 **Shared Memory + Semaphores** for process synchronization
- 📁 Handles **999+ files** with ease (thanks to auto-generator)
- 🔑 Reads secret key from `.env` for enhanced security
- 🧪 Includes a Python test file generator
- 📦 Simple to build using `Makefile`

---

## 📂 Project Structure

```

Encrypt-Decrypt/
├── main.cpp                    # Main entry point
├── Makefile                    # Build automation
├── makeDirs.py                 # Generates 999 test files
├── src/
│   └── app/
│       ├── encryptDecrypt/
│       │   ├── Cryption.cpp   # Core encryption/decryption logic
│       │   ├── Cryption.hpp
│       │   └── CryptionMain.cpp
│       ├── fileHandling/
│       │   ├── IO.cpp         # File stream management
│       │   ├── IO.hpp
│       │   └── ReadEnv.cpp    # Reads key from .env file
│       └── processes/
│           ├── ProcessManagement.cpp  # Thread/Process manager
│           ├── ProcessManagement.hpp
│           └── Task.hpp       # Represents encryption/decryption task

````

---

## ⚙️ How It Works

1. You run the program and specify a **directory path** and **action** (`ENCRYPT` or `DECRYPT`).
2. It recursively finds all text files and pushes them into a **shared task queue**.
3. **Worker threads or child processes** pick up tasks and perform the specified action using a secret key from `.env`.
4. The result is a fully encrypted or decrypted directory, ready to use.

---

## 📸 Performance Comparison

The following benchmarks were taken using 999 test files in a directory named `test/` on a MacBook Air (M2). Each mode was timed independently.

| Execution Mode      | Action     | Time (seconds) |
|---------------------|------------|----------------|
| V1 - Sequential     | ENCRYPT    | 2.25967        |
| V1 - Sequential     | DECRYPT    | 2.31632        |
| V2 - Multiprocessing| ENCRYPT    | 1.50106        |
| V2 - Multiprocessing| DECRYPT    | 1.51997        |
| V3 - Nested         | ENCRYPT    | 1.38807        |
| V3 - Nested         | DECRYPT    | 1.69876        |


📷 Screenshots of terminal outputs:

![Sequential (Single)](https://github.com/Harmit485/Encrypt-Decrypt/blob/main/Performance/Sequential.png)
![Multiprocessing](https://github.com/Harmit485/Encrypt-Decrypt/blob/main/Performance/Multiprocessing.png)
![Multithreading](https://github.com/Harmit485/Encrypt-Decrypt/blob/main/Performance/Multithreading.png)

> ✅ **V3 - Multiprocessing with Nested Multithreading is the fastest overall**, with up to 40% improvement over sequential processing.

---

## 🔧 Build Instructions

### Prerequisites
- C++17 or later
- Python 3 (for test file generation)

### 🛠️ Build the Project

```bash
make
````

### 🧪 Generate Test Files

```bash
python3 makeDirs.py
```

---

## ▶️ Usage

```bash
./encrypt_decrypt
```

Then follow the prompts:

```
Enter the directory path : test
Enter the action (encryption/decryption) : ENCRYPT/DECRYPT
```

---

## 🔍 Internals Breakdown

| Component           | Purpose                                                             |
| ------------------- | ------------------------------------------------------------------- |
| `Cryption`          | File-level byte-wise encryption/decryption                          |
| `ProcessManagement` | Spawns worker threads/processes, manages shared memory + semaphores |
| `Task`              | Serializable structure representing a file + operation              |
| `IO`                | Handles safe opening and management of file streams                 |
| `ReadEnv`           | Reads the key from the `.env` file for encryption logic             |

---

## 🤓 Example

Original `test003.txt`:

```
HelloWorld123
```

After encryption with `KEY=5`:

```
MjqqtBtwqi678
```

---

## 💡 Fun Fact

This project can handle 999+ files concurrently using shared memory and semaphores — just run the generator and enjoy watching your CPU flex its muscles.

---

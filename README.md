# File encryption/decryption c++ tool

###

<div align="center">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" height="150" alt="cplusplus logo"  />
</div>

###

## Project structure:

```rust
├── LICENSE
├── fcrypt.cpp
└── README.md
```

## Installation

```shell
git clone https://github.com/kenjitheman/fcryptcpp
```

## Usage

- **Modify main function to use this tool**

```c++
// your key must be 16 bytes for AES-128
std::string key = "mysecretkey12345"; // your secret key

std::string inputFile = "input.txt";
std::string encryptedFile = "encrypted.bin";

std::string decryptedFile = "decrypted.txt";
```

- Compile c++ and include the OpenSSL library:

```shell
g++ fcrypt.cpp -o fcrypt -lssl -lcrypto
```

- Run:

```shell
./fcrypt
```

## Contributing

- Pull requests are welcome, for major changes, please open an issue first to
  discuss what you would like to change.

## License

- [MIT](https://choosealicense.com/licenses/mit/)


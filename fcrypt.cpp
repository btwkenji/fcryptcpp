#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string>
#include <vector>

bool EncryptFile(const std::string &key, const std::string &inputFile,
        const std::string &outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "[ERROR] could not open input file" << std::endl;
        return false;
    }
    std::vector<char> plaintext((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
    inFile.close();

    unsigned char iv[EVP_MAX_IV_LENGTH];
    if (!RAND_bytes(iv, EVP_MAX_IV_LENGTH)) {
        std::cerr << "[ERROR] could not generate IV" << std::endl;
        return false;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "[ERROR] EVP_CIPHER_CTX_new failed" << std::endl;
        return false;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), nullptr,
                reinterpret_cast<const unsigned char *>(key.c_str()),
                iv) != 1) {
        std::cerr << "[ERROR] EVP_EncryptInit_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::vector<char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
    int ciphertextLength = 0;

    if (EVP_EncryptUpdate(
                ctx, reinterpret_cast<unsigned char *>(ciphertext.data()),
                &ciphertextLength,
                reinterpret_cast<const unsigned char *>(plaintext.data()),
                plaintext.size()) != 1) {
        std::cerr << "[ERROR] EVP_EncryptUpdate failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_EncryptFinal_ex(ctx,
                reinterpret_cast<unsigned char *>(ciphertext.data()) +
                ciphertextLength,
                &ciphertextLength) != 1) {
        std::cerr << "[ERROR] EVP_EncryptFinal_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertextLength);

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "[ERROR] could not open output file" << std::endl;
        return false;
    }

    outFile.write(reinterpret_cast<const char *>(iv), EVP_MAX_IV_LENGTH);
    outFile.write(ciphertext.data(), ciphertext.size());
    outFile.close();

    return true;
}

bool DecryptFile(const std::string &key, const std::string &inputFile,
        const std::string &outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "[ERROR] could not open input file" << std::endl;
        return false;
    }

    unsigned char iv[EVP_MAX_IV_LENGTH];
    inFile.read(reinterpret_cast<char *>(iv), EVP_MAX_IV_LENGTH);

    std::vector<char> ciphertext((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
    inFile.close();

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << "[ERROR] EVP_CIPHER_CTX_new failed" << std::endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), nullptr,
                reinterpret_cast<const unsigned char *>(key.c_str()),
                iv) != 1) {
        std::cerr << "[ERROR] EVP_DecryptInit_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    std::vector<char> plaintext(ciphertext.size() + EVP_MAX_BLOCK_LENGTH);
    int plaintextLength = 0;

    if (EVP_DecryptUpdate(
                ctx, reinterpret_cast<unsigned char *>(plaintext.data()),
                &plaintextLength,
                reinterpret_cast<const unsigned char *>(ciphertext.data()),
                ciphertext.size()) != 1) {
        std::cerr << "[ERROR] EVP_DecryptUpdate failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_DecryptFinal_ex(ctx,
                reinterpret_cast<unsigned char *>(plaintext.data()) +
                plaintextLength,
                &plaintextLength) != 1) {
        std::cerr << "[ERROR] EVP_DecryptFinal_ex failed" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintextLength);

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "[ERROR] could not open output file" << std::endl;
        return false;
    }

    outFile.write(plaintext.data(), plaintext.size());
    outFile.close();

    return true;
}

int main() {
    // your key must be 16 bytes for AES-128
    std::string key = "mysecretkey12345"; // your secret key

    std::string inputFile = "input.txt";
    std::string encryptedFile = "encrypted.bin";
    std::string decryptedFile = "decrypted.txt";

    if (EncryptFile(key, inputFile, encryptedFile)) {
        std::cout << "[SUCCESS] file encrypted successfully" << std::endl;
    } else {
        std::cerr << "[ERROR] file encryption failed" << std::endl;
        return 1;
    }

    if (DecryptFile(key, encryptedFile, decryptedFile)) {
        std::cout << "[SUCCESS] file decrypted successfully" << std::endl;
    } else {
        std::cerr << "[ERROR] file decryption failed" << std::endl;
        return 1;
    }

    return 0;
}

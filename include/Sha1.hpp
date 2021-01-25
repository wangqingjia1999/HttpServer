/**
 * This module is totally copied from https://github.com/rhymu8354/Sha1
 * Honestly, I do not understand the SHA-1 algorithm.
 * Richard Walters (github username: rhymu8354) is my teacher (although he doesn't know me).
 * I've learnt a lot from him. Highly recommended to follow him, if you like.
 * I will take some time to figure out this amazing algorithm.
 */
#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace Sha1
{
    /**
     * Encrypt given bytes vector by SHA-1 algorithm.
     * 
     * @param[in] data  
     *      Data in the form of bytes sequence/vector.
     * 
     * @return  
     *      The message digest in the form of string.
     */
    std::string sha1_encrypt(const std::vector< uint8_t >& data);

    /**
     * Encrypt given data string by SHA-1 algorithm.
     * 
     * @param[in] data  
     *      Data in the form of string.
     * 
     * @return  
     *      The message digest in the form of string.
     */
    std::string sha1_encrypt(const std::string& data);

    /**
     * Core function to convert given data into message digest in the form of bytes.
     * 
     * @param[in] data  
     *      Data in the form of bytes sequence/vector.
     * 
     * @return  
     *      The message digest bytes sequence.
     */
    std::vector< uint8_t > sha1_encrypt_into_bytes(const std::vector< uint8_t >& data);

    /**
     * Core function to convert given data into message digest in the form of bytes.
     * 
     * @param[in] data  
     *      Data in the form of string.
     * 
     * @return  
     *      The message digest bytes sequence.
     */
    std::vector< uint8_t > sha1_encrypt_into_bytes(const std::string& data);
}

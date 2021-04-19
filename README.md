[![MIT License](https://img.shields.io/apm/l/atomic-design-ui.svg?)](https://github.com/tterb/atomic-design-ui/blob/master/LICENSEs)
# Shamir-Secret-Sharing

<img src="https://img.shields.io/badge/c%20-%2300599C.svg?&style=plastic&logo=c&logoColor=white"/> ![GitHub](https://img.shields.io/badge/-GitHub-181717?style=plastic&logo=github)

A C implementation constricting to the Galois Field(256) of the following [**problem**](https://www.codechef.com/IPTS2019/problems/CTS1).

## Installation

* Run the helper bash script to generate binaries for the secret sharing algorithm :

      ./helper_script.sh

## Usage

### Encryption

* The code has limited functionality (which can be easily modified) as it is tuned to match the problem statement. Hence, it will always split into four keys with a linear polynomial :

      ./bin/SSS encrypt

This is will point to user to enter a string and will generate keys once the Enter key is pressed :

      Enter string: Hello
 
      KEYS = 
      bfbb38ddd96df8
      31b6634427d3aa
      b8a18ef43e0a5c
      4f3981b0003757
      
### Decryption

* Enter any of the two encrypted keys separated by a space, this will then decode the message , once you press 'Enter':

      ./bin/SSS decrypt bfbb38ddd96df8 31b6634427d3aa

      Decoded String: Hello

## Wiki

Refer to the [**Wiki**](https://github.com/leander-dsouza/Shamir-Secret-Sharing/wiki) page to learn more about Shamir Secret Sharing.












###### 💾 EOF


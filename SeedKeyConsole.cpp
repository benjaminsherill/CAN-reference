// SeedKeyConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>

unsigned char vciRxBuffer[8], vciTxBuffer[8];
unsigned char seed[8], key[8];
unsigned int FlashProgrammingSessionV1[4] = { 0x11651145, 0x1FFF3242, 0x3A5211D4, 0x3FD21DAB };
unsigned int FlashProgrammingSessionV2[4] = { 0x21742054, 0x2E0F4151, 0x496121E3, 0x4EE12EBB };
unsigned int FlashProgrammingSessionV4[4] = { 0x34957B34, 0x846BA548, 0x397FBFED, 0x324BAEF9 };
unsigned int FlashProgrammingSessionV6[4] = { 0x850E70AF, 0x0EFEAC58, 0xC49357F8, 0x91F2ECAE };
unsigned int FlashProgrammingSessionV7[4] = { 0x59341802, 0xBAFD48EC, 0x11AA98DA, 0x69475810 };


unsigned int pattern[4] = { 0x850E70AF, 0x0EFEAC58, 0xC49357F8, 0x91F2ECAE };

void SeedkeyAlgorithm(unsigned int* AKeys, unsigned char* pSeed, unsigned char* pKey) {
    unsigned int dwPos, dwConst, dwTemp, dwSeed1, dwSeed2;
    int i;

    dwPos = 0;
    dwConst = 0x9E3779B9;
    //make seed in int32
    dwSeed1 = pSeed[0];
    dwSeed1 <<= 8;
    dwSeed1 |= pSeed[1];
    dwSeed1 <<= 8;
    dwSeed1 |= pSeed[2];
    dwSeed1 <<= 8;
    dwSeed1 |= pSeed[3];

    dwSeed2 = pSeed[4];
    dwSeed2 <<= 8;
    dwSeed2 |= pSeed[5];
    dwSeed2 <<= 8;
    dwSeed2 |= pSeed[6];
    dwSeed2 <<= 8;
    dwSeed2 |= pSeed[7];

    for (i = 0; i < 0x20; i++) {
        dwTemp = ((dwSeed2 >> 5) ^ (dwSeed2 << 4)) + dwSeed2;
        dwTemp = dwTemp ^ (AKeys[dwPos % 4] + dwPos);
        dwSeed1 = dwSeed1 + dwTemp;

        dwPos = dwPos + dwConst;

        dwTemp = ((dwSeed1 >> 5) ^ (dwSeed1 << 4)) + dwSeed1;
        dwTemp = dwTemp ^ (AKeys[(dwPos >> 0x0B) % 4] + dwPos);
        dwSeed2 = dwSeed2 + dwTemp;
    }

    //make key result in byte array
    pKey[0] = (dwSeed1 >> 24) & 0xFF;
    pKey[1] = (dwSeed1 >> 16) & 0xFF;
    pKey[2] = (dwSeed1 >> 8) & 0xFF;
    pKey[3] = (dwSeed1 >> 0) & 0xFF;
    pKey[4] = (dwSeed2 >> 24) & 0xFF;
    pKey[5] = (dwSeed2 >> 16) & 0xFF;
    pKey[6] = (dwSeed2 >> 8) & 0xFF;
    pKey[7] = (dwSeed2 >> 0) & 0xFF;
}

int main(int argc, char* argv[])
{
    //std::cout << "Hello World!\n";
    unsigned char seed[8], key[8];
    unsigned char pTemp[6], nHi, nLow;

    if (argc != 2)
        return 0;
    const char* pp = argv[1];
    //const char* pp = "821A90F4B96D748F";

    long int value;
    unsigned short real;
    for (int i = 0; i < 4; i++)
    {
        memset(pTemp, 0, 6);
        memcpy(pTemp, pp + 4*i, 4);
        char* endptr;
        value = strtol((const char*)pTemp, &endptr, 16);
        if (*endptr != '\0')
        {
            return 0;
        }
        //printf("The hexadecimal %s is %ld in decimal.\n", pTemp, value);
        real = value & 0xFFFF;
        nHi = (real >> 8) & 0xFF;
        nLow = real & 0xFF;
        seed[i * 2] = nHi;
        seed[i * 2 + 1] = nLow;
        //printf("%02X,%02X", nHi, nLow);
    }
/*
    seed[0] = 0x82;
    seed[1] = 0x1A;
    seed[2] = 0x90;
    seed[3] = 0xF4;
    seed[4] = 0xB9;
    seed[5] = 0x6D;
    seed[6] = 0x74;
    seed[7] = 0x8F;
*/
    SeedkeyAlgorithm(pattern, seed, key);
    //std::cout << key;
    for(int i=0;i<8;i++)
        printf("%02X", key[i]);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

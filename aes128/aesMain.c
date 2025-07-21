#include <stdio.h>   // <- Standart lib
#include <string.h>  // <- Able to use string
#include <stdlib.h>  // <- Memory alloc functions.
#include <unistd.h>  // <- Used for ssize_t
#include <conio.h>
#include "info.h" // <- Func to print out what sys using, for debug.
#include "aes.h" // <- Header file with all the AES functions.

/*
Note: Could be improved with multipe threads.
Compile: gcc -o {name.exe} aesMain.c aes.c 
*/
#define SECRET_KEY "123456789012345" 

int key_input(const char *promt, char *userKey);
int main()
{
    print_system_info();
    char *message = "Encrypt me now!! or else!";
    uint8_t *pMessageHex = NULL;            // <- Text in hex
    size_t numBlocks = 0;                   
    uint8_t roundKeys[AES_ROUND_KEYS_SIZE]; 
    printf("The message is being encrypted. \n");

    // Prep the message.
    prepare_Block(message, &pMessageHex, &numBlocks);
    printf("Current action ended: prepare_Block. \n");
    printf("Padded message in hex: ");
    for (size_t i = 0; i < numBlocks * AES_BLOCK_SIZE; i++)
    {
        printf("%02x ", pMessageHex[i]);
        if ((i + 1) % AES_BLOCK_SIZE == 0)
            printf("\n");
    }

    // Prep the key.
    printf("The key is being expanded. \n");
    key_Expansion(SECRET_KEY, roundKeys);
    for (int i = 0; i < AES_ROUND_KEYS_SIZE; i++)
    {
        printf("%02x ", roundKeys[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }

    printf("The message in HEX is being encrypted. \n");
    // Encrypting the Hex message / Final encrypt
    printf("The message in HEX: ");
    for (size_t i = 0; i < numBlocks; i++)
    {
        aes_Encrypt(pMessageHex + (i * AES_BLOCK_SIZE), roundKeys);
    }

    for (int i = 0; i < AES_BLOCK_SIZE * numBlocks; i++)
    {
        printf("%02x", pMessageHex[i]);
    }
    printf("\n");

    printf("\n");
    printf("\n");

    // Decrypting.
    printf("The dycrpyting process begun!");
    for (size_t i = 0; i < numBlocks; i++)
    {
        de_Crypt(pMessageHex + (i * AES_BLOCK_SIZE), roundKeys);
    }
    printf("\n");

    printf("Message decrypted! \n");
    printf("The message in HEX (ASCII): ");
    for (int i = 0; i < AES_BLOCK_SIZE * numBlocks; i++)
    {
        printf("%02x", pMessageHex[i]);
    }
    printf("\n");
    revert_Hex(&pMessageHex);

    printf("Freeing the allocated message memory. \n");
    free(pMessageHex);
    return 0;
}




int key_input(const char *promt, char *userKey)
{
    int count = 0;
    char ch;

    printf("%s (type 'quit' to exit):\n", promt);
    printf("> ");

      while (1) {
        ch = _getch();

        if (ch == 13) {  
            userKey[count] = '\0';
            break;
        }

        if ((ch == 8 || ch == 127) && count > 0) {  
            count--;
            printf("\r> ");          
            for (int i = 0; i < count; i++)
                putchar(userKey[i]); 
            printf(" ");             
            printf(" [%d/%d]", count, AES_BLOCK_SIZE);
            fflush(stdout);
            continue;
        }

        if (count >= AES_BLOCK_SIZE) {
            continue;
        }

        userKey[count++] = ch;
        printf("\r> ");
        for (int i = 0; i < count; i++) {
            putchar(userKey[i]);
        }
        printf(" [%d/%d]  ", count, AES_BLOCK_SIZE); 
        fflush(stdout);
    }

    printf("\n");

    if (strcmp(userKey, "quit") == 0) {
        return 0;
    }

    if (count != AES_BLOCK_SIZE) {
        printf("Error: Key must be exactly %d characters.\n", AES_BLOCK_SIZE);
        return -1;
    }

    printf("Key accepted.\n");
    return 1;
}

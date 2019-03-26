/*
 * Created by @GeraGalindo on 03/26/2019
 *
 * */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <signal.h>

#include "logger.h"

/*
 * This Program encodes and decodes using a base64 algorithm based on:
 *
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 *
 * Additionally it registers SIGINT and SIGPWR signals so that they can handled.
 *
 * When a signal is caught, the progress of the encoding/decoding is printed.
 *
 * Note:
 * Since the encoding/decoding is relatively fast, we cannot send SIGINT (Ctl+c) or SIGPWR (Clt+t)
 * Therefore, I am sending the raising the signals with raise(sig_number) System Call.
 *
 * Similarly we could create/fork another process to raise those signals but for demonstration
 * purposes I decided to use the raise() system call arbitrarily.
 *
 * */


#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

void printUsage();
int base64encode(const void* data_buf, ssize_t dataLength, char* result, size_t resultSize);
int base64decode (char *in, ssize_t inLen, unsigned char *out, size_t *outLen);
void encodeFile(const char *fileName);
void decodeFile(const char *fileName);
void signalHandler(int);
int getNumberOfLines(const char *fileName);

int readLines;
int numberLines = 0;

int main(int argc, char *argv[]) {
    initLogger("STDOUT");
    infof("Starting base64 program\n");

    // Signal Registration SIGINT
    if(signal(SIGINT, signalHandler) == SIG_ERR){
        errorf("Unable to set the SIGINT signal handler\n");
        exit(1);
    }

    // Signal Registration SIGINFO = SIGPWR
    if(signal(SIGPWR, signalHandler) == SIG_ERR){
        errorf("Unable to set the SIGPWR signal handler\n");
        exit(1);
    }

    if (argc != 3){
        printUsage();
    } else {
        if (!strcmp("--encode", argv[1])){
            getNumberOfLines(argv[2]);
            encodeFile(argv[2]);
        }
        if (!strcmp("--decode", argv[1])){
            getNumberOfLines(argv[2]);
            decodeFile(argv[2]);
        }
    }

    return 0;
}

void printUsage(){
    errorf("\nUsage:\n"
          "./base64 --encode <input_file>     (To encode a file using the base64 algorithm)\nOr:\n"
          "./base64 --decode <encoded_file>   (To decode a file using the base64 algorithm)\n");
}

int getNumberOfLines(const char *fileName){
    char* buffer;
    ssize_t charCount;
    size_t buffSize = 256;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL){
        errorf("Unable to open file: %s\n", fileName);
        exit(1);
    }

    buffer = (char*)malloc(buffSize * sizeof(char));
    if (buffer == NULL){
        errorf("Unable to allocate buffer\n");
        exit(1);
    }

    do{
        charCount = getline(&buffer, &buffSize, fp);
        ++numberLines;
    } while (charCount >= 0);

    return numberLines;
}

void encodeFile(const char *fileName){
    infof("Encoding file: %s\n", fileName);
    char* buffer;
    char* outBuffer;
    size_t buffSize = 128;
    ssize_t charCount;
    FILE *ifp;
    FILE *ofp;

    readLines = 0;

    ifp = fopen(fileName, "r");
    ofp = fopen("encoded.txt", "w");
    if(ifp != NULL && ofp != NULL){
        buffer = (char*)malloc(buffSize * sizeof(char));
        outBuffer = (char*)malloc(buffSize * sizeof(char));
        if(buffer == NULL || outBuffer == NULL){
            errorf("Unable to allocate buffers");
            exit(1);
        }

        charCount = getline(&buffer, &buffSize, ifp);
        ++readLines;

        while(charCount >= 0){
            base64encode(buffer, charCount, outBuffer, buffSize);
            fprintf(ofp, "%s\n", outBuffer);
            charCount = (size_t) getline(&buffer, &buffSize, ifp);
            ++readLines;
            if (!(readLines%1000)){
                raise(SIGINT);
            }
        }
        raise(SIGPWR);
        free(buffer);
        free(outBuffer);
        fclose(ofp);
        fclose(ifp);
    }
    infof("Done Encoding File: %s\n", fileName);
}

void decodeFile(const char *fileName){
    infof("Decoding file: %s\n", fileName);
    FILE *ifp;
    FILE *ofp;
    char* inBuffer;
    unsigned char* outBuffer;
    size_t buffSize = 128;
    ssize_t charCount;

    readLines = 0;

    ifp = fopen(fileName, "r");
    ofp = fopen("decoded.txt", "w");
    if(ifp != NULL && ofp != NULL){
        inBuffer = (char*)malloc(buffSize * sizeof(char));
        if(inBuffer == NULL){
            errorf("Unable to allocate input buffer\n");
            exit(1);
        }

        charCount = getline(&inBuffer, &buffSize, ifp);
        ++readLines;

        while(charCount >= 0){
            outBuffer = (unsigned char*)calloc(256, sizeof(unsigned char));
            if (outBuffer == NULL){
                errorf("Unable to allocate output buffer\n");
                exit(1);
            }
            base64decode(inBuffer, charCount, outBuffer, &buffSize);
            fprintf(ofp, "%s", outBuffer);
            charCount = (size_t) getline(&inBuffer, &buffSize, ifp);
            free(outBuffer);
            ++readLines;
            if (!(readLines%1000)){
                raise(SIGINT);
            }
        }
        raise(SIGPWR);
        free(inBuffer);
        fclose(ofp);
        fclose(ifp);
    }
    infof("Done Decoding File: %s\n", fileName);
}

void signalHandler(int signal){
    int progress = readLines*100/(numberLines-1);
    switch (signal){
        case SIGPWR:
            infof("SIGPWR Signal Caught\n");
            infof("Progress: %d percent\n", progress);
            break;
        case SIGINT:
            infof("SIGINT Signal Caught\n");
            infof("Progress: %d percent\n", progress);
            break;
        default:
            infof("Other Signal Caught\n");
            break;
    }
}

/*
 * This block of code was copied from the link below and slightly modified to fit our needs
 *
 * Copied from https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 *
 * Block of code starts here...
 *
 * */

int base64encode(const void* data_buf, ssize_t dataLength, char* result, size_t resultSize)
{
    const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t *data = (const uint8_t *)data_buf;
    size_t resultIndex = 0;
    size_t x;
    uint32_t n = 0;
    int padCount = dataLength % 3;
    uint8_t n0, n1, n2, n3;

    /* increment over the length of the string, three characters at a time */
    for (x = 0; x < dataLength; x += 3)
    {
        /* these three 8-bit (ASCII) characters become one 24-bit number */
        n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if((x+1) < dataLength)
            n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

        if((x+2) < dataLength)
            n += data[x+2];

        /* this 24-bit number gets separated into four 6-bit numbers */
        n0 = (uint8_t)(n >> 18) & 63;
        n1 = (uint8_t)(n >> 12) & 63;
        n2 = (uint8_t)(n >> 6) & 63;
        n3 = (uint8_t)n & 63;

        /*
         * if we have one byte available, then its encoding is spread
         * out over two characters
         */
        if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
        result[resultIndex++] = base64chars[n0];
        if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
        result[resultIndex++] = base64chars[n1];

        /*
         * if we have only two bytes available, then their encoding is
         * spread out over three chars
         */
        if((x+1) < dataLength)
        {
            if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
            result[resultIndex++] = base64chars[n2];
        }

        /*
         * if we have all three bytes available, then their encoding is spread
         * out over four characters
         */
        if((x+2) < dataLength)
        {
            if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
            result[resultIndex++] = base64chars[n3];
        }
    }

    /*
     * create and add padding that is required if we did not have a multiple of 3
     * number of characters available
     */
    if (padCount > 0)
    {
        for (; padCount < 3; padCount++)
        {
            if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
            result[resultIndex++] = '=';
        }
    }
    if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
    result[resultIndex] = 0;
    return 0;   /* indicate success */
}

static const unsigned char d[] = {
        66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
        66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
        54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
        29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
        66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
        66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
        66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
        66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
        66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
        66,66,66,66,66,66
};

int base64decode (char *in, ssize_t inLen, unsigned char *out, size_t *outLen) {
    char *end = in + inLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;

    while (in < end) {
        unsigned char c = d[*in++];

        switch (c) {
            case WHITESPACE: continue;   /* skip whitespace */
            case INVALID:    return 1;   /* invalid input, return error */
            case EQUALS:                 /* pad character, end of data */
                in = end;
                continue;
            default:
                buf = buf << 6 | c;
                iter++; // increment the number of iteration
                /* If the buffer is full, split it into bytes */
                if (iter == 4) {
                    if ((len += 3) > *outLen) return 1; /* buffer overflow */
                    *(out++) = (buf >> 16) & 255;
                    *(out++) = (buf >> 8) & 255;
                    *(out++) = buf & 255;
                    buf = 0; iter = 0;

                }
        }
    }

    if (iter == 3) {
        if ((len += 2) > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len; /* modify to reflect the actual output size */
    return 0;
}

/*
 * Block of code finishes here...
 *
 * Copied from https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 *
 * */
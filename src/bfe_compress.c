#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT 1048576 // Match bfe.c max message size
#define MAX_OUTPUT (MAX_INPUT * 12) // Account for [4294967296] (10 digits + brackets)

// Compress the input string (e.g., >>>>+<<+ to [4]>+[2]<+)
char *compress(char *input) {
    char *output = malloc(MAX_OUTPUT + 1);
    if (!output) return NULL;
    
    size_t out_idx = 0;
    size_t i = 0;
    while (input[i] != '\0') {
        if (input[i] == '+' && out_idx < MAX_OUTPUT) {
            output[out_idx++] = '+';
            i++;
        } else if ((input[i] == '>' || input[i] == '<') && out_idx < MAX_OUTPUT) {
            char dir = input[i];
            unsigned long count = 0;
            while (input[i] == dir && input[i] != '\0') {
                count++;
                i++;
            }
            // Write [count]dir
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "[%lu]%c", count, dir);
            for (size_t j = 0; buffer[j] != '\0' && out_idx < MAX_OUTPUT; j++) {
                output[out_idx++] = buffer[j];
            }
        } else {
            free(output);
            return NULL; // Invalid character
        }
    }
    output[out_idx] = '\0';
    return output;
}

// Decompress the input string (e.g., [4]>+[2]<+ to >>>>+<<+)
char *decompress(char *input) {
    char *output = malloc(MAX_INPUT + 1);
    if (!output) return NULL;
    
    size_t out_idx = 0;
    size_t i = 0;
    while (input[i] != '\0' && out_idx < MAX_INPUT) {
        if (input[i] == '+') {
            output[out_idx++] = '+';
            i++;
        } else if (input[i] == '[') {
            i++;
            // Parse number
            char num_buf[32];
            size_t num_idx = 0;
            while (isdigit(input[i]) && num_idx < sizeof(num_buf) - 1) {
                num_buf[num_idx++] = input[i++];
            }
            num_buf[num_idx] = '\0';
            if (input[i] != ']') {
                free(output);
                return NULL; // Malformed input
            }
            i++;
            if (input[i] != '>' && input[i] != '<') {
                free(output);
                return NULL; // Invalid direction
            }
            char dir = input[i++];
            unsigned long count = strtoul(num_buf, NULL, 10);
            if (count > MAX_INPUT - out_idx) {
                free(output);
                return NULL; // Output would exceed buffer
            }
            for (unsigned long j = 0; j < count; j++) {
                output[out_idx++] = dir;
            }
        } else {
            free(output);
            return NULL; // Invalid character
        }
    }
    output[out_idx] = '\0';
    return output;
}

// Print usage information
void print_usage() {
    printf("\nBrainFuckEncryption Compressor (bfe_compress)\n");
    printf("Usage: bfe_compress --input <string> [--compress | --decompress]\n");
    printf("  --input <string>: String to compress or decompress\n");
    printf("  --compress: Compress the input (e.g., >>>>+<<+ to [4]>+[2]<+)\n");
    printf("  --decompress: Decompress the input (e.g., [4]>+[2]<+ to >>>>+<<+)\n");
    printf("\nExamples:\n");
    printf("  bfe_compress --input \">>>>+<<+\" --compress\n");
    printf("  bfe_compress --input \"[4]>+[2]<+\" --decompress\n");
}

int main(int argc, char *argv[]) {
    char *input = NULL;
    int compress_mode = 0;
    int decompress_mode = 0;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            input = argv[++i];
        } else if (strcmp(argv[i], "--compress") == 0) {
            compress_mode = 1;
        } else if (strcmp(argv[i], "--decompress") == 0) {
            decompress_mode = 1;
        } else {
            print_usage();
            printf("Error: Invalid argument '%s'\n", argv[i]);
            return 1;
        }
    }

    // Validate arguments
    if (!input || (compress_mode + decompress_mode) != 1) {
        print_usage();
        printf("Error: --input and exactly one of --compress or --decompress required\n");
        return 1;
    }

    if (strlen(input) > MAX_INPUT) {
        printf("Error: Input too long (max %d characters)\n", MAX_INPUT);
        return 1;
    }

    // Process input
    char *result = NULL;
    if (compress_mode) {
        result = compress(input);
        if (!result) {
            printf("Error: Invalid input for compression (must contain only '>', '<', '+')\n");
            return 1;
        }
    } else {
        result = decompress(input);
        if (!result) {
            printf("Error: Invalid input for decompression (e.g., [number]>[+][number]<[+])\n");
            return 1;
        }
    }

    // Print result
    printf("\nInput: %s\n", input);
    printf("Output: %s\n\n", result);

    // Clean up
    free(result);
    return 0;
}
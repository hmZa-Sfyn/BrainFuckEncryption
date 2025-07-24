#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_MESSAGE 1048576 // 1MB for large messages
#define DEFAULT_KEY_VECTOR_SIZE 62
#define MAX_KEY_SIZE 256

// Function to initialize the default key vector
void init_default_key_vector(char *key_vector) {
    int i = 0;
    // Lowercase letters
    for (char c = 'a'; c <= 'z'; c++) key_vector[i++] = c;
    // Numbers
    for (char c = '0'; c <= '9'; c++) key_vector[i++] = c;
    // Uppercase letters
    for (char c = 'A'; c <= 'Z'; c++) key_vector[i++] = c;
    key_vector[i] = '\0';
}

// Function to generate a random key vector
void generate_random_key_vector(char *key_vector, int *size) {
    char pool[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int pool_size = strlen(pool);
    *size = 10 + rand() % (MAX_KEY_SIZE - 10); // Random size between 10 and MAX_KEY_SIZE
    for (int i = 0; i < *size; i++) {
        key_vector[i] = pool[rand() % pool_size];
    }
    key_vector[*size] = '\0';
}

// Function to validate custom key vector
int validate_key_vector(char *key_vector, int *size) {
    int len = strlen(key_vector);
    if (len == 0 || len > MAX_KEY_SIZE) return 0;
    *size = len;
    return 1;
}

// Find position of character in key vector
int find_char_pos(char c, char *key_vector, int size) {
    for (int i = 0; i < size; i++) {
        if (key_vector[i] == c) return i;
    }
    return -1; // Character not found
}

// Encrypt the message
void encrypt(char *message, int sPos, char *key_vector, int key_size, char *output) {
    int current_pos = sPos;
    size_t output_index = 0;
    
    for (size_t i = 0; message[i] != '\0'; i++) {
        int target_pos = find_char_pos(message[i], key_vector, key_size);
        if (target_pos == -1) continue; // Skip invalid characters
        
        // Always move forward (using >)
        int distance = (target_pos >= current_pos) ? 
                       target_pos - current_pos : 
                       key_size - current_pos + target_pos;
        
        for (int j = 0; j < distance; j++) {
            output[output_index++] = '>';
        }
        
        output[output_index++] = '+';
        current_pos = target_pos;
    }
    output[output_index] = '\0';
}

// Print usage information
void print_usage() {
    printf("\nBrainFuckEncryption (bfe) - A simple encryption system\n");
    printf("Usage: bfe --message <message> [--spos <starting_position>] [--kvec <custom_key_vector>]\n");
    printf("  --message <text>: The text to encrypt (max %d characters)\n", MAX_MESSAGE);
    printf("  --spos <number>: Starting position in key vector (0 to key_vector_size-1, default: random)\n");
    printf("  --kvec <string>: Custom key vector (default: random or a-z,0-9,A-Z)\n");
    printf("\nExamples:\n");
    printf("  bfe --message \"hello worlds!\"                    # Random sPos and key vector\n");
    printf("  bfe --message \"hello worlds!\" --spos 3          # Specified sPos, default key vector\n");
    printf("  bfe --message \"hello\" --spos 0 --kvec \"abc123\" # Specified sPos and custom key vector\n");
    printf("\nOutput format: Uses '>' for movement, '+' for character selection\n");
}

int main(int argc, char *argv[]) {
    // Seed random number generator
    srand(time(NULL));

    // Variables for arguments
    char *message = NULL;
    int sPos = -1;
    char *custom_kvec = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--message") == 0 && i + 1 < argc) {
            message = argv[++i];
        } else if (strcmp(argv[i], "--spos") == 0 && i + 1 < argc) {
            sPos = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--kvec") == 0 && i + 1 < argc) {
            custom_kvec = argv[++i];
        } else {
            print_usage();
            printf("Error: Invalid argument '%s'\n", argv[i]);
            return 1;
        }
    }

    // Check if message was provided
    if (!message) {
        print_usage();
        printf("Error: --message is required\n");
        return 1;
    }

    // Validate message length
    if (strlen(message) > MAX_MESSAGE) {
        printf("Error: Message too long (max %d characters)\n", MAX_MESSAGE);
        return 1;
    }

    // Initialize key vector
    char key_vector[MAX_KEY_SIZE];
    int key_size;

    if (custom_kvec) {
        // Use custom key vector
        if (!validate_key_vector(custom_kvec, &key_size)) {
            printf("Error: Invalid custom key vector\n");
            return 1;
        }
        strncpy(key_vector, custom_kvec, MAX_KEY_SIZE);
    } else {
        // Generate random or default key vector
        if (sPos == -1) {
            generate_random_key_vector(key_vector, &key_size);
        } else {
            init_default_key_vector(key_vector);
            key_size = DEFAULT_KEY_VECTOR_SIZE;
        }
    }

    // Set or validate sPos
    if (sPos == -1) {
        sPos = rand() % key_size; // Random sPos if not provided
    } else if (sPos < 0 || sPos >= key_size) {
        printf("Error: Starting position must be between 0 and %d\n", key_size - 1);
        return 1;
    }

    // Prepare output buffer (conservatively sized for worst case)
    char *output = malloc(MAX_MESSAGE * (size_t)key_size + 1);
    if (!output) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    // Encrypt
    encrypt(message, sPos, key_vector, key_size, output);

    // Print results
    printf("\nInput Message: %s\n", message);
    printf("Starting Position: %d (%c)\n", sPos, key_vector[sPos]);
    printf("Key Vector: %s\n", key_vector);
    printf("Encrypted Output: %s\n\n", output);

    // Clean up
    free(output);
    return 0;
}
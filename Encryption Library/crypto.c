#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

char* sanitize_text(const char *text) {
    int len = strlen(text);
    char *result = malloc(len + 1);
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            result[j++] = toupper(text[i]);
        }
    }
    result[j] = '\0';
    return result;
}

// Timing utilities
double get_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}

// RSA removed - will be implemented separately later

// ============================================================================
// CAESAR CIPHER
// ============================================================================

char* caesar_encrypt(const char *plaintext, int shift) {
    int len = strlen(plaintext);
    char *ciphertext = malloc(len + 1);
    
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            ciphertext[i] = ((plaintext[i] - base + shift) % 26) + base;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
    return ciphertext;
}

char* caesar_decrypt(const char *ciphertext, int shift) {
    return caesar_encrypt(ciphertext, 26 - shift);
}

// ============================================================================
// VIGENÈRE CIPHER
// ============================================================================

char* vigenere_encrypt(const char *plaintext, const char *key) {
    int text_len = strlen(plaintext);
    int key_len = strlen(key);
    char *ciphertext = malloc(text_len + 1);
    int key_index = 0;
    
    for (int i = 0; i < text_len; i++) {
        if (isalpha(plaintext[i])) {
            char base = isupper(plaintext[i]) ? 'A' : 'a';
            int shift = toupper(key[key_index % key_len]) - 'A';
            ciphertext[i] = ((plaintext[i] - base + shift) % 26) + base;
            key_index++;
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[text_len] = '\0';
    return ciphertext;
}

char* vigenere_decrypt(const char *ciphertext, const char *key) {
    int text_len = strlen(ciphertext);
    int key_len = strlen(key);
    char *plaintext = malloc(text_len + 1);
    int key_index = 0;
    
    for (int i = 0; i < text_len; i++) {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            int shift = toupper(key[key_index % key_len]) - 'A';
            plaintext[i] = ((ciphertext[i] - base - shift + 26) % 26) + base;
            key_index++;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[text_len] = '\0';
    return plaintext;
}

// ============================================================================
// PLAYFAIR CIPHER
// ============================================================================

void generate_playfair_matrix(const char *key, char matrix[5][5]) {
    int used[26] = {0};
    used['J' - 'A'] = 1;
    int pos = 0;
    
    for (int i = 0; key[i]; i++) {
        char c = toupper(key[i]);
        if (c == 'J') c = 'I';
        if (isalpha(c) && !used[c - 'A']) {
            matrix[pos / 5][pos % 5] = c;
            used[c - 'A'] = 1;
            pos++;
        }
    }
    
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        if (!used[c - 'A']) {
            matrix[pos / 5][pos % 5] = c;
            pos++;
        }
    }
}

void find_position(char matrix[5][5], char c, int *row, int *col) {
    if (c == 'J') c = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
    // Default values if not found (should never happen with valid input)
    *row = 0;
    *col = 0;
}

char* playfair_encrypt(const char *plaintext, const char *key) {
    char matrix[5][5];
    generate_playfair_matrix(key, matrix);
    
    char *clean = sanitize_text(plaintext);
    int len = strlen(clean);
    char *prepared = malloc(len * 2 + 2);
    int prep_pos = 0;
    
    for (int i = 0; i < len; i++) {
        prepared[prep_pos++] = clean[i];
        if (i < len - 1 && clean[i] == clean[i + 1]) {
            prepared[prep_pos++] = 'X';
        }
    }
    if (prep_pos % 2 == 1) {
        prepared[prep_pos++] = 'X';
    }
    prepared[prep_pos] = '\0';
    
    char *ciphertext = malloc(prep_pos + 1);
    
    for (int i = 0; i < prep_pos; i += 2) {
        int r1, c1, r2, c2;
        find_position(matrix, prepared[i], &r1, &c1);
        find_position(matrix, prepared[i + 1], &r2, &c2);
        
        if (r1 == r2) {
            ciphertext[i] = matrix[r1][(c1 + 1) % 5];
            ciphertext[i + 1] = matrix[r2][(c2 + 1) % 5];
        } else if (c1 == c2) {
            ciphertext[i] = matrix[(r1 + 1) % 5][c1];
            ciphertext[i + 1] = matrix[(r2 + 1) % 5][c2];
        } else {
            ciphertext[i] = matrix[r1][c2];
            ciphertext[i + 1] = matrix[r2][c1];
        }
    }
    ciphertext[prep_pos] = '\0';
    
    free(clean);
    free(prepared);
    return ciphertext;
}

// Cryptanalysis features removed for simplicity

// ============================================================================
// BENCHMARKING
// ============================================================================

void benchmark() {
    printf("\n========================================================\n");
    printf("                   CIPHER BENCHMARKS                    \n");
    printf("========================================================\n\n");
    
    const char *test_text = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG";
    int iterations = 10000;
    
    printf("Test text: %s\n", test_text);
    printf("Iterations: %d\n\n", iterations);
    
    // Caesar
    double start = get_time();
    for (int i = 0; i < iterations; i++) {
        char *enc = caesar_encrypt(test_text, 3);
        free(enc);
    }
    double caesar_time = get_time() - start;
    
    // Vigenere
    start = get_time();
    for (int i = 0; i < iterations; i++) {
        char *enc = vigenere_encrypt(test_text, "KEY");
        free(enc);
    }
    double vigenere_time = get_time() - start;
    
    // Playfair
    start = get_time();
    for (int i = 0; i < iterations; i++) {
        char *enc = playfair_encrypt(test_text, "KEYWORD");
        free(enc);
    }
    double playfair_time = get_time() - start;
    
    printf("---------------------------------------------------------\n");
    printf(" Cipher      | Time (s)  | Ops/sec  | Relative       \n");
    printf("---------------------------------------------------------\n");
    printf(" Caesar      | %8.4f  | %8.0f | 1.00x          \n", 
           caesar_time, iterations/caesar_time);
    printf(" Vigenere    | %8.4f  | %8.0f | %.2fx          \n", 
           vigenere_time, iterations/vigenere_time, vigenere_time/caesar_time);
    printf(" Playfair    | %8.4f  | %8.0f | %.2fx          \n", 
           playfair_time, iterations/playfair_time, playfair_time/caesar_time);
    printf("---------------------------------------------------------\n");
}

// ============================================================================
// CLI INTERFACE
// ============================================================================

void print_banner() {
    printf("\n========================================================\n");
    printf("     CRYPTOGRAPHY LIBRARY & CRYPTANALYSIS TOOLKIT       \n");
    printf("                    v1.0 - 2025                         \n");
    printf("========================================================\n");
}

void print_menu() {
    printf("\n---------------------------------------------------------\n");
    printf(" MAIN MENU                                               \n");
    printf("---------------------------------------------------------\n");
    printf(" 1. Caesar Cipher                                        \n");
    printf(" 2. Vigenere Cipher                                      \n");
    printf(" 3. Playfair Cipher                                      \n");
    printf(" 4. Benchmark All Ciphers                                \n");
    printf(" 5. Exit                                                 \n");
    printf("---------------------------------------------------------\n");
    printf("\nChoice: ");
}

void caesar_menu() {
    printf("\n=== Caesar Cipher ===\n");
    printf("1. Encrypt\n2. Decrypt\n\nChoice: ");
    int choice;
    if (scanf("%d", &choice) != 1) return;
    getchar();
    
    char text[1000];
    int shift;
    
    printf("Enter text: ");
    if (fgets(text, sizeof(text), stdin) == NULL) return;
    text[strcspn(text, "\n")] = 0;
    
    printf("Enter shift (0-25): ");
    if (scanf("%d", &shift) != 1) return;
    
    double start = get_time();
    char *output;
    if (choice == 1) {
        output = caesar_encrypt(text, shift);
    } else {
        output = caesar_decrypt(text, shift);
    }
    double elapsed = get_time() - start;
    
    printf("\nResult: %s\n", output);
    printf("Time: %.6f seconds\n", elapsed);
    free(output);
}

void vigenere_menu() {
    printf("\n=== Vigenere Cipher ===\n");
    printf("1. Encrypt\n2. Decrypt\n\nChoice: ");
    int choice;
    if (scanf("%d", &choice) != 1) return;
    getchar();
    
    char text[1000], key[100];
    
    printf("Enter text: ");
    if (fgets(text, sizeof(text), stdin) == NULL) return;
    text[strcspn(text, "\n")] = 0;
    
    printf("Enter key: ");
    if (fgets(key, sizeof(key), stdin) == NULL) return;
    key[strcspn(key, "\n")] = 0;
    
    double start = get_time();
    char *output;
    if (choice == 1) {
        output = vigenere_encrypt(text, key);
    } else {
        output = vigenere_decrypt(text, key);
    }
    double elapsed = get_time() - start;
    
    printf("\nResult: %s\n", output);
    printf("Time: %.6f seconds\n", elapsed);
    
    free(output);
}

void playfair_menu() {
    printf("\n=== Playfair Cipher ===\n");
    char text[1000], key[100];
    
    printf("Enter text: ");
    getchar();
    if (fgets(text, sizeof(text), stdin) == NULL) return;
    text[strcspn(text, "\n")] = 0;
    
    printf("Enter key: ");
    if (fgets(key, sizeof(key), stdin) == NULL) return;
    key[strcspn(key, "\n")] = 0;
    
    double start = get_time();
    char *output = playfair_encrypt(text, key);
    double elapsed = get_time() - start;
    
    printf("\nResult: %s\n", output);
    printf("Time: %.6f seconds\n", elapsed);
    free(output);
}

// RSA menu removed

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    if (argc > 1) {
        // Command line mode
        if (strcmp(argv[1], "benchmark") == 0) {
            benchmark();
        } else if (strcmp(argv[1], "help") == 0) {
            printf("Usage: %s [command]\n", argv[0]);
            printf("Commands:\n");
            printf("  benchmark - Run cipher benchmarks\n");
            printf("  help      - Show this message\n");
            printf("  (no args) - Interactive mode\n");
        }
        return 0;
    }
    
    // Interactive mode
    print_banner();
    
    while (1) {
        print_menu();
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\nInvalid input!\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                caesar_menu();
                break;
            case 2:
                vigenere_menu();
                break;
            case 3:
                playfair_menu();
                break;
            case 4:
                benchmark();
                break;
            case 5:
                printf("\nExiting. Stay secure!\n");
                return 0;
            default:
                printf("\nInvalid choice!\n");
        }
    }
    
    return 0;
}
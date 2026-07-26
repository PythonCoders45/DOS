#define MAX_TOKENS 16

/**
 * @brief Bare-metal custom tokenizer (Replaces strtok)
 */
int separate_command(char* input_string, char* output_tokens[MAX_TOKENS]) {
    int token_count = 0;
    int in_token = 0;

    for (size_t i = 0; input_string[i] != '\0'; i++) {
        char c = input_string[i];

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            input_string[i] = '\0'; // Replace whitespace with null-terminator
            in_token = 0;
        } else if (!in_token) {
            if (token_count < MAX_TOKENS) {
                output_tokens[token_count] = &input_string[i];
                token_count++;
                in_token = 1;
            }
        }
    }
    return token_count;
}

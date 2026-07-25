#define MAX_TOKENS 16
int separate_command(char* input_string, char* output_tokens[MAX_TOKENS]) {
    int token_count = 0;
    char* token = strtok(input_string, " \t\r\n");
    while (token != NULL && token_count < MAX_TOKENS) {
        output_tokens[token_count] = token;
        token_count++;
        token = strtok(NULL, " \t\r\n");
    }
    return token_count; 
}

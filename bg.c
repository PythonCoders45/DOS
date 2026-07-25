#define BG_BLACK             "\033[40m"
else if (strcmp(tokens[0], "clear") == 0) {
    printf(BG_BLACK "\033[H\033[J"); 
}

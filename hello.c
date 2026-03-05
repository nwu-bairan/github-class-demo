#include <stdio.h>

int main() {
    char input_str[100];
    
    printf("请输入内容: ");
   
    scanf("%s", input_str); 

    printf("你输入的内容是: %s\n", input_str);
    
    return 0;
}
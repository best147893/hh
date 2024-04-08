#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// 定义 Token 类型枚举
typedef enum {
    ID,     // 标识符
    NUM,    // 数字
    PLUS,   // 加号
    MINUS,  // 减号
    MULTI,  // 乘号
    DIV,    // 除号
    LPAREN, // 左括号
    RPAREN, // 右括号
    END     // 结束标志
} Token;

Token token; // 全局变量，存放当前的词法单元
char lexeme[100]; // 全局变量，存放当前词法单元对应的词素（字符串）
int number; // 全局变量，存放数字的值

void factor();
void term();
void expr();
// 词法分析函数，获取下一个词法单元
Token getToken() {
    char ch;

    // 跳过空格和制表符
    while ((ch = getchar()) == ' ' || ch == '\t');

    // 根据读入的字符判断词法单元类型
    switch (ch) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MULTI;
        case '/': return DIV;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '\n':return END; // 当遇到换行符时，表示结束
        default:
            if (isdigit(ch)) { // 如果是数字，继续读取整个数字
                ungetc(ch, stdin);
                scanf("%d", &number);
                return NUM;
            } else if (isalpha(ch)) { // 如果是字母，继续读取整个标识符
                int i = 0;
                lexeme[i++] = ch;
                while ((ch = getchar()) && (isalnum(ch) || ch == '_')) {
                    lexeme[i++] = ch;
                }
                lexeme[i] = '\0';
                ungetc(ch, stdin);
                return ID;
            } else {
                fprintf(stderr, "Error: Invalid character encountered!\n");
                exit(1);
            }
    }
}

// 匹配函数，用于验证当前的词法单元是否与预期相符
void match(Token expected) {
    if (token == expected) {
        token = getToken(); // 匹配成功，获取下一个词法单元
    } else {
        fprintf(stderr, "Error: Syntax error!\n");
        exit(1);
    }
}

// term -> factor {(* | /) factor}
void term() {
    factor();
    while (token == MULTI || token == DIV) {
        Token op = token;
        match(op);
        factor();
        if (op == MULTI) {
            printf("* ");
        } else {
            printf("/ ");
        }
    }
}

// expr -> term {(+ | -) term}
void expr() {
    term();
    while (token == PLUS || token == MINUS) {
        Token op = token;
        match(op);
        term();
        if (op == PLUS) {
            printf("+ ");
        } else {
            printf("- ");
        }
    }
}

// factor -> '(' expr ')' | id | num
void factor() {
    if (token == LPAREN) {
        match(LPAREN);
        expr();
        match(RPAREN);
    } else if (token == ID) {
        printf("%s ", lexeme);
        match(ID);
    } else if (token == NUM) {
        printf("%d ", number);
        match(NUM);
    } else {
        fprintf(stderr, "Error: Invalid factor!\n");
        exit(1);
    }
}

int main() {
    printf("Please input the expression: \n");
    
    token = getToken(); // 获取第一个词法单元
    printf("Output the expression: \n");
    expr(); // 开始解析表达式

    printf("\n");

    return 0;
}


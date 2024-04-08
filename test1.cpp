#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// ���� Token ����ö��
typedef enum {
    ID,     // ��ʶ��
    NUM,    // ����
    PLUS,   // �Ӻ�
    MINUS,  // ����
    MULTI,  // �˺�
    DIV,    // ����
    LPAREN, // ������
    RPAREN, // ������
    END     // ������־
} Token;

Token token; // ȫ�ֱ�������ŵ�ǰ�Ĵʷ���Ԫ
char lexeme[100]; // ȫ�ֱ�������ŵ�ǰ�ʷ���Ԫ��Ӧ�Ĵ��أ��ַ�����
int number; // ȫ�ֱ�����������ֵ�ֵ

void factor();
void term();
void expr();
// �ʷ�������������ȡ��һ���ʷ���Ԫ
Token getToken() {
    char ch;

    // �����ո���Ʊ��
    while ((ch = getchar()) == ' ' || ch == '\t');

    // ���ݶ�����ַ��жϴʷ���Ԫ����
    switch (ch) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MULTI;
        case '/': return DIV;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '\n':return END; // ���������з�ʱ����ʾ����
        default:
            if (isdigit(ch)) { // ��������֣�������ȡ��������
                ungetc(ch, stdin);
                scanf("%d", &number);
                return NUM;
            } else if (isalpha(ch)) { // �������ĸ��������ȡ������ʶ��
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

// ƥ�亯����������֤��ǰ�Ĵʷ���Ԫ�Ƿ���Ԥ�����
void match(Token expected) {
    if (token == expected) {
        token = getToken(); // ƥ��ɹ�����ȡ��һ���ʷ���Ԫ
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
    
    token = getToken(); // ��ȡ��һ���ʷ���Ԫ
    printf("Output the expression: \n");
    expr(); // ��ʼ�������ʽ

    printf("\n");

    return 0;
}


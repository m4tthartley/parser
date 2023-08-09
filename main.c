#include <windows.h>
#include <stdio.h>

typedef enum {
	PUSH,
	ADD,
	SUB,
	MUL,
	DIV,
	EXIT
} instruction;

int exe[1024];
int *ip = exe;
int stack[1024];
int *sp = stack;

void emit(int i) {
	*ip++ = i;
}

void emit2(int a, int b) {
	*ip++ = a;
	*ip++ = b;
}

// void emitOp() {
// 
// }
// 
// void emitInt(int num) {
// 	*ip
// }

void execute() {
	ip = exe;

	while(*ip != EXIT) {
		switch(*ip) {
		case PUSH:
			++ip;
			*sp++ = *ip;
			break;
		case ADD:
			sp[-2] += sp[-1];
			sp--;
			break;
		case SUB:
			sp[-2] -= sp[-1];
			sp--;
			break;
		case MUL:
			sp[-2] *= sp[-1];
			sp--;
			break;
		case DIV:
			sp[-2] /= sp[-1];
			sp--;
			break;
		default:
			goto error;
		}

		++ip;
	}

	return;
error:
	printf("unknown op code %i\n", *ip);
}

void codePrintOut() {
	ip = exe;
	
	while(*ip != EXIT) {
		switch(*ip) {
		case PUSH:
			++ip;
			printf("PUSH %i\n", *ip);
			break;
		case ADD:
			printf("ADD\n");
			break;
		case SUB:
			printf("SUB\n");
			break;
		case MUL:
			printf("MUL\n");
			break;
		case DIV:
			printf("DIV\n");
			break;
		default:
			goto error;
		}
		
		++ip;
	}
	
	printf("EXIT\n");
	return;
error:
	printf("UNKNOWN OP CODE BEFORE EXIT%i\n", *ip);
}

//char *testString = "13 + 2*4"; // 21
char *testString = "2*2*3 + 4*5 - 10 - 3*2"; // 16

int isNumber(char c) {
	return c>= '0' && c <= '9';
}

typedef enum {
	TOKEN_NUMBER = 256
} token_type;

typedef struct {
	enum token_type type;
	int num;
} token;

token createToken(token_type type, int num) {
	token t;
	t.type = type;
	t.num = num;
	return t;
}

char *c;
token t;
void nextToken() {
	while(*c == ' ' || *c == '\t') {
		++c;
	}
	if (*c) {
		if (isNumber(*c)) {
			int num = 0;
			while (isNumber(*c)) {
				//printf("%c \n", *input);
				num = num*10 + (*c - '0');
				++c;
			}
			//printf("num %i\n", num);
			t = createToken(TOKEN_NUMBER, num);
			return;
		}

		//printf("%c \n", *c);
		t = createToken(*c++, 0);
		return;
		//++c;
	}

	{
		token r = {0};
		//return r;
		t = r;
	}
}

void expr2() {
	nextToken();
	if (t.type == TOKEN_NUMBER) {
		//int num = t.num;
		emit2(PUSH, t.num);
		nextToken();
		//return num;
	} else {
		printf("Expected number\n");
		exit(0);
	}
}

void  expr1() {
	expr2();
	while(t.type == '*' || t.type == '/') {
		char op = t.type;
		expr2();
		//printf("%i %c %i\n", l, op, r);
		//l = op=='*' ? l*r : l/r;
		if (op=='*') {
			emit(MUL);
		} else {
			emit(DIV);
		}
	}

	//return l;
}

void expr() {
	expr1();
	
	while(t.type == '+' || t.type == '-') {
		char op = t.type;
		expr1();
		//printf("%i %c %i\n", l, op, r);
		//l = op=='+' ? l+r : l-r;
		if (op=='+') {
			emit(ADD);
		} else {
			emit(SUB);
		}
	}
	
	//return l;
}

int main() {
	char buf[64];
	c = testString;
	
	printf("question: ");

	{
		char *str = fgets(buf, 64, stdin);
		//printf("input: %s\n", buf);
		if (str) {
			c = buf;
		} else {
			printf("input too long\n");
			exit(0);
		}
	}
	
	//printf("question: %s\n", testString);
	{
		expr();
		emit(EXIT);
		//printf("answer: %i\n", a);
	}

	execute();
	printf("answer: %i\n", sp[-1]);
	codePrintOut();
	
// 	{
// 		int *p = 0;
// 		*p = 0;
// 	}

	{
// 		emit2(PUSH, 5);
// 		emit2(PUSH, 3);
// 		emit(ADD);
// 		emit(EXIT);
// 		execute();
	}

	getchar();
}
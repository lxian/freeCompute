/*************************************************************************
	> File Name: lex.c
	> Author: ellochen
	> Mail: god_mode@yeah.net
	> Created Time: Mon Apr 29 21:57:57 2013
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lex.h"
#include "var.h"
typedef struct Token (*Matcher)(void);
extern char* CURSOR;
#define END_OF_FILE  255
static Matcher matchers[END_OF_FILE+1];

static struct Token matchInt(void)
{
	int i = 0;
	char *start = CURSOR;
	CURSOR += 1;
	while (IsDigit(*CURSOR)) {
		CURSOR += 1;
	}
	i = atoi(start);
	struct Token token;
	token.var.i = i;
	token.type = TOKEN_INTCONST;
	return token;
}

static struct Token matchDouble()
{
	double d = 0;
	char *start = CURSOR;
	CURSOR += 1;
	while (IsDigit(*CURSOR)) {
		CURSOR += 1;
	}
	if ('.' == *CURSOR) {
		CURSOR += 1;
		while (IsDigit(*CURSOR)) {
			CURSOR += 1;
		}	
	}
	d = strtod(start,0);
	struct Token token;
	token.var.d = d;
	token.type = TOKEN_DOUBLECONST;
	return token;
}

static struct Token matchPlus(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_PLUS;
	return token;
}

static struct Token matchMinus(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_MINUS;
	return token;
}

static struct Token matchStar(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_STAR;
	return token;
}

static struct Token matchSlash(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_SLASH;
	return token;
}

static struct Token matchChar(void)
{
	return varEval();	
	/*
	struct Token token;
	token.type = TOKEN_DOUBLECONST;
	token.var.d = 1.0;
	return token;
	*/
	//printf("无法识别的字符:%c\n",*CURSOR);
	//exit(0);
}

static struct Token matchLineEnd(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_LINEEND;
	return token;
}

static struct Token matchLparen(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_LPAREN;
	return token;
}

static struct Token matchRparen(void)
{
	CURSOR += 1;
	struct Token token;
	token.type = TOKEN_RPAREN;
	return token;
}

void InitLexer(void)
{
	int i;
	for (i = 0; i < END_OF_FILE + 1; i++)
	{
		if (IsDigit(i))
		{
			matchers[i] = matchDouble;
		}
		else
		{
			matchers[i] = matchChar;
		}
	}
	matchers['\0'] = matchLineEnd;
	matchers['+'] = matchPlus;
	matchers['-'] = matchMinus;
	matchers['*'] = matchStar;
	matchers['/'] = matchSlash;
	matchers['('] = matchLparen;
	matchers[')'] = matchRparen;
}

static void SkipWhiteSpace(void)
{
	while (*CURSOR == '\t' || *CURSOR == '\v' || *CURSOR == '\f' || *CURSOR == ' ' ||
	       *CURSOR == '\r' || *CURSOR == '\n') {
			   CURSOR += 1;
	}
}

struct Token getNextToken()
{
	SkipWhiteSpace();
	return (matchers[*CURSOR])();	
}

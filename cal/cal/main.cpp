//
//  main.cpp
//  cal
//
//  Created by taowei on 13-10-26.
//  Copyright (c) 2013年 taowei. All rights reserved.
//

#include <iostream>
#include "token.h"
using namespace std;

#define LINE_BUFF_SIZE 1024

static char *input_line;
static int input_line_pos;

static Token lookahead_token;
static int lookaheadToken;

typedef enum {
	init_Status,
	int_Status,
	dot_Status,
	frac_Status
}LexerStatus;

void get_lex_token(Token *token) {
	int output_pos = 0;
	LexerStatus status = init_Status;
	char current_char;

	token->tKind = Bad_Token;
	while (input_line[input_line_pos] != '\0') {
		current_char = input_line[input_line_pos];
		if ((status == int_Status || status == frac_Status)
			&& !(current_char <= '9' && current_char >= '0')
			&& current_char != '.') {
			token->tKind = Number_Token;
			sscanf(token->str, "%lf", &token->value);
			return;
		}
		if (current_char == ' ') {
			if (current_char == '\n') {
				token->tKind = END_OF_LINE_TOKEN;
				return;
			}
			input_line++;
			continue;
		}

		if (output_pos >= MAX_TOKEN_SIZE - 1) {
			cout << "token too long!\n";
			exit(1);
		}
		token->str[output_pos] = input_line[input_line_pos];
		input_line_pos++;
		output_pos++;
		token->str[output_pos] = '\0';

		if (current_char == '+') {
			token->tKind = Add_Token;
			return;
		} else if (current_char == '-') {
			token->tKind = Sub_Token;
			return;
		} else if (current_char == '*') {
			token->tKind = Mul_Token;
			return;
		} else if (current_char == '/') {
			token->tKind = Div_Token;
			return;
		} else if (current_char == '(') {
			token->tKind = LP_Token;
		} else if (current_char == ')') {
			token->tKind = RP_Token;
		} else if (current_char >= '0' && current_char <= '9') {
			if (status == init_Status) {
				status = int_Status;
			} else if (status == dot_Status) {
				status = frac_Status;
			}
		} else if (current_char == '.') {
			if (status == int_Status) {
				status = dot_Status;
			} else {
				cout << "syntax error!\n";
			}
		} else {
			cout << "bad char!\n";
		}
	}
}

void unget_token(Token *token) {
	lookahead_token = *token;
	lookaheadToken = 1;
}

void get_token(Token *token) {
	if (lookaheadToken) {
		*token = lookahead_token;
		lookaheadToken = 0;
	} else {
		get_lex_token(token);
	}
}

double parse_expression();

double parse_primary_expression() {
	Token token;
	double value = 0;
	int mflag = 0;
	get_token(&token);
	if (token.tKind == Sub_Token) {
		mflag = 1;
	} else {
		unget_token(&token);
	}
	get_token(&token);
	if (token.tKind == Number_Token) {
		return token.value;
	} else if (token.tKind == LP_Token) {
		value = parse_expression();
		get_token(&token);
		if (token.tKind != RP_Token) {
			cout << "syntax erros! missing ')'\n";
			exit(1);
		}
	} else {
		unget_token(&token);
	}
	if (mflag) {
		value = -value;
	}
	return value;
}

double parse_term() {
	double v1,v2;
	Token token;

	v1 = parse_primary_expression();
	for (; ;) {
		get_token(&token);
		if (token.tKind != Mul_Token && token.tKind != Div_Token) {
			unget_token(&token);
			break;
		}
		v2 = parse_primary_expression();
		if (token.tKind == Mul_Token) {
			v1 = v1 * v2;
		} else if (token.tKind == Div_Token) {
			v1 = v1 / v2;
		}
	}
	return v1;
}

double parse_expression() {
	double v1,v2;
	Token token;

	v1 = parse_term();
	for (; ;) {
		get_token(&token);
		if (token.tKind != Add_Token && token.tKind != Sub_Token) {
			unget_token(&token);
			break;
		}
		v2 = parse_term();
		if (token.tKind == Add_Token) {
			v1 = v1 + v2;
		} else if (token.tKind == Sub_Token) {
			v1 = v1 - v2;
		} else {
			unget_token(&token);
		}
	}
	return v1;
}

double parse_line() {
	double value;
	lookaheadToken = 0;
	value = parse_expression();
	return value;
}

void set_line(char *line) {
	input_line = line;
	input_line_pos = 0;
}

int main(int argc, const char * argv[])
{

	char line[LINE_BUFF_SIZE];
	double value;

	while (fgets(line, LINE_BUFF_SIZE, stdin) != NULL) {
		set_line(line);
		value = parse_line();
		cout << value << '\n';
	}
    return 0;
}


//
//  token.h
//  cal
//
//  Created by taowei on 13-10-26.
//  Copyright (c) 2013年 taowei. All rights reserved.
//

#ifndef cal_token_h
#define cal_token_h

typedef enum {
	Bad_Token,
	Add_Token,
	Sub_Token,
	Mul_Token,
	Div_Token,
	Number_Token,
	LP_Token,
	RP_Token,
	END_OF_LINE_TOKEN
}TokenKind;

#define MAX_TOKEN_SIZE 100

typedef struct {
	TokenKind tKind;
	double value;
	char str[MAX_TOKEN_SIZE];
}Token;

void set_line(char *line);
void get_token(Token *token);

#endif

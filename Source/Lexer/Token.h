#pragma once

#include <string>
#ifdef TOKDBG
	#include <iostream>
	#include <unordered_map>
#endif

#define FOR_TOKENTYPES(TOKENIZE) \
TOKENIZE(IMPORT)	TOKENIZE(DEFINE)	TOKENIZE(IDENTIFIER)	TOKENIZE(NEWLINE) \
TOKENIZE(PLUS)		TOKENIZE(MINUS)		TOKENIZE(ASSIGN)		TOKENIZE(COMMA) \
TOKENIZE(STR_LITERAL)	TOKENIZE(INT_LITERAL)	TOKENIZE(BOOL_LITERAL)	TOKENIZE(LBRA) \
TOKENIZE(RBRA)		TOKENIZE(LPAR)		TOKENIZE(RPAR)			TOKENIZE(LSBR) \
TOKENIZE(RSBR)		TOKENIZE(END)		TOKENIZE(INVALID)

#define TOKEN_ENUMCLASS(TOKEN) TOKEN,
#define TOKEN_STRING(TOKEN) #TOKEN
#define TOKEN_MAP(TOKEN)  {TokenType::TOKEN, #TOKEN },
#define PRINT_TOKEN(TOKEN, CHAR) ( \
			std::cout << "TOKEN:\t\t" << TOKEN << " " << CHAR << std::endl \
		)

template <typename T, typename... U>
extern std::string joinWhSpace(T&& str, U&& ...nstr);

class Token
{
public:

	enum class TokenType
	{		
		/******
			IMPORT,			// #import module
			DEFINE,			// #define target Pytorch
			
			IDENTIFIER, 	// identifier (see grammar)

			NEWLINE,		// '\n'
			PLUS,			// '+'
			MINUS,			// '-'
			ASSIGN,			// '='
			COMMA, 			// ','

			STR_LITERAL,	// "([a-z] | [A-Z] | [0-9] | '_')*"
			INT_LITERAL,	// ([0-9])*
			BOOL_LITERAL,	// True/False true/false 1/0

			LBRA,			// '{'
			RBRA,			// '}'
			LPAR,			// '('
			RPAR,			// ')'
			LSBR,			// '['
			RSBR,			// ']'

			END,			// EOF
			INVALID			// Unk token
		******/
		FOR_TOKENTYPES(TOKEN_ENUMCLASS)
	};

    using Position = std::pair<int, int>;

	Token() = delete;
	//maybe add template instead of overload ?
	Token(TokenType t, Position p, std::string d = "") : token(t), pos(p), data(d) {
	#ifdef TOKDBG
		print();
	#endif
	}
	Token(TokenType t, Position p, char d) : token(t), pos(p), data((const char*)&d) {
	#ifdef TOKDBG
		print();
	#endif
	}
	~Token() = default;

	const TokenType &getToken() const
	{
		return token;
	}

	const std::string getStrPos() const
	{
		return joinWhSpace("l:" + std::to_string(pos.first), "c:" + std::to_string(pos.second) + ":");
	}

	const std::string getData() const
	{
		return data;
	}

#ifdef TOKDBG
	void print() const
	{
		PRINT_TOKEN(tokenMap[token], data);
		return;
	}
#endif
	bool operator== (const Token &r) const
	{
		return ((token == r.token) && (data == r.data) ? true : false);
	}

	bool operator== (const TokenType &rt) const
	{
		return ( token == rt ? true : false);
	}

private:
	TokenType token;
	Position pos;
	std::string data;

#ifdef TOKDBG
public:
	static std::unordered_map<TokenType, std::string> tokenMap;
#endif

};

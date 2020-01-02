#pragma once

#include <string>

class LengthOf : public ASTNode
{
public:
	LengthOf() = delete;
	LengthOf(std::string l) : layer(l) {}
	~LengthOf() = default;

private:
	std::string layer;
};

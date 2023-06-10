// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#include "core/argparse/context.h"

namespace core::argp
{

Context::Context(const std::vector<std::string>& args)
    : m_index(0)
{
    for (const auto& arg : args)
	m_tokens.push_back(arg);
}

bool Context::end() const
{
    return m_index >= m_tokens.size();
}

std::string_view Context::front() const
{
    return m_tokens[m_index];
}

void Context::pop()
{
    if (m_index < m_tokens.size())
	++m_index;
}

const std::vector<std::string>& Context::tokens() const
{
    return m_tokens;
}

std::string Context::canonical_line() const
{
    std::string line;
    for (const auto& token : m_tokens)
    {
	line += " ";
	line += token;
    }
    return line;
}

std::string Context::canonical_marker() const
{
    std::string line;
    for (const auto& token : m_tokens)
    {
	line += " ";
	if (token == front())
	    line += "^";
	line += std::string(token.size(), ' ');
    }
    return line;
}


}; // core::argp

// Copyright (C) 2019, 2022, 2023 by Mark Melton
//

#pragma once
#include "base.h"

namespace core::argp
{

class Context
{
public:
    Context(const std::vector<std::string>& args);

    bool end() const;
    std::string_view front() const;
    void pop();

    const std::vector<std::string>& tokens() const;
    std::string canonical_line() const;
    std::string canonical_marker() const;
    
private:
    size_t m_index;
    std::vector<std::string> m_tokens;
};

}; // core::argp

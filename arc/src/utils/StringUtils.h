#pragma once

#include <string>
#include <vector>

namespace arc
{
    namespace string
    {
        inline static std::vector<std::string> split (const std::string& s, const std::string& delimiter) {
            size_t pos_start = 0, pos_end, delim_len = delimiter.length();
            std::string token;
            std::vector<std::string> res;

            while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
                token = s.substr (pos_start, pos_end - pos_start);
                if (!token.empty() && token[token.size() - 1] == '\r')
                    token.erase(token.size() - 1);

                pos_start = pos_end + delim_len;

                if(token.size() > 0)
                    res.push_back (token);
            }

            std::string remain = s.substr(pos_start);

            if (!remain.empty() && remain[remain.size() - 1] == '\r')
                remain.erase(remain.size() - 1);

            if(remain.size() > 0)
                res.push_back (remain);
            return res;
        }
    }
}
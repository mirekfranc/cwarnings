#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>

namespace
{
    class Warnings
    {
        std::unordered_map<std::string, int> m_warnings {};
        int m_limit {};

    public:
        explicit Warnings (int _limit = 0) : m_limit(_limit) {}

        void insert (std::string _warning)
        {
            if (!_warning.empty())
            {
                const auto it = m_warnings.find (_warning);
                if (it == m_warnings.end ())
                    m_warnings.insert (std::make_pair (std::move (_warning), 1));
                else
                    ++it->second;
            }
        }

        void print ()
        {
            struct warning_extended
            {
                std::string text;
                int count;
            };
            std::vector<warning_extended> to_print;
            to_print.reserve (m_warnings.size ());

            for (const auto& w: m_warnings)
            {
                warning_extended my_warning = { w.first, w.second };
                to_print.emplace_back (std::move (my_warning));
            }
            std::sort(to_print.begin (),
                      to_print.end (),
                      [] (const auto& _a, const auto& _b) { return _a.count > _b.count; });

            int limit = 1;
            for (const auto& w: to_print)
            {
                std::cout << ">=== " << w.count << " warning";
                if (w.count > 1)
                    std::cout << 's';
                std::cout << " ===<\n" << w.text << std::endl;

                if (m_limit > 0 && ++limit > m_limit)
                    break;
            }
        }
    };

    constexpr int how_many_lines_of_context = 2;
}

int main (int argc, char *argv[])
{
    const int limit = (argc > 1 ? std::atoi (argv[1]) : 0);
    Warnings warnings (limit);

    std::string string_buffer;
    int context_line (0);

    std::string line;
    while (std::getline (std::cin, line))
    {
        if (!context_line && !string_buffer.empty())
        {
            warnings.insert(std::move(string_buffer));
            string_buffer = {};
        }

        bool skip_this_line (true);

        auto r = line.crbegin (), b = line.crend ();
        if (context_line)
        {
            skip_this_line = false;
            --context_line;
        }
        else if (r != b && *r == ']')
            for (++r; r != b; ++r)
            {
                if (*r == '[')
                {
                    context_line = how_many_lines_of_context;
                    skip_this_line = false;
                }
                else if (*r != '-' && (*r < 'A' || *r > 'Z') && (*r < 'a' || *r > 'z'))
                    break;
            }

        if (skip_this_line)
            continue;

        string_buffer += '\n';
        string_buffer += line;
    }

    warnings.insert (std::move (string_buffer));

    warnings.print ();
}

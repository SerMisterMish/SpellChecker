#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

std::set<std::string> find_nearest_words(const std::set<std::string> &dict, const std::string &str)
{
    std::set<std::string> result;
    size_t len = str.size();
    for (size_t i{}; i < len; ++i)
    {
        std::string str_mod(str);
        char former_char = str_mod[i];
        for (char c = 'a'; c <= 'z'; ++c)
        {
            str_mod[i] = c;
            if (dict.count(str_mod))
                result.insert(str_mod);
        }
        str_mod[i] = former_char;

        str_mod.insert(i, 1, ' ');
        for (char c = 'a'; c <= 'z'; ++c)
        {
            str_mod[i] = c;
            if (dict.count(str_mod))
                result.insert(str_mod);
        }
        str_mod.erase(i, 1);

        str_mod.erase(i, 1);
        if (dict.count(str_mod))
            result.insert(str_mod);
    }

    return result;
}

int main()
{
    std::ifstream input("../input.txt");
    std::ifstream dict_file("../dictionary.txt");
    std::ofstream output("../output.txt");
    std::ofstream dict_out("../dictionary_new.txt");

    std::set<std::string> dict;
    std::string buff;
    dict_file >> buff;
    while (dict_file && !buff.empty())
    {
        buff[0] = tolower(buff[0]);
        dict.insert(buff);
        dict_file >> buff;
    }

    input >> buff;
    std::string lower_case_str;
    while (input && !buff.empty())
    {
        if (buff == "-")
        {
            output << "- ";
            input >> buff;
            continue;
        }

        lower_case_str = buff;
        char punctuation_mark = 0;
        switch (lower_case_str.back())
        {
        case ',':
        case '.':
        case ';':
        case ':':
        case '?':
        case '!':
            punctuation_mark = lower_case_str.back();
            lower_case_str.pop_back();
        }
        std::transform(lower_case_str.begin(), lower_case_str.end(), lower_case_str.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (dict.count(lower_case_str) == 0)
        {
            char input_char{};
            bool correct_input{};

            std::cout << "\nUnknown word \"" << lower_case_str << "\" is encountered.\n"
                      << "What would you like to do with it?\n"
                      << "1: Add the word to the dictionary\n"
                      << "2: Ignore the word\n"
                      << "3: Replace the word with a word from the dictionary\n"
                      << "Input a digit from 1 to 3: ";
            while (!correct_input)
            {
                std::cin >> input_char;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (input_char >= '1' && input_char <= '3')
                    correct_input = true;
                else
                {
                    std::cerr << "Incorrect input.\nInput a digit from 1 to 3: ";
                }
            }

            bool find_replacements{};
            switch (input_char)
            {
            case '1':
                dict.insert(lower_case_str);
                break;
            case '2':
                break;
            case '3':
                find_replacements = true;
                break;
            default:
                break;
            }

            if (find_replacements)
            {
                std::set<std::string> replacements(find_nearest_words(dict, lower_case_str));
                if (replacements.size() == 0)
                {
                    std::cout << "\nNo replacements found in the dictionary.\n"
                              << "What would you like to do?\n"
                              << "1: Add the word to the dictionary\n"
                              << "2: Ignore the word\n"
                              << "Input a digit from 1 to 2: ";
                    input_char = 0;
                    correct_input = false;
                    while (!correct_input)
                    {
                        std::cin >> input_char;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (input_char == '1' || input_char == '2')
                        {
                            correct_input = true;
                        }
                        else
                        {
                            std::cerr << "Incorrect input.\nInput a digit from 1 to 2: ";
                        }
                    }

                    switch (input_char)
                    {
                    case '1':
                        dict.insert(lower_case_str);
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    std::cout << "\nChoose a replacement word from the list below:\n";
                    size_t replacements_size = replacements.size(), tmp_ind{1};
                    for (auto it = replacements.begin(); it != replacements.end(); ++it, ++tmp_ind)
                    {
                        std::cout << tmp_ind << ": " << *it << '\n';
                    }
                    std::cout << "Input a number from 1 to " << replacements_size << ": ";
                    size_t input_ind = 0;
                    correct_input = false;
                    while (!correct_input)
                    {
                        std::cin >> input_ind;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (input_ind >= 1 && input_ind <= replacements_size)
                        {
                            correct_input = true;
                        }
                        else
                        {
                            std::cerr << "Incorrect input.\nInput a number from 1 to " << replacements_size << ": ";
                        }
                    }
                    auto it = replacements.begin();
                    for (size_t i{ 1 }; i < input_ind; ++i)
                    {
                        ++it;
                    }
                    buff = *it + punctuation_mark;
                }
            }
        }

        output << buff << ' ';
        input >> buff;
    }

    std::string word_upper;
    for (auto& word : dict) {
        word_upper = word;
        word_upper[0] = toupper(word_upper[0]);
        dict_out << word_upper << '\n';
    }
}

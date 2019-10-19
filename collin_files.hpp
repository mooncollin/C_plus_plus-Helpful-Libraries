#ifndef COLLIN_FILES
#define COLLIN_FILES

namespace collin
{
	template<typename T>
    class Lines : public std::istream_iterator<T>
    {
        using parent = std::istream_iterator<T>;

        public:
            Lines()
            {
            }

            Lines(std::ifstream& input) : input(&input)
            {
                this->operator++();
            }

            Lines<T> end() const
            {
                return Lines<T>();
            }

            typename parent::reference operator*() const
            {
                return current;
            }

            typename parent::pointer operator->() const
            {
                return &current;
            }

            std::istream_iterator<T>& operator++()
            {
                next_input();
                return *this;
            }

            std::istream_iterator<T> operator++(int)
            {
                auto l = Lines(*this);
                next_input();

                return l;
            }

            friend bool operator==(const Lines<T>& lhs, const Lines<T>& rhs)
            {
                return lhs.input == rhs.input;
            }

            friend bool operator!=(const Lines<T>& lhs, const Lines<T>& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            std::ifstream* input = nullptr;
            T current{};

            std::string get_input() const
            {
                std::string str;
                std::getline(*input, str);
                return str;
            }

            T parse_input(const std::string& str)
            {
                T result;
                std::stringstream ss;
                ss << str;
                ss >> result;
                return result;
            }

            void next_input()
            {
                if(input != NULL && input->eof())
                {
                    input = NULL;
                }
                if(input != NULL)
                {
                    current = parse_input(get_input());
                }
            }
    };

    std::string read_all(const std::string& name)
    {
        std::ifstream file(name);

        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
}

#endif
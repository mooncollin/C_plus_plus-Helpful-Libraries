export module cmoon.json.json_outputter;

import <iostream>;
import <functional>;
import <variant>;

import cmoon.json.json_value;

namespace cmoon::json
{
	export
	template<class JsonType = json_value<>>
	class json_outputter
	{
		public:
			using json_value_type = JsonType;
			using json_boolean_type = typename json_value_type::boolean_type;
			using json_string_type = typename json_value_type::string_type;
			using json_int_type = typename json_value_type::int_type;
			using json_float_type = typename json_value_type::float_type;
			using json_null_type = typename json_value_type::null_type;
			using json_list_type = typename json_value_type::list_type;
			using json_object_type = typename json_value_type::object_type;

			using json_char_type = typename json_string_type::value_type;
			using json_variant_type = typename json_value_type::type;

			json_outputter(std::basic_ostream<json_char_type>& os)
				: os{os} {}

			inline void operator()(const json_int_type& value)
			{
				os.get() << value;
			}

			inline void operator()(const json_float_type& value)
			{
				os.get() << value;
			}

			inline void operator()(const json_boolean_type& value)
			{
				if (value)
				{
					os.get() << "true";
				}
				else
				{
					os.get() << "false";
				}
			}

			inline void operator()(const json_null_type& value)
			{
				os.get() << "null";
			}

			void operator()(const json_string_type& s)
			{
				os.get() << "\"";

				for (const auto ch : s)
				{
					switch (ch)
					{
						case '"':
							os.get() << "\\\"";
							break;
						case '\\':
							os.get() << "\\\\";
							break;
						case '/':
							os.get() << "\\/";
							break;
						case '\b':
							os.get() << "\\b";
							break;
						case '\f':
							os.get() << "\\f";
							break;
						case '\n':
							os.get() << "\\n";
							break;
						case '\r':
							os.get() << "\\r";
							break;
						case '\t':
							os.get() << "\\t";
							break;
						default:
							os.get() << ch;
							break;
					}
				}

				os.get() << "\"";
			}

			void operator()(const json_list_type& list)
			{
				os.get() << "[";
				for(auto it = std::cbegin(list); it != std::cend(list); ++it)
				{
					operator()(*it);
					if ((it + 1) != std::cend(list))
					{
						os.get() << ", ";
					}
				}
				os.get() << "]";
			}

			void operator()(const json_object_type& obj)
			{
				os.get() << "{";
				for(auto it = std::cbegin(obj); it != std::cend(obj); ++it)
				{
					operator()(it->first);
					os.get() << ": ";
					operator()(it->second);
					auto it2 = it;
					if (++it2 != std::cend(obj))
					{
						os.get() << ", ";
					}
				}
				os.get() << "}";
			}

			inline void operator()(const json_value_type& json)
			{
				operator()(json.value);
			}

			inline void operator()(const json_variant_type& json)
			{
				std::visit(*this, json);
			}
		private:
			std::reference_wrapper<std::basic_ostream<json_char_type>> os;
	};

	export
	template<class BooleanType, class StringType, class IntType, class FloatType, template<class, class> class ListType, template<class...> class ObjectType>
	std::ostream& operator<<(std::ostream& os, const basic_json_value<BooleanType, StringType, IntType, FloatType, ListType, ObjectType>& json)
	{
		using json_t = basic_json_value<BooleanType, StringType, IntType, FloatType, ListType, ObjectType>;
		json_outputter<json_t> outputter {os};
		outputter(json);
		return os;
	}
}
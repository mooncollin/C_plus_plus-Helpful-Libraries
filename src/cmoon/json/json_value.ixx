export module cmoon.json.json_value;

import <memory>;
import <string>;
import <functional>;
import <variant>;
import <list>;
import <map>;

namespace cmoon::json
{
	export
	struct null {};

	export
	template<class BooleanType, class StringType, class IntType, class FloatType, template<class, class> class ListType, template<class...> class ObjectType, class Allocator = std::allocator<void>>
	struct basic_json_value
	{
		using boolean_type = BooleanType;
		using string_type = StringType;
		using int_type = IntType;
		using float_type = FloatType;
		using null_type = null;
		using list_type = ListType<basic_json_value, typename std::allocator_traits<Allocator>::template rebind_alloc<basic_json_value>>;

		private:
			template<class T, class Alloc>
			struct object_picker;

			template<class Alloc>
			struct object_picker<std::map<string_type, basic_json_value, std::less<string_type>, std::allocator<std::pair<const string_type, basic_json_value>>>, Alloc>
			{
				using type = std::map<string_type, basic_json_value, std::less<string_type>, typename std::allocator_traits<Alloc>::template rebind_alloc<std::pair<const string_type, basic_json_value>>>;
			};

			template<class Alloc>
			struct object_picker<std::unordered_map<string_type, basic_json_value, std::hash<string_type>, std::equal_to<string_type>, std::allocator<std::pair<const string_type, basic_json_value>>>, Alloc>
			{
				using type = std::unordered_map<string_type, basic_json_value, std::hash<string_type>, std::equal_to<string_type>, typename std::allocator_traits<Alloc>::template rebind_alloc<std::pair<const string_type, basic_json_value>>>;
			};

			template<class T, class Alloc>
			using object_picker_t = typename object_picker<T, Alloc>::type;
		public:

		using object_type = object_picker_t<ObjectType<string_type, basic_json_value>, Allocator>;

		using type = std::variant<null_type, boolean_type, string_type, int_type, float_type, list_type, object_type>;

		constexpr operator const type& () const noexcept
		{
			return value;
		}

		constexpr operator type& () noexcept
		{
			return value;
		}

		constexpr basic_json_value(const type& v)
			: value{v} {}

		constexpr basic_json_value(type&& v) noexcept
			: value{std::forward<type>(v)} {}

		constexpr explicit basic_json_value() noexcept = default;

		constexpr basic_json_value(const basic_json_value&) = default;
		constexpr basic_json_value(basic_json_value&&) noexcept = default;

		constexpr basic_json_value& operator=(const basic_json_value&) = default;
		constexpr basic_json_value& operator=(basic_json_value&&) noexcept = default;

		template<class T>
		basic_json_value& operator=(T&& v)
		{
			value = std::forward<T>(v);
			return *this;
		}

		template<class T>
		constexpr basic_json_value(const T& v)
			: value{v} {}

		template<class T>
		constexpr basic_json_value(T&& v)
			: value{std::forward<T>(v)} {}

		constexpr basic_json_value& operator[](const string_type& key)
		{
			return as_object()[key];
		}

		constexpr basic_json_value& operator[](std::size_t key)
			requires(requires(list_type& l) { l[0]; })
		{
			return as_list()[key];
		}

		constexpr const basic_json_value& operator[](std::size_t key) const
			requires(requires(const list_type& l) { l[0]; })
		{
			return as_list()[key];
		}

		constexpr basic_json_value& at(const string_type& key)
		{
			return as_object().at(key);
		}

		const basic_json_value& at(const string_type& key) const
		{
			return as_object().at(key);
		}

		constexpr basic_json_value& at(std::size_t key)
		{
			return as_list().at(key);
		}

		constexpr const basic_json_value& at(std::size_t key) const
		{
			return as_list().at(key);
		}

		constexpr boolean_type& as_boolean()
		{
			return std::get<boolean_type>(value);
		}

		constexpr const boolean_type& as_boolean() const
		{
			return std::get<boolean_type>(value);
		}

		constexpr string_type& as_string()
		{
			return std::get<string_type>(value);
		}

		constexpr const string_type& as_string() const
		{
			return std::get<string_type>(value);
		}

		constexpr int_type& as_integer()
		{
			return std::get<int_type>(value);
		}

		constexpr const int_type& as_integer() const
		{
			return std::get<int_type>(value);
		}

		constexpr float_type& as_floating()
		{
			return std::get<float_type>(value);
		}

		constexpr const float_type& as_floating() const
		{
			return std::get<float_type>(value);
		}

		constexpr list_type& as_list()
		{
			return std::get<list_type>(value);
		}

		constexpr const list_type& as_list() const
		{
			return std::get<list_type>(value);
		}

		constexpr object_type& as_object()
		{
			return std::get<object_type>(value);
		}

		constexpr const object_type& as_object() const
		{
			return std::get<object_type>(value);
		}

		constexpr bool is_boolean() const noexcept
		{
			return std::holds_alternative<boolean_type>(value);
		}

		constexpr bool is_string() const noexcept
		{
			return std::holds_alternative<string_type>(value);
		}

		constexpr bool is_integer() const noexcept
		{
			return std::holds_alternative<int_type>(value);
		}

		constexpr bool is_floating() const noexcept
		{
			return std::holds_alternative<float_type>(value);
		}

		constexpr bool is_null() const noexcept
		{
			return std::holds_alternative<null_type>(value);
		}

		constexpr bool is_list() const noexcept
		{
			return std::holds_alternative<list_type>(value);
		}

		constexpr bool is_object() const noexcept
		{
			return std::holds_alternative<object_type>(value);
		}

		type value {};
	};

	export
	template<class Allocator = std::allocator<void>>
	using json_value = basic_json_value<bool, std::string, std::intmax_t, long double, std::list, std::map, Allocator>;
}
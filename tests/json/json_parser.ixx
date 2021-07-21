export module cmoon.tests.json.json_parser;

import cmoon.test;
import cmoon.json;

namespace cmoon::tests::json
{
	export
	class json_parse_test : public collin::test::test_case
	{
		public:
			json_parse_test()
				: collin::test::test_case{"json_parse_test"} {}

			void operator()() override
			{
				std::ifstream json_file {"jsonExample.json"};
				collin::json::json_value<> data;
				json_file >> data;

				collin::test::assert_true(data.is_object(),
					"First type is not an object"
				);

				collin::test::assert_true(data.at("glossary").is_object(),
					"\"glossary\" is not an object"
				);

				collin::test::assert_equal(data.at("glossary").at("title").as_string(), 
					"example glossary",
					"\"glossary\" has an incorrect value"
				);

				collin::test::assert_true(data.at("glossary").at("GlossDiv").is_object(),
					"\"GlossDiv\" is not an object"
				);

				collin::test::assert_true(data.at("glossary").at("GlossDiv").at("title").is_string(),
					"\"GlossDiv::title\" is not a string"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("title").as_string(), 
					"S",
					"\"GlossDiv::title\" has an incorrect value"
				);

				collin::test::assert_true(data.at("glossary").at("GlossDiv").at("GlossList").is_object(),
					"\"GlossList\" is not an object"
				);

				collin::test::assert_true(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").is_object(),
					"\"GlossEntry\" is not an object"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("ID").as_string(), 
					"SGML",
					"\"GlossEntry::ID\" has an incorrect value"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("SortAs").as_string(), 
					"SGML",
					"\"GlossEntry::SortAt\" has an incorrect value"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("GlossTerm").as_string(), 
					"Standard Generalized Markup Language",
					"\"GlossEntry::GlossTerm\" has an incorrect value"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("Acronym").as_string(), 
					"SGML",
					"\"GlossEntry::Acronym\" has an incorrect value"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("Abbrev").as_string(), 
					"ISO 8879:1986",
					"\"GlossEntry::Abbrev\" has an incorrect value"
				);

				collin::test::assert_true(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("GlossDef").is_object(),
					"\"GlossDef\" is not an object"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("GlossDef").at("para").as_string(), 
					"A meta-markup language, used to create markup languages such as DocBook.",
					"\"GlossEntry::para\" has an incorrect value"
				);

				collin::test::assert_true(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("GlossDef").at("GlossSeeAlso").is_list(),
					"\"GlossSeeAlso\" is not a list"
				);

				collin::test::assert_equal(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("GlossDef").at("GlossSeeAlso").as_list().front().as_string(), 
					"GML",
					"\"GlossSeeAlso::0\" has an incorrect value"
				);

				collin::test::assert_equal(collin::first_of(data.at("glossary").at("GlossDiv").at("GlossList").at("GlossEntry").at("GlossDef").at("GlossSeeAlso").as_list(), 1).as_string(), 
					"XML",
					"\"GlossSeeAlso::1\" has an incorrect value"
				);

				collin::test::assert_true(data.at("ThisIsAnInteger").is_integer(),
					"\"ThisIsAnInteger\" is not an integer"
				);

				collin::test::assert_true(data.at("ThisIsAFloat").is_floating(),
					"\"ThisIsAFloat\" is not a float"
				);

				collin::test::assert_true(data.at("ThisIsATrue").is_boolean(),
					"\"ThisIsATrue\" is not a boolean"
				);

				collin::test::assert_true(data.at("ThisIsAFalse").is_boolean(),
					"\"ThisIsAFalse\" is not a boolean"
				);

				collin::test::assert_true(data.at("ThisIsANull").is_null(),
					"\"ThisIsANull\" is not null"
				);

				collin::test::assert_equal(data.at("ThisIsAnInteger").as_integer(), 
					50,
					"\"ThisIsAnInteger\" has an incorrect value"
				);

				collin::test::assert_almost_equal(data.at("ThisIsAFloat").as_floating(), 
					3.14, 0.0000001,
					"\"ThisIsAFloat\" has an incorrect value"
				);

				collin::test::assert_equal(data.at("ThisIsATrue").as_boolean(), 
					true,
					"\"ThisIsATrue\" has an incorrect value"
				);

				collin::test::assert_equal(data.at("ThisIsAFalse").as_boolean(), 
					false,
					"\"ThisIsAFalse\" has an incorrect value"
				);
			}
	};
}
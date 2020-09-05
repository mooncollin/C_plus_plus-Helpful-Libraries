#pragma once

#include <string_view>
#include <array>
#include <algorithm>

namespace collin
{
	namespace finance
	{
		template<std::size_t CodeLength>
		class basic_currency_code
		{
			public:
				constexpr basic_currency_code() noexcept = default;
				constexpr basic_currency_code(std::string_view c) noexcept
				{
					code(c);
				}

				constexpr std::string_view code() const noexcept
				{
					return std::string_view{code_.data(), CodeLength};
				}

				constexpr void code(std::string_view c) noexcept
				{
					const auto s = std::min(CodeLength, std::size(c));
					std::copy_n(std::begin(c), s, std::begin(code_));
					std::fill(std::begin(code_) + s, std::end(code_), '\0');
				}

				constexpr operator std::string_view() const noexcept
				{
					return code();
				}
			private:
				std::array<char, CodeLength> code_ {};
		};

		constexpr std::size_t standard_currency_code_length {3};

		using currency_code = basic_currency_code<standard_currency_code_length>;

		template<std::size_t CodeLength>
		class basic_currency
		{
			public:
				constexpr basic_currency() noexcept = default;
				constexpr basic_currency(const basic_currency_code<CodeLength>& code, unsigned int num)
					: code_{code}, num_{num} {}

				constexpr basic_currency_code<CodeLength>& currency_code() noexcept
				{
					return code_;
				}

				constexpr const basic_currency_code<CodeLength>& currency_code() const noexcept
				{
					return code_;
				}

				constexpr unsigned int& currency_number() noexcept
				{
					return num_;
				}

				constexpr unsigned int currency_number() const noexcept
				{
					return num_;
				}

			private:
				basic_currency_code<CodeLength> code_ {};
				unsigned int num_ {};
		};

		using currency = basic_currency<standard_currency_code_length>;

		namespace currencies
		{
			constexpr currency albanian_lek{{"ALL"}, 8};
			constexpr currency algerian_dinar{{"DZD"}, 12};
			constexpr currency argentine_peso{{"ARS"}, 32};
			constexpr currency australian_dollar{{"AUD"}, 36};
			constexpr currency bahamian_dollar{{"BSD"}, 44};
			constexpr currency bahraini_dinar{{"BHD"}, 48};
			constexpr currency bangladeshi_taka{{"BDT"}, 50};
			constexpr currency armenian_dram{{"AMD"}, 51};
			constexpr currency barbados_dollar{{"BBD"}, 52};
			constexpr currency bermedian_dollar{{"BMD"}, 60};
			constexpr currency bhutanese_ngultrum{{"BTN"}, 64};
			constexpr currency boliviano{{"BOB"}, 68};
			constexpr currency botswana_pula{{"BWP"}, 72};
			constexpr currency belize_dollar{{"BZD"}, 84};
			constexpr currency solomon_islands_dollar{{"SBD"}, 90};
			constexpr currency brunei_dollar{{"BND"}, 96};
			constexpr currency myanmar_kyat{{"MMK"}, 104};
			constexpr currency burundian_franc{{"BIF"}, 108};
			constexpr currency cambodian_riel{{"KHR"}, 116};
			constexpr currency canadian_dollar{{"CAD"}, 124};
			constexpr currency cape_verdean_escudo{{"CVE"}, 132};
			constexpr currency cayman_islands_dollar{{"KYD"}, 136};
			constexpr currency sri_lankan_rupee{{"LKR"}, 144};
			constexpr currency chilean_peso{{"CLP"}, 152};
			constexpr currency chinese_yuan{{"CNY"}, 156};
			constexpr currency colombian_peso{{"COP"}, 170};
			constexpr currency comoro_franc{{"KMF"}, 174};
			constexpr currency costa_rican_colon{{"CRC"}, 188};
			constexpr currency croatian_kuna{{"HRK"}, 191};
			constexpr currency cuban_peso{{"CUP"}, 192};
			constexpr currency czech_koruna{{"CZK"}, 203};
			constexpr currency danish_krone{{"DKK"}, 208};
			constexpr currency dominican_peso{{"DOP"}, 214};
			constexpr currency salvadoran_colon{{"SVC"}, 222};
			constexpr currency ethiopian_birr{{"ETB"}, 230};
			constexpr currency eritrean_nakfa{{"ERN"}, 232};
			constexpr currency falkland_islands_pound{{"FKP"}, 238};
			constexpr currency fiji_dollar{{"FJD"}, 242};
			constexpr currency djiboutian_franc{{"DJF"}, 262};
			constexpr currency gambian_dalasi{{"GMD"}, 270};
			constexpr currency gibraltar_pound{{"GIP"}, 292};
			constexpr currency guatemalan_quetzal{{"GTQ"}, 320};
			constexpr currency guinean_franc{{"GNF"}, 324};
			constexpr currency guyanese_dollar{{"GYD"}, 328};
			constexpr currency haitian_gourde{{"HTG"}, 332};
			constexpr currency honduran_lempira{{"HNL"}, 340};
			constexpr currency hong_kong_dollar{{"HKD"}, 344};
			constexpr currency hungarian_forint{{"HUF"}, 348};
			constexpr currency iclandic_krona{{"ISK"}, 352};
			constexpr currency indian_rupee{{"INR"}, 356};
			constexpr currency indonesian_rupiah{{"IDR"}, 360};
			constexpr currency iranian_rial{{"IRR"}, 364};
			constexpr currency iraqi_dinar{{"IQD"}, 368};
			constexpr currency israeli_new_shekel{{"ILS"}, 376};
			constexpr currency jamaican_dollar{{"JMD"}, 388};
			constexpr currency japanese_yen{{"JPY"}, 392};
			constexpr currency kazakhstani_tenge{{"KZT"}, 398};
			constexpr currency jordanian_dinar{{"JOD"}, 400};
			constexpr currency kenyan_shilling{{"KES"}, 404};
			constexpr currency north_korean_won{{"KPW"}, 408};
			constexpr currency south_korean_won{{"KRW"}, 410};
			constexpr currency kuwaiti_dinar{{"KWD"}, 414};
			constexpr currency kyrgyzstani_som{{"KGS"}, 417};
			constexpr currency lao_kip{{"LAK"}, 418};
			constexpr currency lebanese_pound{{"LBP"}, 422};
			constexpr currency lesotho_loti{{"LSL"}, 426};
			constexpr currency liberian_dollar{{"LRD"}, 430};
			constexpr currency libyan_dinar{{"LYD"}, 434};
			constexpr currency macanese_pataca{{"MOP"}, 446};
			constexpr currency malawian_kwacha{{"MWK"}, 454};
			constexpr currency malysian_ringgit{{"MYR"}, 458};
			constexpr currency maldivian_rufiyaa{{"MVR"}, 462};
			constexpr currency mauritian_rupee{{"MUR"}, 480};
			constexpr currency mexican_peso{{"MXN"}, 484};
			constexpr currency mongolian_togrog{{"MNT"}, 496};
			constexpr currency moldovan_leu{{"MDL"}, 498};
			constexpr currency moroccan_dirham{{"MAD"}, 504};
			constexpr currency omani_rial{{"OMR"}, 512};
			constexpr currency namibian_dollar{{"NAD"}, 516};
			constexpr currency nepalese_rupee{{"NPR"}, 524};
			constexpr currency netherlands_antillean_guilder{{"ANG"}, 532};
			constexpr currency aruban_florin{{"AWG"}, 533};
			constexpr currency vanuatu_vatu{{"VUV"}, 548};
			constexpr currency new_zealand_dollar{{"NZD"}, 554};
			constexpr currency nicaraguan_cordoba{{"NIO"}, 558};
			constexpr currency nigerian_naira{{"NGN"}, 566};
			constexpr currency norwegian_krone{{"NOK"}, 578};
			constexpr currency pakistani_rupee{{"PKR"}, 586};
			constexpr currency panamanian_balboa{{"PAB"}, 590};
			constexpr currency papua_new_guinean_kina{{"PGK"}, 598};
			constexpr currency paraguayan_guarani{{"PYG"}, 600};
			constexpr currency peruvian_sol{{"PEN"}, 604};
			constexpr currency philippine_peso{{"PHP"}, 608};
			constexpr currency qatari_riyal{{"QAR"}, 634};
			constexpr currency russian_ruble{{"RUB"}, 643};
			constexpr currency rwandan_franc{{"RWF"}, 646};
			constexpr currency saint_helena_pound{{"SHP"}, 654};
			constexpr currency saudi_riyal{{"SAR"}, 682};
			constexpr currency seychelles_rupee{{"SCR"}, 690};
			constexpr currency sierra_leanean_leone{{"SLL"}, 694};
			constexpr currency singapore_dollar{{"SGD"}, 702};
			constexpr currency vietnamese_dong{{"VND"}, 704};
			constexpr currency somali_shilling{{"SOS"}, 706};
			constexpr currency south_african_rand{{"ZAR"}, 710};
			constexpr currency south_sudanese_pound{{"SSP"}, 728};
			constexpr currency swazi_lilangeni{{"SZL"}, 748};
			constexpr currency swedish_krona{{"SEK"}, 752};
			constexpr currency swiss_franc{{"CHF"}, 756};
			constexpr currency syrian_pound{{"SYP"}, 760};
			constexpr currency thai_baht{{"THB"}, 764};
			constexpr currency tongan_paanga{{"TOP"}, 776};
			constexpr currency trinidata_tobago_dollar{{"TTD"}, 780};
			constexpr currency united_arab_emirates_dirham{{"AED"}, 784};
			constexpr currency tunisian_dinar{{"TND"}, 788};
			constexpr currency ugandan_shilling{{"UGX"}, 800};
			constexpr currency macedonian_denar{{"MKD"}, 807};
			constexpr currency egyptian_pound{{"EGP"}, 818};
			constexpr currency pound_sterling{{"GBP"}, 826};
			constexpr currency tanzanian_shilling{{"TZS"}, 834};
			constexpr currency united_states_dollar{{"USD"}, 840};
			constexpr currency uruguayan_peso{{"UYU"}, 858};
			constexpr currency uzbekistan_som{{"UZS"}, 860};
			constexpr currency samoan_tala{{"WST"}, 882};
			constexpr currency yemeni_rial{{"YER"}, 886};
			constexpr currency new_taiwan_dollar{{"TWD"}, 901};
			constexpr currency unidad_previsional{{"UYW"}, 927};
			constexpr currency venezuelan_bolivar_soberano{{"VES"}, 928};
			constexpr currency mauritanian_ouguiya{{"MRU"}, 929};
			constexpr currency sao_tome_and_principe_dobra{{"STN"}, 930};
			constexpr currency cuban_convertible_peso{{"CUC"}, 931};
			constexpr currency zimbabwean_dollar{{"ZWL"}, 932};
			constexpr currency belarusian_ruble{{"BYN"}, 933};
			constexpr currency turkmenistan_manat{{"TMT"}, 934};
			constexpr currency ghanaian_cedi{{"GHS"}, 936};
			constexpr currency sudanese_pound{{"SDG"}, 938};
			constexpr currency uruguay_peso_unidades_indexadas{{"UYI"}, 940};
			constexpr currency serbian_dinar{{"RSD"}, 941};
			constexpr currency mozambican_metical{{"MZN"}, 943};
			constexpr currency azerbaijani_manat{{"AZN"}, 944};
			constexpr currency romanian_leu{{"RON"}, 946};
			constexpr currency wir_euro{{"CHE"}, 947};
			constexpr currency wir_franc{{"CHW"}, 948};
			constexpr currency turkish_lira{{"TRY"}, 949};
			constexpr currency cfa_franc_beac{{"XAF"}, 950};
			constexpr currency east_caribbean_dollar{{"XCD"}, 951};
			constexpr currency cfa_franc_bceao{{"XOF"}, 952};
			constexpr currency cfp_franc{{"XPF"}, 953};
			constexpr currency european_composite_unit{{"XBA"}, 955};
			constexpr currency european_monetary_unit{{"XBB"}, 956};
			constexpr currency european_unit_account_9{{"XBC"}, 957};
			constexpr currency european_unit_account_17{{"XBD"}, 958};
			constexpr currency gold{{"XAU"}, 959};
			constexpr currency special_drawing_rights{{"XDR"}, 960};
			constexpr currency silver{{"XAG"}, 961};
			constexpr currency platinum{{"XPT"}, 962};
			constexpr currency palladium{{"XPD"}, 964};
			constexpr currency abd_unit_account{{"XUA"}, 965};
			constexpr currency zambian_kwacha{{"ZMW"}, 967};
			constexpr currency surinamese_dollar{{"SRD"}, 968};
			constexpr currency malagasy_ariary{{"MGA"}, 969};
			constexpr currency unidad_valor_real{{"COU"}, 970};
			constexpr currency afghan_afghani{{"AFN"}, 971};
			constexpr currency tajikistani_somoni{{"TJS"}, 972};
			constexpr currency angolan_kwanza{{"AOA"}, 973};
			constexpr currency bulgarian_lev{{"BGN"}, 975};
			constexpr currency congolese_franc{{"CDF"}, 976};
			constexpr currency bosnia_herzegovina_convertible_mark{{"BAM"}, 977};
			constexpr currency euro{{"EUR"}, 978};
			constexpr currency mexican_unidad_inversion{{"MXV"}, 979};
			constexpr currency ukrainian_hryvnia{{"UAH"}, 980};
			constexpr currency georgian_lari{{"GEL"}, 981};
			constexpr currency bolivian_mvdol{{"BOV"}, 984};
			constexpr currency polish_zloty{{"PLN"}, 985};
			constexpr currency brazilian_real{{"BRL"}, 986};
			constexpr currency unidad_fomento{{"CLF"}, 990};
			constexpr currency sucre{{"XSU"}, 994};
		}
	}
}
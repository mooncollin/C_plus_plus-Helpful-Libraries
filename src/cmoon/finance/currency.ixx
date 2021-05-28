export module cmoon.finance.currency;

import <string_view>;
import <array>;
import <algorithm>;

namespace cmoon::finance
{
	export
	template<std::size_t CodeLength>
	class basic_currency_code
	{
		public:
			constexpr basic_currency_code() noexcept = default;
			constexpr basic_currency_code(std::string_view c) noexcept
			{
				const auto s = std::min(CodeLength, std::size(c));
				std::copy_n(std::begin(c), s, std::begin(code_));
				std::fill(std::begin(code_) + s, std::end(code_), '\0');
			}

			constexpr std::string_view code() const noexcept
			{
				return std::string_view{code_.data(), CodeLength};
			}

			constexpr operator std::string_view() const noexcept
			{
				return code();
			}

			[[nodiscard]] friend constexpr bool operator==(const basic_currency_code&, const basic_currency_code&) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const basic_currency_code&, const basic_currency_code&) noexcept = default;
		private:
			std::array<char, CodeLength> code_ {};
	};

	export
	inline constexpr std::size_t standard_currency_code_length {3};

	export
	using currency_code = basic_currency_code<standard_currency_code_length>;

	export
	template<std::size_t CodeLength>
	class basic_currency
	{
		public:
			constexpr basic_currency() noexcept = default;
			constexpr basic_currency(const basic_currency_code<CodeLength>& code, unsigned num)
				: code_{code}, num_{num} {}

			constexpr const basic_currency_code<CodeLength>& currency_code() const noexcept
			{
				return code_;
			}

			constexpr unsigned currency_number() const noexcept
			{
				return num_;
			}

			[[nodiscard]] friend constexpr bool operator==(const basic_currency& x, const basic_currency& y) noexcept
			{
				return x.num_ == y.num_;
			}

			[[nodiscard]] friend constexpr bool operator!=(const basic_currency&, const basic_currency&) noexcept = default;
		private:
			basic_currency_code<CodeLength> code_ {};
			unsigned num_ {};
	};

	export
	using currency = basic_currency<standard_currency_code_length>;

	namespace currencies
	{
		export
		inline constexpr currency albanian_lek{{"ALL"}, 8};
		
		export
		inline constexpr currency algerian_dinar{{"DZD"}, 12};
		
		export
		inline constexpr currency argentine_peso{{"ARS"}, 32};
		
		export
		inline constexpr currency australian_dollar{{"AUD"}, 36};
		
		export
		inline constexpr currency bahamian_dollar{{"BSD"}, 44};
		
		export
		inline constexpr currency bahraini_dinar{{"BHD"}, 48};
		
		export
		inline constexpr currency bangladeshi_taka{{"BDT"}, 50};
		
		export
		inline constexpr currency armenian_dram{{"AMD"}, 51};
		
		export
		inline constexpr currency barbados_dollar{{"BBD"}, 52};
		
		export
		inline constexpr currency bermedian_dollar{{"BMD"}, 60};
		
		export
		inline constexpr currency bhutanese_ngultrum{{"BTN"}, 64};
		
		export
		inline constexpr currency boliviano{{"BOB"}, 68};
		
		export
		inline constexpr currency botswana_pula{{"BWP"}, 72};
		
		export
		inline constexpr currency belize_dollar{{"BZD"}, 84};
		
		export
		inline constexpr currency solomon_islands_dollar{{"SBD"}, 90};
		
		export
		inline constexpr currency brunei_dollar{{"BND"}, 96};
		
		export
		inline constexpr currency myanmar_kyat{{"MMK"}, 104};
		
		export
		inline constexpr currency burundian_franc{{"BIF"}, 108};
		
		export
		inline constexpr currency cambodian_riel{{"KHR"}, 116};
		
		export
		inline constexpr currency canadian_dollar{{"CAD"}, 124};
		
		export
		inline constexpr currency cape_verdean_escudo{{"CVE"}, 132};
		
		export
		inline constexpr currency cayman_islands_dollar{{"KYD"}, 136};
		
		export
		inline constexpr currency sri_lankan_rupee{{"LKR"}, 144};
		
		export
		inline constexpr currency chilean_peso{{"CLP"}, 152};
		
		export
		inline constexpr currency chinese_yuan{{"CNY"}, 156};
		
		export
		inline constexpr currency colombian_peso{{"COP"}, 170};
		
		export
		inline constexpr currency comoro_franc{{"KMF"}, 174};
		
		export
		inline constexpr currency costa_rican_colon{{"CRC"}, 188};
		
		export
		inline constexpr currency croatian_kuna{{"HRK"}, 191};
		
		export
		inline constexpr currency cuban_peso{{"CUP"}, 192};
		
		export
		inline constexpr currency czech_koruna{{"CZK"}, 203};
		
		export
		inline constexpr currency danish_krone{{"DKK"}, 208};
		
		export
		inline constexpr currency dominican_peso{{"DOP"}, 214};
		
		export
		inline constexpr currency salvadoran_colon{{"SVC"}, 222};
		
		export
		inline constexpr currency ethiopian_birr{{"ETB"}, 230};
		
		export
		inline constexpr currency eritrean_nakfa{{"ERN"}, 232};
		
		export
		inline constexpr currency falkland_islands_pound{{"FKP"}, 238};
		
		export
		inline constexpr currency fiji_dollar{{"FJD"}, 242};
		
		export
		inline constexpr currency djiboutian_franc{{"DJF"}, 262};
		
		export
		inline constexpr currency gambian_dalasi{{"GMD"}, 270};
		
		export
		inline constexpr currency gibraltar_pound{{"GIP"}, 292};
		
		export
		inline constexpr currency guatemalan_quetzal{{"GTQ"}, 320};
		
		export
		inline constexpr currency guinean_franc{{"GNF"}, 324};
		
		export
		inline constexpr currency guyanese_dollar{{"GYD"}, 328};
		
		export
		inline constexpr currency haitian_gourde{{"HTG"}, 332};
		
		export
		inline constexpr currency honduran_lempira{{"HNL"}, 340};
		
		export
		inline constexpr currency hong_kong_dollar{{"HKD"}, 344};
		
		export
		inline constexpr currency hungarian_forint{{"HUF"}, 348};
		
		export
		inline constexpr currency iclandic_krona{{"ISK"}, 352};
		
		export
		inline constexpr currency indian_rupee{{"INR"}, 356};
		
		export
		inline constexpr currency indonesian_rupiah{{"IDR"}, 360};
		
		export
		inline constexpr currency iranian_rial{{"IRR"}, 364};
		
		export
		inline constexpr currency iraqi_dinar{{"IQD"}, 368};
		
		export
		inline constexpr currency israeli_new_shekel{{"ILS"}, 376};
		
		export
		inline constexpr currency jamaican_dollar{{"JMD"}, 388};
		
		export
		inline constexpr currency japanese_yen{{"JPY"}, 392};
		
		export
		inline constexpr currency kazakhstani_tenge{{"KZT"}, 398};
		
		export
		inline constexpr currency jordanian_dinar{{"JOD"}, 400};
		
		export
		inline constexpr currency kenyan_shilling{{"KES"}, 404};
		
		export
		inline constexpr currency north_korean_won{{"KPW"}, 408};
		
		export
		inline constexpr currency south_korean_won{{"KRW"}, 410};
		
		export
		inline constexpr currency kuwaiti_dinar{{"KWD"}, 414};
		
		export
		inline constexpr currency kyrgyzstani_som{{"KGS"}, 417};
		
		export
		inline constexpr currency lao_kip{{"LAK"}, 418};
		
		export
		inline constexpr currency lebanese_pound{{"LBP"}, 422};
		
		export
		inline constexpr currency lesotho_loti{{"LSL"}, 426};
		
		export
		inline constexpr currency liberian_dollar{{"LRD"}, 430};
		
		export
		inline constexpr currency libyan_dinar{{"LYD"}, 434};
		
		export
		inline constexpr currency macanese_pataca{{"MOP"}, 446};
		
		export
		inline constexpr currency malawian_kwacha{{"MWK"}, 454};
		
		export
		inline constexpr currency malysian_ringgit{{"MYR"}, 458};
		
		export
		inline constexpr currency maldivian_rufiyaa{{"MVR"}, 462};
		
		export
		inline constexpr currency mauritian_rupee{{"MUR"}, 480};
		
		export
		inline constexpr currency mexican_peso{{"MXN"}, 484};
		
		export
		inline constexpr currency mongolian_togrog{{"MNT"}, 496};
		
		export
		inline constexpr currency moldovan_leu{{"MDL"}, 498};
		
		export
		inline constexpr currency moroccan_dirham{{"MAD"}, 504};
		
		export
		inline constexpr currency omani_rial{{"OMR"}, 512};
		
		export
		inline constexpr currency namibian_dollar{{"NAD"}, 516};
		
		export
		inline constexpr currency nepalese_rupee{{"NPR"}, 524};
		
		export
		inline constexpr currency netherlands_antillean_guilder{{"ANG"}, 532};
		
		export
		inline constexpr currency aruban_florin{{"AWG"}, 533};
		
		export
		inline constexpr currency vanuatu_vatu{{"VUV"}, 548};
		
		export
		inline constexpr currency new_zealand_dollar{{"NZD"}, 554};
		
		export
		inline constexpr currency nicaraguan_cordoba{{"NIO"}, 558};
		
		export
		inline constexpr currency nigerian_naira{{"NGN"}, 566};
		
		export
		inline constexpr currency norwegian_krone{{"NOK"}, 578};
		
		export
		inline constexpr currency pakistani_rupee{{"PKR"}, 586};
		
		export
		inline constexpr currency panamanian_balboa{{"PAB"}, 590};
		
		export
		inline constexpr currency papua_new_guinean_kina{{"PGK"}, 598};
		
		export
		inline constexpr currency paraguayan_guarani{{"PYG"}, 600};
		
		export
		inline constexpr currency peruvian_sol{{"PEN"}, 604};
		
		export
		inline constexpr currency philippine_peso{{"PHP"}, 608};
		
		export
		inline constexpr currency qatari_riyal{{"QAR"}, 634};
		
		export
		inline constexpr currency russian_ruble{{"RUB"}, 643};
		
		export
		inline constexpr currency rwandan_franc{{"RWF"}, 646};
		
		export
		inline constexpr currency saint_helena_pound{{"SHP"}, 654};
		
		export
		inline constexpr currency saudi_riyal{{"SAR"}, 682};
		
		export
		inline constexpr currency seychelles_rupee{{"SCR"}, 690};
		
		export
		inline constexpr currency sierra_leanean_leone{{"SLL"}, 694};
		
		export
		inline constexpr currency singapore_dollar{{"SGD"}, 702};
		
		export
		inline constexpr currency vietnamese_dong{{"VND"}, 704};
		
		export
		inline constexpr currency somali_shilling{{"SOS"}, 706};
		
		export
		inline constexpr currency south_african_rand{{"ZAR"}, 710};
		
		export
		inline constexpr currency south_sudanese_pound{{"SSP"}, 728};
		
		export
		inline constexpr currency swazi_lilangeni{{"SZL"}, 748};
		
		export
		inline constexpr currency swedish_krona{{"SEK"}, 752};
		
		export
		inline constexpr currency swiss_franc{{"CHF"}, 756};
		
		export
		inline constexpr currency syrian_pound{{"SYP"}, 760};
		
		export
		inline constexpr currency thai_baht{{"THB"}, 764};
		
		export
		inline constexpr currency tongan_paanga{{"TOP"}, 776};
		
		export
		inline constexpr currency trinidata_tobago_dollar{{"TTD"}, 780};
		
		export
		inline constexpr currency united_arab_emirates_dirham{{"AED"}, 784};
		
		export
		inline constexpr currency tunisian_dinar{{"TND"}, 788};
		
		export
		inline constexpr currency ugandan_shilling{{"UGX"}, 800};
		
		export
		inline constexpr currency macedonian_denar{{"MKD"}, 807};
		
		export
		inline constexpr currency egyptian_pound{{"EGP"}, 818};
		
		export
		inline constexpr currency pound_sterling{{"GBP"}, 826};
		
		export
		inline constexpr currency tanzanian_shilling{{"TZS"}, 834};
		
		export
		inline constexpr currency united_states_dollar{{"USD"}, 840};
		
		export
		inline constexpr currency uruguayan_peso{{"UYU"}, 858};
		
		export
		inline constexpr currency uzbekistan_som{{"UZS"}, 860};
		
		export
		inline constexpr currency samoan_tala{{"WST"}, 882};
		
		export
		inline constexpr currency yemeni_rial{{"YER"}, 886};
		
		export
		inline constexpr currency new_taiwan_dollar{{"TWD"}, 901};
		
		export
		inline constexpr currency unidad_previsional{{"UYW"}, 927};
		
		export
		inline constexpr currency venezuelan_bolivar_soberano{{"VES"}, 928};
		
		export
		inline constexpr currency mauritanian_ouguiya{{"MRU"}, 929};
		
		export
		inline constexpr currency sao_tome_and_principe_dobra{{"STN"}, 930};
		
		export
		inline constexpr currency cuban_convertible_peso{{"CUC"}, 931};
		
		export
		inline constexpr currency zimbabwean_dollar{{"ZWL"}, 932};
		
		export
		inline constexpr currency belarusian_ruble{{"BYN"}, 933};
		
		export
		inline constexpr currency turkmenistan_manat{{"TMT"}, 934};
		
		export
		inline constexpr currency ghanaian_cedi{{"GHS"}, 936};
		
		export
		inline constexpr currency sudanese_pound{{"SDG"}, 938};
		
		export
		inline constexpr currency uruguay_peso_unidades_indexadas{{"UYI"}, 940};
		
		export
		inline constexpr currency serbian_dinar{{"RSD"}, 941};
		
		export
		inline constexpr currency mozambican_metical{{"MZN"}, 943};
		
		export
		inline constexpr currency azerbaijani_manat{{"AZN"}, 944};
		
		export
		inline constexpr currency romanian_leu{{"RON"}, 946};
		
		export
		inline constexpr currency wir_euro{{"CHE"}, 947};
		
		export
		inline constexpr currency wir_franc{{"CHW"}, 948};
		
		export
		inline constexpr currency turkish_lira{{"TRY"}, 949};
		
		export
		inline constexpr currency cfa_franc_beac{{"XAF"}, 950};
		
		export
		inline constexpr currency east_caribbean_dollar{{"XCD"}, 951};
		
		export
		inline constexpr currency cfa_franc_bceao{{"XOF"}, 952};
		
		export
		inline constexpr currency cfp_franc{{"XPF"}, 953};
		
		export
		inline constexpr currency european_composite_unit{{"XBA"}, 955};
		
		export
		inline constexpr currency european_monetary_unit{{"XBB"}, 956};
		
		export
		inline constexpr currency european_unit_account_9{{"XBC"}, 957};
		
		export
		inline constexpr currency european_unit_account_17{{"XBD"}, 958};
		
		export
		inline constexpr currency gold{{"XAU"}, 959};
		
		export
		inline constexpr currency special_drawing_rights{{"XDR"}, 960};
		
		export
		inline constexpr currency silver{{"XAG"}, 961};
		
		export
		inline constexpr currency platinum{{"XPT"}, 962};
		
		export
		inline constexpr currency palladium{{"XPD"}, 964};
		
		export
		inline constexpr currency abd_unit_account{{"XUA"}, 965};
		
		export
		inline constexpr currency zambian_kwacha{{"ZMW"}, 967};
		
		export
		inline constexpr currency surinamese_dollar{{"SRD"}, 968};
		
		export
		inline constexpr currency malagasy_ariary{{"MGA"}, 969};
		
		export
		inline constexpr currency unidad_valor_real{{"COU"}, 970};
		
		export
		inline constexpr currency afghan_afghani{{"AFN"}, 971};
		
		export
		inline constexpr currency tajikistani_somoni{{"TJS"}, 972};
		
		export
		inline constexpr currency angolan_kwanza{{"AOA"}, 973};
		
		export
		inline constexpr currency bulgarian_lev{{"BGN"}, 975};
		
		export
		inline constexpr currency congolese_franc{{"CDF"}, 976};
		
		export
		inline constexpr currency bosnia_herzegovina_convertible_mark{{"BAM"}, 977};
		
		export
		inline constexpr currency euro{{"EUR"}, 978};
		
		export
		inline constexpr currency mexican_unidad_inversion{{"MXV"}, 979};
		
		export
		inline constexpr currency ukrainian_hryvnia{{"UAH"}, 980};
		
		export
		inline constexpr currency georgian_lari{{"GEL"}, 981};
		
		export
		inline constexpr currency bolivian_mvdol{{"BOV"}, 984};
		
		export
		inline constexpr currency polish_zloty{{"PLN"}, 985};
		
		export
		inline constexpr currency brazilian_real{{"BRL"}, 986};
		
		export
		inline constexpr currency unidad_fomento{{"CLF"}, 990};
		
		export
		inline constexpr currency sucre{{"XSU"}, 994};
	}
}
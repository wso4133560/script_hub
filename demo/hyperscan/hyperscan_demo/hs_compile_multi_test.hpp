#pragma once
#include "Common.hpp"
#include <string>
// hs_compile_multiº¯Êý²âÊÔ

class HsCompileMultiTest
{
public:
	void TestModeBlock()
	{
		hs_database_t *db = nullptr;
		hs_compile_error_t *compile_err = nullptr;
		CallBackContext c;
		std::string data = "aooAaooAbarZ";
		const char *expr[] = { "aoo[A-K]", "bar[L-Z]" };
		unsigned flags[] = { 0, 0 };
		unsigned ids[] = { 30, 31 };
		hs_error_t err = hs_compile_multi(expr, flags, ids, 2, HS_MODE_NOSTREAM, nullptr, &db, &compile_err);

		hs_scratch_t *scratch = nullptr;
		err = hs_alloc_scratch(db, &scratch);

		c.halt = 0;
		err = hs_scan(db, data.c_str(), data.size(), 0, scratch, record_cb, (void *)&c);
		std::cout << "err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		hs_free_database(db);
		err = hs_free_scratch(scratch);
	}
	
	void TestLogic()
	{
		hs_database_t *db = nullptr;
		hs_compile_error_t *compile_err = nullptr;
		const char *expr[] = {
			 "abc",
			"def",
			"foobar.*gh",
			"teakettle{4,10}",
			"ijkl[mMn]",
			"(101 & 102 & 103) | (104 & !105)",
			"!101 & 102",
			"!(!101 | 102)",
			"101 & !102" 
		};
		unsigned flags[] = {
			 HS_FLAG_QUIET,
			HS_FLAG_QUIET,
			HS_FLAG_QUIET,
			HS_FLAG_QUIET,
			0,
			HS_FLAG_COMBINATION | HS_FLAG_SINGLEMATCH,
			HS_FLAG_COMBINATION,
			HS_FLAG_COMBINATION | HS_FLAG_SINGLEMATCH,
			HS_FLAG_COMBINATION | HS_FLAG_SINGLEMATCH 
		};
		unsigned ids[] = { 101, 102, 103, 104, 105, 1001, 1002, 1003, 1004 };
		hs_error_t err = hs_compile_multi(expr, flags, ids, 9, HS_MODE_NOSTREAM, nullptr, &db, &compile_err);
		
		hs_scratch_t *scratch = nullptr;
		err = hs_alloc_scratch(db, &scratch);

		
		CallBackContext c;
		c.halt = 0;
		
		std::string data = "abc def foobar.gh";
		err = hs_scan(db, data.c_str(), data.size(), 0, scratch, record_cb, (void *)&c);
		std::cout << "err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		hs_free_database(db);
		err = hs_free_scratch(scratch);
	}
};
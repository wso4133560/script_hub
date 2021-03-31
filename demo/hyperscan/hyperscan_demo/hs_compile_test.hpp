#pragma once
#include "Common.hpp"
// 对hs_compile函数的测试用例

class HsCompileTest
{
public:
	void TestModeBlock()
	{
		hs_database_t *db = nullptr;
		hs_compile_error_t *compile_err = nullptr;
		hs_error_t err = hs_compile("foobar", 0, HS_MODE_NOSTREAM, nullptr, &db, &compile_err);
		
		hs_scratch_t *scratch = nullptr;
		err = hs_alloc_scratch(db, &scratch);
		
		int length = 0;
		hs_scan(db, "data", 4, 0, scratch, dummy_cb, &length);
		printf("TestModeBlock scan data = %d\n", length);
		length = 0;
		
		hs_scan(db, "foobar", 6, 0, scratch, dummy_cb, &length);
		printf("TestModeBlock scan foobar = %d\n", length);
		
		// teardown
		err = hs_free_scratch(scratch);
		hs_free_database(db);
	}
	
	void TestModeStream()
	{
		//const char data[] = "barfoobar";
		hs_compile_error_t *compile_err = nullptr;
		hs_database_t *db;

		hs_set_stream_allocator(my_alloc, my_free);
		alloc_called = 0;

		hs_error_t err = hs_compile("foo.*bar", 0, HS_MODE_STREAM, nullptr, &db, &compile_err);

		hs_scratch_t *scratch = nullptr;
		err = hs_alloc_scratch(db, &scratch);

		hs_stream_t *stream = nullptr;

		CallBackContext c;

		err = hs_open_stream(db, 0, &stream);

		char data1[] = "bar";
		err = hs_scan_stream(stream, data1, sizeof(data1), 0, scratch, record_cb, (void *)&c);
		std::cout << "err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		char data2[] = "foo";
		err = hs_scan_stream(stream, data2, sizeof(data2), 0, scratch, record_cb, (void *)&c);
		std::cout << "err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		char data3[] = "bars";
		err = hs_scan_stream(stream, data3, sizeof(data3), 0, scratch, record_cb, (void *)&c);
		std::cout << "err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		char data4[] = "foosssbarss";
		err = hs_scan_stream(stream, data4, sizeof(data4), 0, scratch, record_cb, (void *)&c);
		std::cout << "err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		hs_close_stream(stream, scratch, record_cb, (void *)&c);

		err = hs_free_scratch(scratch);

		hs_free_database(db);
		hs_free_compile_error(compile_err);
		hs_set_allocator(nullptr, nullptr);
	}
	
	// 多个数组同时匹配一个协议的数据，适用于不连续内存
	void TestModeVectored()
	{
		hs_compile_error_t *compile_err = nullptr;
		// build a database
		hs_database_t *db;
		hs_error_t err = hs_compile("foo.*bar", HS_FLAG_DOTALL, HS_MODE_VECTORED, nullptr, &db, &compile_err);

		// alloc some scratch
		hs_scratch_t *scratch = nullptr;
		err = hs_alloc_scratch(db, &scratch);

		const char* data1 = "foo";
		const char* data2 = "   ";
		const char* data3 = "";
		const char* data4 = "bar";
		
		const char *data[] = { data1, data2, data3, data4 };
		unsigned int len[] = { 3, 3, 0, 3 };

		CallBackContext c;
		
		err = hs_scan_vector(db, data, len, 4, 0, scratch, record_cb, &c);
		std::cout << "TestModeVectored err code " << err << ";matches size " << c.matches.size() << std::endl;
		
		// teardown
		err = hs_free_scratch(scratch);
		hs_free_database(db);
	}
};
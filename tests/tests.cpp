﻿#include <microstl.h>

#define TEST_SCOPE(x)
#define REQUIRE(x) {if (!(x)) throw std::runtime_error("Test assertioin failed!"); }

std::filesystem::path findTestFile(std::string fileName)
{
	auto dir = std::filesystem::current_path();
	dir = std::filesystem::absolute(dir);
	while (std::filesystem::exists(dir))
	{
		auto filePath = dir / "testdata" / std::filesystem::u8path(fileName);
		if (std::filesystem::exists(filePath))
			return filePath;

		if (dir.has_parent_path() && dir.parent_path() != dir)
			dir = dir.parent_path();
		else
			throw std::runtime_error("Unable to find parent folder!");
	}

	throw std::runtime_error("Unable to find test file!");
}

int main()
{
	{
		TEST_SCOPE("Parse minimal ASCII STL file and check all results");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("simple_ascii.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.name == "minimal");
		REQUIRE(handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.empty());
		REQUIRE(handler.mesh.facets.size() == 1);
		REQUIRE(handler.mesh.facets[0].n.x == -1);
		REQUIRE(handler.mesh.facets[0].n.y == 0);
		REQUIRE(handler.mesh.facets[0].n.z == 0);
		REQUIRE(handler.mesh.facets[0].v1.x == 0);
		REQUIRE(handler.mesh.facets[0].v1.y == 0);
		REQUIRE(handler.mesh.facets[0].v1.z == 0);
		REQUIRE(handler.mesh.facets[0].v2.x == 0);
		REQUIRE(handler.mesh.facets[0].v2.y == 0);
		REQUIRE(handler.mesh.facets[0].v2.z == 1);
		REQUIRE(handler.mesh.facets[0].v3.x == 0);
		REQUIRE(handler.mesh.facets[0].v3.y == 1);
		REQUIRE(handler.mesh.facets[0].v3.z == 1);
	}

	{
		TEST_SCOPE("Parse ASCII STL file with creative white spaces and check all results");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("crazy_whitespace_ascii.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.name == "min \t imal");
		REQUIRE(handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.empty());
		REQUIRE(handler.mesh.facets.size() == 1);
		REQUIRE(handler.mesh.facets[0].n.x == -1);
		REQUIRE(handler.mesh.facets[0].n.y == 0);
		REQUIRE(handler.mesh.facets[0].n.z == 0);
		REQUIRE(handler.mesh.facets[0].v1.x == 0);
		REQUIRE(handler.mesh.facets[0].v1.y == 0);
		REQUIRE(handler.mesh.facets[0].v1.z == 0);
		REQUIRE(handler.mesh.facets[0].v2.x == 0);
		REQUIRE(handler.mesh.facets[0].v2.y == 0);
		REQUIRE(handler.mesh.facets[0].v2.z == 1);
		REQUIRE(handler.mesh.facets[0].v3.x == 0);
		REQUIRE(handler.mesh.facets[0].v3.y == 1);
		REQUIRE(handler.mesh.facets[0].v3.z == 1);
	}

	{
		TEST_SCOPE("Parse small ASCII STL file and do a simple check on the results");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("half_donut_ascii.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.name == "Half Donut");
		REQUIRE(handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.empty());
		REQUIRE(handler.mesh.facets.size() == 288);
	}

	{
		TEST_SCOPE("Parse binary STL file and do a simple check on the results");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("stencil_binary.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.name.empty());
		REQUIRE(!handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.size() == 80);
		for (size_t i = 0; i < 80; i++)
			REQUIRE(handler.header[i] == 0);
		REQUIRE(handler.mesh.facets.size() == 2330);
	}

	{
		TEST_SCOPE("Parse binary STL file from FreeCAD and do some checks on the results");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("box_freecad_binary.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.name.empty());
		REQUIRE(!handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.size() == 80);
		REQUIRE(handler.mesh.facets.size() == 12);
		REQUIRE(handler.mesh.facets[11].n.x == 0);
		REQUIRE(handler.mesh.facets[11].n.y == 0);
		REQUIRE(handler.mesh.facets[11].n.z == 1);
		REQUIRE(handler.mesh.facets[11].v1.x == 20);
		REQUIRE(handler.mesh.facets[11].v1.y == 0);
		REQUIRE(handler.mesh.facets[11].v1.z == 20);
		REQUIRE(handler.mesh.facets[11].v2.x == 0);
		REQUIRE(handler.mesh.facets[11].v2.y == 0);
		REQUIRE(handler.mesh.facets[11].v2.z == 20);
		REQUIRE(handler.mesh.facets[11].v3.x == 20);
		REQUIRE(handler.mesh.facets[11].v3.y == -20);
		REQUIRE(handler.mesh.facets[11].v3.z == 20);
	}

	{
		TEST_SCOPE("Parse ASCII STL file from MeshLab and do some checks on the results");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("box_meshlab_ascii.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.name == "STL generated by MeshLab");
		REQUIRE(handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.empty());
		REQUIRE(handler.mesh.facets.size() == 12);
		REQUIRE(handler.mesh.facets[11].n.x == 0);
		REQUIRE(handler.mesh.facets[11].n.y == 0);
		REQUIRE(handler.mesh.facets[11].n.z == 1);
		REQUIRE(handler.mesh.facets[11].v1.x == 20);
		REQUIRE(handler.mesh.facets[11].v1.y == 0);
		REQUIRE(handler.mesh.facets[11].v1.z == 20);
		REQUIRE(handler.mesh.facets[11].v2.x == 0);
		REQUIRE(handler.mesh.facets[11].v2.y == 0);
		REQUIRE(handler.mesh.facets[11].v2.z == 20);
		REQUIRE(handler.mesh.facets[11].v3.x == 20);
		REQUIRE(handler.mesh.facets[11].v3.y == -20);
		REQUIRE(handler.mesh.facets[11].v3.z == 20);
	}

	{
		TEST_SCOPE("Test parsing file paths supplied as UTF8 string");
		auto filePath = findTestFile(u8"简化字.stl");
		auto utf8String = filePath.u8string();
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(utf8String.c_str(), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.mesh.facets.size() == 1);
	}

	#ifdef _WIN32 // Wide strings are only common on Windows
	{
		TEST_SCOPE("Test parsing file paths supplied as wide string");
		auto filePath = findTestFile(u8"简化字.stl");
		auto wideString = filePath.wstring();
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(wideString.c_str(), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.mesh.facets.size() == 1);
	}
	#endif

	{
		TEST_SCOPE("Test parsing STL data supplied as memory buffer");
		std::ifstream ifs(findTestFile("simple_ascii.stl"), std::ios::binary | std::ios::ate);
		std::streamsize size = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		std::vector<char> buffer(size);
		ifs.read(buffer.data(), size);
		REQUIRE(size > 0);

		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlBuffer(buffer.data(), buffer.size(), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.mesh.facets.size() == 1);
	}

	{
		TEST_SCOPE("Test parsing STL data supplied as std::istream");
		std::ifstream ifs(findTestFile("simple_ascii.stl"), std::ios::binary);
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlStream(ifs, handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.mesh.facets.size() == 1);
	}

	{
		TEST_SCOPE("Parse STL with sphere and check all vertices");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("sphere_binary.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.mesh.facets.size() == 1360);
		const float radius = 10;
		const float allowedDeviation = 0.00001f;
		for (const auto& f : handler.mesh.facets)
		{
			float length1 = sqrt(f.v1.x * f.v1.x + f.v1.y * f.v1.y + f.v1.z * f.v1.z);
			REQUIRE(fabs(length1 - radius) < allowedDeviation);
			float length2 = sqrt(f.v2.x * f.v2.x + f.v2.y * f.v2.y + f.v2.z * f.v2.z);
			REQUIRE(fabs(length2 - radius) < allowedDeviation);
			float length3 = sqrt(f.v3.x * f.v3.x + f.v3.y * f.v3.y + f.v3.z * f.v3.z);
			REQUIRE(fabs(length3 - radius) < allowedDeviation);
		}
	}

	{
		TEST_SCOPE("Test parsing an empty file and check for correct error");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("empty_file.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::MissingDataError);
		REQUIRE(handler.name.empty());
		REQUIRE(!handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.empty());
		REQUIRE(handler.mesh.facets.empty());
	}

	{
		TEST_SCOPE("Test parsing an non-existing file and check for correct error");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile("does_not_exist.stl", handler);
		REQUIRE(res == handler.result && res == microstl::Result::FileError);
		REQUIRE(handler.name.empty());
		REQUIRE(!handler.ascii);
		REQUIRE(handler.errorLineNumber == 0);
		REQUIRE(handler.header.empty());
		REQUIRE(handler.mesh.facets.empty());
	}

	{
		TEST_SCOPE("Parse STL with cube and deduplicate vertices");
		microstl::MeshParserHandler handler;
		auto res = microstl::Parser::parseStlFile(findTestFile("box_meshlab_ascii.stl"), handler);
		REQUIRE(res == handler.result && res == microstl::Result::Success);
		REQUIRE(handler.mesh.facets.size() == 12);
		microstl::FVMesh deduplicatedMesh = microstl::deduplicateVertices(handler.mesh);
		REQUIRE(deduplicatedMesh.facets.size() == 12);
		REQUIRE(deduplicatedMesh.vertices.size() == 8);
	}

	return 0;
}

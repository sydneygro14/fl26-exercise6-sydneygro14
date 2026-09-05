////////////////////////////////////////////////////////
// ECE 3574 Exercise: Unit Tests with Catch - starter code
//
#define CATCH_CONFIG_MAIN  // This line tells Catch to provide a main() function
                           // - do this in one cpp file only
#include "catch.hpp"       // This line provides access to all Catch macros
                           // - do not modify catch.hpp
#include <cassert>    
#include <cstdlib>

TEST_CASE("multimap unit test1", "[multimap]") {
	std::multimap<std::string, std::string> m;	
	REQUIRE(m.size() == 0);
	
	m.insert(std::pair<std::string, std::string>("Ross", "Betsy"));
	REQUIRE(m.size() == 1);

}

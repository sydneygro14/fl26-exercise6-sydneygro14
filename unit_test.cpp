////////////////////////////////////////////////////////
// ECE 3574 Exercise: Unit Tests with Catch - starter code
//
#define CATCH_CONFIG_MAIN 
                           
#include "catch.hpp"      
                           
#include <map>      
#include <string>   

TEST_CASE("multimap unit test1", "[multimap]") {
	std::multimap<std::string, std::string> m;	
	REQUIRE(m.size() == 0);                       
	
	m.insert(std::pair<std::string, std::string>("Ross", "Betsy")); // insert one key-value pair
	REQUIRE(m.size() == 1);                                          

	// multimap allows duplicate keys, so inserting another "Ross" should grow the size rather than overwrite whats already there
	m.insert(std::pair<std::string, std::string>("Ross", "Jemima")); // insert a 2nd pair with the same key 'ross'
	REQUIRE(m.size() == 2);                                         
	REQUIRE(m.count("Ross") == 2);                                   // tells us how many entries have 'ross'

	m.insert(std::pair<std::string, std::string>("Monica", "Chandler")); // insert a pair with a brand-new key "Monica"
	REQUIRE(m.size() == 3);                                              
	REQUIRE(m.count("Monica") == 1);                                     // "Monica" appears exactly once
	REQUIRE(m.count("Nonexistent") == 0);                                // a key that was never inserted has count 0

	// find() on a key that exists should not return end()
	auto it = m.find("Monica");         
	REQUIRE(it != m.end());             // since "Monica' exists, find() should NOT return the end() iterator
	REQUIRE(it->second == "Chandler");  //  gives us the value stored and checks it matches

	// find() on a key that doesn't exist should return end()
	REQUIRE(m.find("Nobody") == m.end()); 

	// erase by key removes ALL matching entries for that key
	size_t numErased = m.erase("Ross"); 
	REQUIRE(numErased == 2);            // we had two "Ross" entries, so 2 should have been erased
	REQUIRE(m.size() == 1);             // only the "Monica" entry should remain, so size drops to 1
	REQUIRE(m.count("Ross") == 0);      // 'ross' should no longer be present at all

	REQUIRE_FALSE(m.empty()); // empty() should be false (we still have one)

	m.clear();                  // clear() removes remaining entries
	REQUIRE(m.empty());         // empty should be true
	REQUIRE(m.size() == 0);     // and size() should be back down to 0
}

TEST_CASE("multimap operator= performs a deep, independent copy", "[multimap][operator=]") {
	std::multimap<int, std::string> m1;                              
	m1.insert(std::pair<int, std::string>(1, "one"));  // insert key 1 -> "one"
	m1.insert(std::pair<int, std::string>(2, "two"));   // insert key 2 -> "two"
	m1.insert(std::pair<int, std::string>(2, "deux"));  // insert a 2nd pair with key 2 

	std::multimap<int, std::string> m2; // create a second, separate, empty multimap
	REQUIRE(m2.empty());               

	m2 = m1; // copy all of m1's contents into m2

	REQUIRE(m2.size() == m1.size()); //  m2 and m1 should have the same number of elements
	REQUIRE(m2.count(2) == 2);       
	REQUIRE(m2.find(1)->second == "one"); // the value for key 1 in m2 should match what was in m1

	// this checks that operator= makes a real copy, not a reference/alias.
	m1.insert(std::pair<int, std::string>(3, "three")); // add a new entry to m1 only, after copy was made
	REQUIRE(m1.size() == 4);        
	REQUIRE(m2.size() == 3);      
	REQUIRE(m2.find(3) == m2.end()); // key 3 should NOT exist in m2

	m1.clear();               // clear m1 completely
	REQUIRE(m1.empty());     
	REQUIRE_FALSE(m2.empty());    // m2 should still have its 3 entries
}

TEST_CASE("multimap empty/size/clear on default construction and after mutation", "[multimap][empty][size][clear]") {
	std::multimap<double, double> m; // create an empty multimap - double keys, double values
	REQUIRE(m.empty());              
	REQUIRE(m.size() == 0);          

	m.insert(std::pair<double, double>(3.14, 1.0)); // insert one key-value pair
	REQUIRE_FALSE(m.empty());                      
	REQUIRE(m.size() == 1);                         

	m.insert(std::pair<double, double>(2.71, 2.0)); // insert a pair with key 2.71
	m.insert(std::pair<double, double>(2.71, 3.0)); // insert another pair with the same key 2.71 
	REQUIRE(m.size() == 3);                         
	REQUIRE(m.count(2.71) == 2);                    // key 2.71 should appear exactly twice

	m.clear();               // remove everything from the multimap
	REQUIRE(m.empty());      
	REQUIRE(m.size() == 0);  

	// clear() on an already-empty multimap should be fine
	m.clear();               // calling clear() should not cause any error
	REQUIRE(m.empty());      
}

TEST_CASE("multimap insert, count, and find with duplicate keys", "[multimap][insert][count][find]") {
	std::multimap<char, int> m; // create an empty multimap: char keys, int values

	m.insert(std::pair<char, int>('a', 1)); 
	m.insert(std::pair<char, int>('a', 2)); // insert a second pair with the same key 'a' -> 2
	m.insert(std::pair<char, int>('b', 3)); 

	REQUIRE(m.size() == 3);      
	REQUIRE(m.count('a') == 2);  // key 'a' appears twice
	REQUIRE(m.count('b') == 1);  // key 'b' appears once
	REQUIRE(m.count('z') == 0);  // key 'z' was never inserted, so its count is 0

	// find() only ever returns ONE entry, even if the key has duplicates -
	// equal_range() is what you'd use to get all of them.
	auto range = m.equal_range('a'); 
	int sum = 0;                     
	int rangeCount = 0;              
	for (auto it = range.first; it != range.second; ++it) { // loop from the first matching entry up to (not including) the last
		sum += it->second;    
		++rangeCount;          
	}
	REQUIRE(rangeCount == 2); 
	REQUIRE(sum == 3);        

	REQUIRE(m.find('b') != m.end()); // find() should locate the single 'b' entry (not return end())
	REQUIRE(m.find('q') == m.end()); // find() should return end() for a key ('q') that was never inserted
}

TEST_CASE("multimap erase by key, by iterator, and by range", "[multimap][erase]") {
	std::multimap<int, char> m;            
	m.insert(std::pair<int, char>(1, 'a')); 
	m.insert(std::pair<int, char>(2, 'b')); 
	m.insert(std::pair<int, char>(2, 'c')); 
	m.insert(std::pair<int, char>(3, 'd')); 
	REQUIRE(m.size() == 4);                 

	// erase(key) removes every element matching that key and returns the count removed
	size_t erasedCount = m.erase(2); 
	REQUIRE(erasedCount == 2);       
	REQUIRE(m.size() == 2);          
	REQUIRE(m.count(2) == 0);        

	// erase(iterator) removes a single element
	auto it = m.find(1);    
	REQUIRE(it != m.end()); 
	m.erase(it);              
	REQUIRE(m.size() == 1);   
	REQUIRE(m.count(1) == 0); 
	REQUIRE(m.count(3) == 1); 

	// erasing a key that doesn't exist returns 0 and leaves the container unchanged
	size_t noOp = m.erase(999); 
	REQUIRE(noOp == 0);          // erase() should report that 0 elements were removed
	REQUIRE(m.size() == 1);      // the container should be completely unaffected, still size 1
}

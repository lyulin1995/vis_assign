add_executable(assignment_test)

target_sources(assignment_test
PRIVATE
	assignment_test.cc
)

target_link_libraries(assignment_test
PRIVATE
	assignment
	gmock_main
)

gtest_discover_tests(assignment_test)
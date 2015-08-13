
enable_testing()
include(CTest)

set(TESTS
	GA
	incomplete
	nnlayer
	popsize
	speed
	splitinst
)

foreach(test ${TESTS})
	add_executable(test${test} test-${test}.cpp)
	add_dependencies(test${test} pneuro ga)
	target_link_libraries(test${test} pneuro ga)
	add_test(${test} test${test})
endforeach(test ${TESTS})
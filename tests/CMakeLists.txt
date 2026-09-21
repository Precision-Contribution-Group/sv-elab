if (WITH_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        set(test_launcher
            ${CMAKE_COMMAND} -E env
            LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/yosys-slang.%p.profraw
        )

        add_custom_target(
            coverage
            COMMAND
                llvm-profdata merge -sparse
                yosys-slang.*.profraw
                -o yosys-slang.profdata
            COMMAND
                llvm-cov export -format lcov
                $<TARGET_FILE:yosys-slang>
                -instr-profile=yosys-slang.profdata
                --ignore-filename-regex=/third_party/
                --ignore-filename-regex=^/usr
                > yosys-slang.info
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()

set(ALL_TESTS
    unit/async.ys
    unit/dff.ys
    unit/dualedge.ys
    unit/function_call.ys
    unit/expression_attrs.ys
    unit/latch.ys
    unit/selftests.tcl
    various/assign_mixing.ys
    various/assignment_pattern_lhs.ys
    various/bb_detect.ys
    various/blackbox_scenarios.ys
    various/bus_range.ys
    various/concurrent_assert.ys
    various/defaults.ys
    various/delays.ys
    various/dualedge.ys
    various/expr.ys
    various/flop_naming.ys
    various/formal_stmts.ys
    various/hierref_error.ys
    various/huge_reg.ys
    various/ignore_asserts.ys
    various/init.ys
    various/intf_array_naming.ys
    various/intf_w_hierarchy.ys
    various/issue142.ys
    various/issue143.ys
    various/issue208.ys
    various/issue227.ys
    various/issue240.ys
    various/issue266.ys
    various/issue300.ys
    various/issue354.ys
    various/issue372.ys
    various/issue50.ys
    various/mem_inference.ys
    various/meminit.ys
    various/pragmas.ys
    various/readmem.ys
    various/readmem_diag.ys
    various/regress.ys
    various/stringattrs.ys
    various/stringparams.ys
    various/timescale.ys
    various/tranif.ys
    various/top_attr.ys
    various/unimplemented.ys
    various/unknown_cells.ys
    various/toplevel_intf.ys
    various/wait_test.ys
    various/expect_test.ys
    various/program_test.ys
    various/synthesis_define.ys
)

foreach(test_script ${ALL_TESTS})
    set(test_fullpath ${CMAKE_CURRENT_SOURCE_DIR}/${test_script})
    get_filename_component(test_dir ${test_fullpath} DIRECTORY)
    add_test(
        NAME ${test_script}
        WORKING_DIRECTORY ${test_dir}
        COMMAND
            ${test_launcher}
            ${YOSYS_BINDIR}/yosys -m $<TARGET_FILE:yosys-slang> ${test_fullpath}
    )
endforeach()

option(USE_EXTERNAL_Catch2 "Build with external Catch2 library" OFF)
if (USE_EXTERNAL_Catch2)
    find_package(Catch2 3 CONFIG QUIET)
endif()

if (Catch2_FOUND)
    message(STATUS "Using external Catch2")
else()
    message(STATUS "Using bundled third_party/Catch2")

    option(Catch2_INSTALL OFF)
    FetchContent_Declare(
        Catch2
        EXCLUDE_FROM_ALL
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/Catch2
    )
    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(cxx-tests cxx/test_pir_builder.cc)
target_link_libraries(cxx-tests PRIVATE Catch2::Catch2WithMain)

# Enable registration of tests with CTest
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(cxx-tests)

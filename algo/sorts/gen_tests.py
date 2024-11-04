import os, sys
from random import randint


def gen_random_array():
    arr = []
    N = 10000
    for _ in range(int(N)):
        arr.append(randint(0, int(N)//2))
    return arr

arr = gen_random_array()

tests = [
    ["null", [], []],
    ["single", [1], [1]],
    ["presorted", [1, 2, 3, 4], [1, 2, 3, 4]],
    ["reversed_odd", [5,4,3,2,1], [1, 2, 3, 4, 5]],
    ["reserved_even", [4, 3, 2, 1], [1, 2, 3, 4]],
]


def append_test(f, sortname, testname, inp, res):
    f.write(f"TEST({sortname}_sort, {testname})\n")
    f.write("{\n    vector<int> array = {")
    f.write(inp)
    f.write("};\n    vector<int> res =   {")
    f.write(res)
    f.write("};\n")
    f.write(f"    {sort}_sort(array);\n    ASSERT_EQ(array,  res);\n")
    f.write("}\n\n")


with open("src/tests.cpp", 'w') as f:
    f.write(f"// Code generated by {sys.argv[0]} \n")
    f.write("""#include <gtest/gtest.h>
#include <vector>
""")

    f.write("std::vector<int> input = {" + ', '.join(map(str, arr)) + "};\n")
    f.write("std::vector<int> result = {" + ', '.join(map(str, sorted(arr))) + "};\n")
    f.write("""
#define MAKE_TESTS

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
""")

    arr = gen_random_array()

    sorts = "count", "dipers", "heap", "insertion", "merge", "quick", "shell"
    for sort in sorts:
        f.write(f'\n#include "{sort}_sort.cpp"\n\n')

        for test in tests:
            append_test(f, sort, test[0], ', '.join(map(str, test[1])), ', '.join(map(str, test[2])))

        append_test(f, sort, "random", "input", "result")
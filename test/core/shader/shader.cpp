#include <catch2/catch_test_macros.hpp>
#include "../../helpers/golden_file.hpp"
#include "../../../core/shader/shader.hpp"
static int Factorial(int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number; // fail
    // return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE("shader of 1 and higher are computed (pass)", "[shader-file]")
{
    // Shader::buildShader("./test_files/shader.vert");
}
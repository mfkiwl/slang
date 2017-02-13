#include "Catch/catch.hpp"
#include "slang.h"

using namespace slang;

namespace {

TEST_CASE("Simple eval", "[eval") {
    ScriptSession session;
    auto value = session.eval("3 * 3");
    CHECK(value.integer() == 9);

    session.eval("int i = 4;");
    value = session.eval("i + 9");
    CHECK(value.integer() == 13);
}

TEST_CASE("Eval function calls", "[eval]") {
    ScriptSession session;
    session.eval(R"(
function logic [15:0] foo(int a, int b);
    return a + b;
endfunction
)");

    auto value = session.eval("foo(3, 4)");
    CHECK(value.integer() == 7);

    session.eval(R"(
function int bar();
    return 2;
    return 3;
endfunction
)");

    value = session.eval("bar()");
    CHECK(value.integer() == 2);
}

TEST_CASE("Nested functions", "[eval]") {
    ScriptSession session;
    session.eval(R"(
function automatic int symbols_in_data(int dataBitsPerSymbol, int data_width);
    return data_width / dataBitsPerSymbol;
endfunction
)");

    session.eval(R"(
function automatic int num_words_in_address_space(int dataBitsPerSymbol, int data_width, int address_width);
    // Riviera-PRO 2015.10 crashes when calling a function from
    // within a function. After all this is understandable since
    // this is a really hard CS problem that has never been solved
    // before... ???
    //
    int address_bits_per_word = $clog2(symbols_in_data(dataBitsPerSymbol, data_width));
    return 2**(address_width - address_bits_per_word);
endfunction
)");

    auto value = session.eval("num_words_in_address_space(8, 64, 20)");

    auto diagnostics = session.reportDiagnostics();
    if (!diagnostics.empty())
        WARN(diagnostics.c_str());

    CHECK(value.integer() == 131072);
}

TEST_CASE("Module param", "[eval]") {
    ScriptSession session;
    auto module = session.eval("module A#(parameter int P); localparam LP = P + 3; endmodule");
    CHECK(module);
    auto instance = session.eval("A #(.P(2)) a0();");
    CHECK(instance);
    auto value = session.eval("a0.LP");
    CHECK(value.integer() == 5);
}

TEST_CASE("Interface param", "[eval]") {
    ScriptSession session;
    auto interface = session.eval("interface IFACE#(parameter int W = 8); logic valid; logic [W-1:0] data; endinterface");
    CHECK(interface);
    auto instance = session.eval("IFACE #(6) i0();");
    CHECK(instance);
    auto value = session.eval("i0.W");
    CHECK(value.integer() == 6);
}

/*
TEST_CASE("Module interface port parametrization", "[eval]") {
    ScriptSession session;
    auto interface = session.eval("interface IFACE#(parameter int W = 8); logic valid; logic [W-1:0] data; endinterface");
    CHECK(interface);
    auto module = session.eval("module M(IFACE iface); localparam int LP = iface.W; endmodule");
    CHECK(module);
    auto tb = session.eval("module tb; IFACE #(6) i0(); M m0(i0); endmodule");
    CHECK(tb);
    auto instance = session.eval("tb tb();");
    CHECK(instance);
    auto value = session.eval("tb.m0.LP");
    CHECK(value.integer() == 6);
}
*/

}

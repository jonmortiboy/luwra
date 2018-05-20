#include <catch.hpp>
#include <luwra.hpp>

TEST_CASE("Function<R>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) return x + y end") == LUA_OK);

	auto fun = state.read<luwra::Function<int>>(-1);
	REQUIRE(fun(13, 37) == 50);

	luwra::Function<double> fun2 = fun;
	REQUIRE(fun2(37.13, 13.37) == 50.5);
}

TEST_CASE("Function<void>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) returnValue = x + y end") == LUA_OK);

	auto fun = state.read<luwra::Function<void>>(-1);
	fun(13, 37);

	int returnValue = state.get<int>("returnValue");
	REQUIRE(returnValue == 50);
}

TEST_CASE("function<R(A...)>") {
	luwra::StateWrapper state;

	SECTION("read") {
		REQUIRE(state.runString("return function (x, y) return x + y end") == LUA_OK);

		auto fun = state.read<std::function<int (int, int)>>(-1);
		REQUIRE(fun(13, 37) == 50);
	}

	SECTION("push") {
		std::function<int (int, int)> func = [](int a, int b) -> int {
			return a + b;
		};

		state.push(func);
		state.push(13);
		state.push(37);

		lua_call(state, 2, 1);
		REQUIRE(state.read<int>(-1) == 50);
	}
}

TEST_CASE("function<void(A...)>") {
	luwra::StateWrapper state;

	REQUIRE(state.runString("return function (x, y) returnValue = x + y end") == LUA_OK);

	auto fun = state.read<std::function<void (int, int)>>(-1);
	fun(13, 37);

	int returnValue = state.get<int>("returnValue");
	REQUIRE(returnValue == 50);
}

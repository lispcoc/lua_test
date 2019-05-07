#include <iostream>
#include <utility>
#include <vector>
#include <map>

#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0
#include "LuaIntf/LuaIntf.h"
#include "kaguya.hpp"

kaguya::State* state_p = nullptr;

enum enum_test{
    test1,
    test2,
    test3,
    test4,
};

class NumTest
{
  public:
    int a;
    NumTest(int a2)
        : a(a2)
    {
    }
};

class Test
{
  public:
    int a;
    NumTest n = NumTest(0);
    std::vector<int> v = std::vector<int>();

    Test(int a2 = 0)
        : a(a2)
    {
    }
    int get_a(int p0, int p1 = 10)
    {
        printf("%d\n", p0);
        return 10;
    }
    std::string get_b(std::string p0)
    {
        std::cout << p0 << std::endl;
        return "bbb";
    }
    std::string get_enum(enum_test e)
    {
        std::cout << e << std::endl;
        return "ccc";
    }
};

enum_test enum_test_gen(std::string s){
    if(s == "test1") return test1;
    if(s == "test2") return test2;
    if(s == "test3") return test3;
    return test4;
}
namespace kaguya{
template<>  struct lua_type_traits<enum_test> {
	typedef enum_test get_type;
	typedef const enum_test& push_type;

    static bool strictCheckType(lua_State *l, int index) {
        return lua_type_traits<luaInt>::strictCheckType(l, index);
    }
    static bool checkType(lua_State *l, int index) {
        return lua_type_traits<luaInt>::checkType(l, index);
    }
	static get_type get(lua_State* l, int index)
	{
		size_t size = 0;
		const char* buffer = lua_tolstring(l, index, &size);
        if (buffer) {
            std::string s = std::string(buffer, size);
            std::cout << s << std::endl;
            kaguya::State& state = *state_p;
            if(state["enum_test"][s] != kaguya::NilValue()){
                return state["enum_test"][s];
            }
        }
		return static_cast<get_type>(lua_type_traits<luaInt>::get(l, index));
	}
	static int push(lua_State* l, push_type s)
	{
		lua_pushinteger(l, s);
		return 1;
	}
};
}

void lua()
{
    kaguya::State state;
    state_p = &state;
    state["Test"].setClass(kaguya::UserdataMetatable<Test>()
        .setConstructors<Test(),Test(int)>()
        .addOverloadedFunctions("get", &Test::get_a, &Test::get_b)
        .addOverloadedFunctions("get_enum", &Test::get_enum)
        .addProperty("a", &Test::a)
        .addProperty("v", &Test::v)
        );
    
    auto t = Test();
    t.v = std::vector<int>{1,2,3};
    state.dofile("test.lua");
    state["testfunc"](&t);
    printf("%d %d %d\n", t.v[0], t.v[1], t.v[2]);
    std::cout << t.a << std::endl;
#if 0
    //lua_State *L = luaL_newstate();
    //luaL_openlibs(L);
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::math);
    lua_State *L = lua.lua_state();
    LuaIntf::LuaRef func(L, "func");
    LuaIntf::LuaBinding(L)
        .beginClass<Test>("Test")
        .addVariableRef("a", &Test::a)
        .addFunction("get", &Test::get_a, LUA_ARGS(int))
        .addFunction("get", &Test::get_b, LUA_ARGS(std::string))
        .endClass();

    lua.script_file("test.lua");
    LuaIntf::LuaRef testfunc(L, "testfunc");
    auto t = Test(0);
    testfunc(&t);
#endif
}

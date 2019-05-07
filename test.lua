enum_test = {
    test1 = 0,
    test2 = 1,
    test3 = 2,
    test4 = 3,
}

function testfunc(t)
    t:get_enum("test1")
    return 1
end

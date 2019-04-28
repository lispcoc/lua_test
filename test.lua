function testfunc(t)
    print(t.a)
    t.a = 22
    print(t.a)
    print(#t.v)
    t2 = Test(1)
    print(t2.a)
    t3 = Test()
    print(t3.a)
    return 1
end

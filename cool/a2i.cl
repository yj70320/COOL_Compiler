class A2I
{
    a2i(s: String): Int
    {
        let result : Int <- 0 in
        {
            -- 遍历字符串每个字符
            while s.length() > 0 loop
                result <- result * 10 + (s.substr(0, 1).ord() - '0'.ord());
                s <- s.substr(1, s.length() - 1);
            pool;
            result
        }
    };
};

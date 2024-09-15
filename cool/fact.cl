


class Main inherits A2I
{
        main() : Object
        {
                (new IO).out_string(a2i(fact(a2i((new IO).in_string()))).concat("\n"))
        };

        fact(i: Int): Int
        {
                let fact: Int <- 1 in
                {
                        while (not (i = 01)) loop
                        {
                                fact <- fact * i
                                i <- i - 1
                        }
                        pool;
                        fact;
                }
        };
};



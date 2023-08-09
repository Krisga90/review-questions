#include <iostream>
#include <functional>
#include <vector>

//when we want to use Cfile compile
// extern "C"{
//     #include "cgile.h"
// }



//wykrywa jaki typ ma suma typow
template<typename T1, typename T2>
auto add(T1 a, T2 b) ->decltype(a+b)
{
    return a+b;
}


//differeneces betwen pointer and references
void diff()
{
    int i = 10;
    int &r = i;
    int *p =&i;

    std::cout << &i <<'\t' << &r <<  "\tp " << p << "\t&p " << &p << std::endl;

    int var[] = {100, 200};
    std::cout << "i = " << i << std::endl;
    r = var[0];    //we change value of i to  100;
    std::cout << "i = " << i << std::endl;
    
    p = &var[1];  //p stores new address, but has old adress;
    std::cout << "i = " << i << std::endl;
    std::cout << &i <<'\t' << &r <<  "\tp " << p << "\t&p " << &p << std::endl;


    //reference can't be null
    // r = nullptr;
    p= nullptr;

    p =&var[0];
    std::cout << *(p) << " " << *(p+1) << std::endl;
}

void dynamicCast()
{
    class A
    {
        public:
            A(char c):m(10){}
            virtual void print()const {std::cout <<"A: m = " << m << std::endl;}
            int value(int i) const {return m;}
        private:
            int m;
    };

    class B: public A
    {
        public:
            B():A('a'), n(20){}
            void print()const override{std::cout << std::endl;A::print(); std::cout << "B: n = " << n << std::endl;} 
        private:
            int n;
    };

    class C: public A{
        public:
            explicit C(char ch):A('a'), c(ch){}
            void print()const override{std::cout << std::endl; A::print(); std::cout << "C: c = " << c << std::endl;}
            char value(char ch) const {return c;};
        private:
            char c;

    };

    A a = '1';  //implicit

    B b;
    // C c1 = 'c'; //forbidden because explicit
    C c('c');
    A *pa =  &a;
    A *pb = &b;
    A &rc = c;

    pa->print();
    pb->print();
    rc.print();

    auto new_b = dynamic_cast<B*>(pb);
    new_b->print();
    auto wrong_c = dynamic_cast<C*>(pb);
    if(wrong_c == nullptr)
    {
        std::cout << "can't cast\n\n";
    }

    auto new_rc = dynamic_cast<C&>(rc);
    new_rc.print();

    try
    {
        auto bad_b = dynamic_cast<B&>(rc);
    }
    catch(const std::bad_cast& e)
    {
        std::cerr << e.what() << '\n';
    }


    //function hiding
    std::cout << a.value(1) << " " << b.value(1) << " " << c.value('c') << " " << A(c).value('c') << " " << c.A::value(1) << std::endl; 
}

//passing function example
namespace function_passing
{

    class Functor
    {
    public:
        Functor(int a = 1):m_add_value{a}{};
        int operator()(int a, double b, std::vector<int>& c)const
        {
            std::cout <<"functor: " << a << ", " << b << ", vector: ";
            for (auto val : c)
            {
                std::cout << val << " ";
            }
            std::cout << std::endl;
            return 5;
        }

        private:
            int m_add_value;
    };

    typedef int(*ArrFun)(int, double, std::vector<int>&);
    void takeFunction1(const std::function<int(int,double,std::vector<int>&)>& fun)
    {
        std::vector<int> a {1,2,3,4};
        fun(1, 1.1, a);
    }

    void takeFunction2(int(*fun)(int, double, std::vector<int>&))
    {
        std::vector<int> a {1,2,3,4};
        fun(1, 1.1, a);
    }

    int func1(int a, double b, std::vector<int> &c)
    {
        std::cout <<"func1: " << a << ", " << b << ", vector: ";
        for (auto val : c)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        return 5;
    }



    void passingFunctionAsAnArgument()
    {

        takeFunction1(func1);
        takeFunction2(func1);

        auto lambda = [](int a, double b, std::vector<int> &c)
        {std::cout <<"lambda: " <<  a << ", " << b << ", vector: ";
        for (auto val : c)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        return 10;
        };

        takeFunction1(lambda);
        takeFunction2(lambda);
        Functor functor;
        auto functor_as_function = [&functor](int a, double b, std::vector<int>& c) {
        return functor(a, b, c);
    };
        takeFunction1(functor_as_function);

        ArrFun fun_vec[2] = {func1, lambda,};
        for(auto fun: fun_vec)
        {
            takeFunction1(fun);
        }

    }
}


//static variable in class
void staticVariable()
{
    class A
    {
    public:
        A(){};
        void print(){static int x = 0; x++; std::cout << x <<" times\n";}

    };

    A a;
    for(int i = 0; i<10; i++)
        a.print();
}


int main(int argc, char** argv)
{
    // std::cout << add(1, 1.1) << std::endl;
    // diff();
    // dynamicCast();
    // function_passing::passingFunctionAsAnArgument();
    staticVariable();
    return 0;
}
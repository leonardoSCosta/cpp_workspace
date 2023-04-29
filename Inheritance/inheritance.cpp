#include <iostream>
#include <stdio.h>

class A
{
protected:
    float moo = 5.2;
    int bar()
    {
        return 1 + 1;
    }
};

class B : public A
{
public:
    int foo()
    {
        return this->bar();
    }
};

// int main()
// {
//     B obj;
//     std::cout << obj.foo();
//     return 0;
// }


int main()
{
    char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    /*
     * Um ponteiro comum é indicado pelo asterisco (*).
     *
     * Nesta declaração está sendo criado um ponteiro do tipo char e o endereço
     * que este ponteiro irá apontar é o endereço da variável abc.
    */
    char* ponteiro = abc;

    std::cout << "Tamanho da string: " << sizeof(abc) <<
                 " Tamanho do ponteiro: " << sizeof(ponteiro) << " Bytes"
                 << std::endl;

    std::cout << *(ponteiro+1) << std::endl;

    return 0;
}

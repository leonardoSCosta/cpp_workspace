#include <iostream>

class Exemplo
{
private:
    int umAtributoPrivado;

    void umMetodoPrivado()
    {
        std::cout << "O metodo privado foi utilizado pelo setUmAtributoPrivado(...)\n";
    }
public:
    Exemplo()
    {
        umAtributoPrivado = 5;
        umAtributoPublico = 0;
    }
    ~Exemplo()
    {
        std::cout << "O objeto foi deletado\n";
    }

    int getUmAtributoPrivado() const
    {
        return umAtributoPrivado;
    }

    void setUmAtributoPrivado(int _valor)
    {
        umAtributoPrivado = _valor;
        umMetodoPrivado(); // Esse método só pode ser chamado de dentro da classe
    }

    int umAtributoPublico;
};

int main()
{
    Exemplo umObjetoDaClasse;
    // Também é possível fazer assim
    // Exemplo umObjetoDaClasse = Exemplo();
    // Valor do atributo antes de atribuir, ou seja, definido pelo construtor
    std::cout << "O valor do atributo privado eh " << umObjetoDaClasse.getUmAtributoPrivado() << std::endl;
    // Valor do atributo público
    std::cout << "O valor do atributo publico eh " << umObjetoDaClasse.umAtributoPublico << std::endl;

    std::cout << "Chamando o setter\n";
    umObjetoDaClasse.setUmAtributoPrivado(10);
    umObjetoDaClasse.umAtributoPublico = 2;
    // std::endl pula uma linha no texto que será mostrado. É a mesma coisa que
    // colocar um \n no fim da string
    std::cout << "O valor do atributo privado eh " << umObjetoDaClasse.getUmAtributoPrivado() << std::endl;
    // Valor do atributo público
    std::cout << "O valor do atributo publico eh " << umObjetoDaClasse.umAtributoPublico << std::endl;

    // Tudo isso irá gerar erros de compilação
    // Não é possível acessar diretamente (através do '.') o atributo/método privado
    //
    // umObjetoDaClasse.umAtributoPrivado = 15;
    // std::cout << "O valor do atributo privado eh " << umObjetoDaClasse.umAtributoPrivado << std::endl;
    // umObjetoDaClasse.umMetodoPrivado();
    std::cout << "Fim do codigo\n";
    return 0;
}

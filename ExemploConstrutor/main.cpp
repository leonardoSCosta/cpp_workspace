#include <iostream>
#include <string>
#include <vector>

class Contato {
public:
  std::string nome;
  int telefone;
  Contato() {}

  ~Contato() {}
};

class Agenda {
  std::vector<Contato> listaContatos;

public:
  Agenda() {}

  ~Agenda() {}

  void adicionaContato(Contato _novoContato) {
    listaContatos.push_back(_novoContato);
  }

  void removeContato(int _i) {
    listaContatos.erase(listaContatos.begin() + _i);
  }
};

void fazAlgumaCoisa(); // Protótipo da função

int main() {
  std::cout << "Iniciando teste...\n";
  fazAlgumaCoisa();
  do {
    Contato a; // Construtor com argumentos
    std::cout << "Meio do teste...\n";
    std::cout << "Valor var = " << a.nome << "\n";
  } while (false);
  std::cout << "Fim do teste...\n";
}

void fazAlgumaCoisa() { std::cout << "Fazendo alguma coisa...\n"; }

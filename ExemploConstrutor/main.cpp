#include <iostream>
#include <string>
#include <vector>

class Contato {
public:
  std::string nome;
  int telefone;
  int *intList;

  Contato() {
    std::cout << "Construtor do contato\n";
    intList = new int[50];
    intList[10] = 20;
  }

  ~Contato() {
    std::cout << "Destrutor do contato\n";
    std::cout << intList[10] << "\n";
    delete[] intList;
    // salvarContato();
  }
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
  while(true){
    Contato contato; // Chama o construtor
  }

  // fazAlgumaCoisa();
  // apagar o contato -> chamar o destrutor
}

void fazAlgumaCoisa() { std::cout << "Fazendo alguma coisa...\n"; }

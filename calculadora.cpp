//_______________________________Bibliotecas_______________________________
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

// 1 linguagem syntax
// 2 lógica
// tipo, nome (paremetros)

//__________________________FUNÇÃO Achar Operador de Prioridade( * ou / )

int Acha_Operacao(std::string exp, char op) // parametros
{

  for (int i = 0; i < exp.size(); ++i) {

    if (exp[i] == op && i > 0) // exp[i] == 'x' or exp[i] == '/'
    {
      std::cout << "\nInd Op_ref:\t" << i << "\t=> "<< exp[i]<< "\n" ;
      return i;
    }
  }
  return -1;// NAO ENCONTROU OPERACAO
}

//_______________________FUNÇÃO Acha operador Esquerda ____________________

int Acha_Ind_Esquerda(std::string exp, int n) {
  for (int i = n; i > -1; --i) {
    if (exp[i] == '-' or exp[i] == '+')
    {
      int op_esq = i;
//       std::cout << "\nIndice do Operador Esquerdo [y] = " << op_esq << "\n\n";
      return i;
    }
  }
  std::cout<<"\nNAO TEM OPERADOR NA ESQUERDA...\n";
  return -1; // // NAO ENCONTROU OPERACAO
}
//_______________________FUNÇÃO Acha valor da Esquerda ____________________
// Acha valor Esquerda
int Valor_Esquerda(std::string exp, int n, int y) // parametros
{
  int dig = n - y - 1; //
  int valor = 0;

  for (int i = y + 1; i < n; ++i)
  {
    if (exp[i] != n && i > -1) {
      // std::cout  << exp[i];
      valor += (exp[i] - '0') * pow(10, dig - 1); // Subtrair o caracter pelo '0' da tabela ask

      //+= => valor = valor + valorX
      dig--;
    }
  }


  //std::cout<<"INDICE FINAL:"<<dig<<"\n";
  std::cout<<"\nvalor esquerda: "<<valor<<"\n";
//   std::cout << "\nValor Esquerdo = " << valor;
  return valor;
}
//_______________________FUNÇÃO Acha operador Direita ____________________

int Acha_Ind_Direita(std::string exp, int n) {
      std::cout << "\nN  = " << n << "\n\n";

  for (int i = n; i < exp.size(); ++i) {
    std::cout << i << " \t";
    if (exp[i] == '-'  or exp[i] == '+')
     {
      // std::cout << "\nIndice do Operador Direita [z] = " << i << "\n\n";
      return i;
    }

  }
  std::cout<<"\nNAO TEM OPERADOR NA DIREITA...\nexp.size: "<<exp.size();
  return exp.size(); // era -1
}
//_______________________FUNÇÃO Acha valor da Direita ____________________

int Valor_Direita(std::string exp, int n, int z) // parametros
{
  std::cout<<"z:\t"<<z<<"\n";
  int dig = z - n - 1; //
  std::cout<<"dig:\t"<<dig<<"\n";
  int valor = 0;

  for (int i = n + 1; i < z; ++i)
  {
    if ( i != n  && i > n)
    {
      valor += (exp[i] - '0') * pow(10, dig - 1);
      //+= => valor = valor + valorX
      dig--;
    }
  }

std::cout<<"valor direita:  "<<valor<<"\n";

return valor;
}

//_______________________FUNÇÃO Calcular Prioridade _____________________

int calc(std::string exp, char op, int w, int d) // parametros
{
  int resultado = 0;
//   std::cout << "\nCalcular: ";

  if (op == 'x') {
//     std::cout << w << "x" << d << " = ?";
    resultado = w * d;
  } else if (op == ':') {
//     std::cout << w << ":" << d << " = ?";
    resultado = w / d;
  } else if (op == '+') {
//     std::cout << w << "+" << d << " = ?";
    resultado = w + d;
  } else {
//     std::cout << w << "-" << d << " = ?";
    resultado = w - d;
  }

  // std::cout << " = ?";
//   std::cout << "\nResultado parcial = " << resultado;
  return resultado;
}

//__________________________ FUNÇÃO String Auxiliar _______________________

int str_aux(std::string &exp, int n, int y, int z, int f)
{
  std::string aux;

  for(int i = 0; i <= y; ++i)
  {
      aux += exp[i];
  }
  aux.append(std::to_string(f)); // Pegando resultado de calc e jogando dentro da string aux
  for(int i = z; i < exp.size(); ++i)
  {
      aux += exp[i];
  }
  exp = aux;
  return 0;
}
//________________________________FUNÇÃO main()____________________________
int main()
{
  std::cout<<"\nInicio do programa...\n";
  std::string expressao = "23+65-10x125-90+2x8";

  int n = 0; // Índice da operação atual a ser resolvida
  bool multOk = false,  // -> Indica que é para procurar multiplicacoes para resolver
        divOk = false;  // -> Indica que é para procurar divisoes para resolver
  char currOp = 'x';    // Operação atual
  int i = 0;
  // Enquanto existir uma operação válida, executa os passos seguintes
  while(n != -1)
  {
    std::cout <<"\nExpressão:\t"<< expressao << '\n';
      if(!multOk) // Resolve as multiplicacoes
      {
        // PROCURAR uma MULTIPLICAÇÃO
        n = Acha_Operacao(expressao, 'x');

        if(n == -1) // Se n == -1, nao tem mais multiplicacoes para resolver
        {
          multOk = true; // Faz com que não entre mais no if da linha 161, assim
                         // não procura mais multiplicacoes
        }
      }
      if(multOk && !divOk) // Resolve as divisoes
      {
        // PROCURAR uma DIVISÃO
        n = Acha_Operacao(expressao, ':');
        if(n == -1) // Se n == -1, nao tem mais divisoes para resolver
        {
            divOk = true;
        }
      }
      if(divOk && multOk) // Resolve soma e subtração da esquerda pra direita
      {
        int indSub = Acha_Operacao(expressao, '-');//PROCURA indice da SUBTRAÇÃO
        std::cout<<"IndSub ANTES:\t"<<indSub<<"\n";
        n = Acha_Operacao(expressao, '+');//PROCURA indice da SOMA

        // Verifica qual operação está mais à esquerda
        if((indSub < n && indSub != -1) || n == -1 )
        {
          n = indSub;
          std::cout<<"IndSub DEPOIS:\t"<<indSub<<"\n";
        }
      }
      std::cout<< "n: "<< n;
      // Se existe uma operação válida para resolver, faz a conta
      if(n != -1)
      {
          currOp = expressao[n]; // Operação atual para resolver: [x, :, +, -]

          int indEsq = Acha_Ind_Esquerda(expressao, n - 1),//ajuste de referencia dentro do parametro
              indDir = Acha_Ind_Direita(expressao, n + 1),
              valEsq = Valor_Esquerda(expressao, n, indEsq),
              valDir = Valor_Direita(expressao, n, indDir);
              if(currOp == '+'|| currOp == '-')
            {
                  if( expressao[indEsq] == '-')
              {
                valEsq = valEsq * -1;
                //std::cout << "Expressao antes: " << expressao << '\n';

                expressao.erase(indEsq, 1);// .erase precisa que indique o indice de onde comeca a apagar e quantos itens serao apagados
                indDir--;
                indEsq--;
                //como o '-'foi apagado , a string diminuiu...Portanto precisa ajustar os indicer dos operadores
                //std::cout << "Expressao depois " << expressao << '\n';

              }
            }


          int val = calc(expressao, currOp, valEsq, valDir);
          std::cout << "\nFaz a conta...  "
                    << valEsq << " " << currOp <<" " << valDir << " = " << val <<"\n\n";



          str_aux(expressao, n, indEsq, indDir, val);
          std::cout << "Expressao nova: " << expressao << "\n\n_________________//_________________\n\n";
      }
  }

}

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <random>
#include <chrono>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cfloat>
#include<bits/stdc++.h>
#include <locale>

using namespace std;

string retornaConteudoArquivo(string _nomeArquivo)
{
   std::ifstream leitura(_nomeArquivo.c_str()); //abre o arquivo
   string linhas = "";

   if(leitura)
   {
	  while(leitura.good()) //enquanto nao houver erro na leitura, como badbit ou failbit
	  {
         string temp;
         getline (leitura, temp); //le linha por linha
         temp += "\n"; //colocar o final de linha
	     linhas += temp;
	  }
   }
   else
      linhas = "Arquivo não existe!!";
   leitura.close(); //fecha o arquivo
   return linhas;
}

int main(int argc, char *argv[])
{


    const char *filename = argv[1];


    std::ofstream leitura(filename, std::ios_base::out);
    string linhas = "";

    if(leitura)
    {
        while(true) //enquanto nao houver erro na leitura, como badbit ou failbit
        {
           leitura << "xpto";
           int i=rand()%5;
           leitura << ":i=" << i << endl;
           sleep(i);

        }
    }
    else
    {
        cout << "Erro ao abrir:" << filename << endl;
    }
    return 0;

}

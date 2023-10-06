#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;

struct Instruction
{
    const string opcode;
    int cont;
};

float performance(string newfile, float tclock)
{
    ifstream inputFile(newfile);
    string line;
    int i = 0;

    while (std::getline(inputFile, line))
    {
        ++i;
    }
    float cpi = (5.0 + 1 * (i - 1)) / static_cast<float>(i);
    float clock = tclock;
    float tcpu = float(i * cpi * clock);

    return tcpu;
}

int main()
{
    setlocale(LC_ALL, "Portuguese");
    ifstream inputFile("doc.txt");

    if (!inputFile)
    {
        cerr << "\nNão existe nenhum arquivo com o nome de "
                "doc.txt"
                " no diretório do projeto.";
        return 0;
    }

    float clock = 0;
    string line;
    int instruction_cont = 0;
    int method;

    Instruction U = {"0110111", 0};
    Instruction J = {"1101111", 0};
    Instruction B = {"1100011", 0};
    Instruction L = {"0000011", 0};
    Instruction S = {"0100011", 0};
    Instruction I = {"0010011", 0};
    Instruction R = {"0110011", 0};

    cout << "\nMÉTODOS PIPELINE";
    cout << "\nCertifique-se de substituir o conteúdo do arquivo doc.txt no diretório pelo seu dump file!";

    cout << "\n\nQual o tempo de clock? Em nanosegundos\n";
    cin >> clock;

    cout << "\n\nQual técnica você deseja utilizar?\n1- Inserção de NOP\n2-Forwarding\n";
    cin >> method;

    switch (method)
    {
    case 1:
        cout << performance("doc.txt", clock);
        break;
    case 2:
        cout << "2";
        break;
    default:
        break;
    }
}
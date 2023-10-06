#include <iostream>
#include <fstream>
#include <locale.h>

using namespace std;

const string nopCode = "00000000000000000000000000010011";

const string U = "0110111";
const string J = "1101111";
const string B = "1100011";
const string L = "0000011";
const string S = "0100011";
const string I = "0010011";
const string R = "0110011";

float performance(string newfile, float tclock) {
    ifstream inputFile(newfile);
    string line;
    int i = 0;

    while (getline(inputFile, line)) {
        ++i;
    }
    float cpi = (5.0 + 1 * (i - 1)) / static_cast<float>(i);
    float clock = tclock;
    float tcpu = float(i * cpi * clock);

    return tcpu;
}

void nop(string file) {
    ifstream inputFile(file);
    ofstream outputFile("nop.txt");
    string line;
    string nextLine1, nextLine2;

    if (getline(inputFile, nextLine1) && getline(inputFile, nextLine2)) {
        do {
            line = nextLine1;
            nextLine1 = nextLine2;
            if (!getline(inputFile, nextLine2)) {
                nextLine2.clear();
            }
            outputFile << line << "\n";

            int length = line.length();
            for (int i = 0; i < length;) {
                if (i + 32 <= length) {
                    string segment = line.substr(i, 32);
                    string last_seven = segment.substr(25, 7);

                    if (last_seven == B || last_seven == S) {
                        i += 32;
                    } else {
                        bool addedNop = false;
                        string rd = segment.substr(20, 5);

                        if (!nextLine1.empty() && (nextLine1.substr(12, 5) == rd || nextLine1.substr(7, 5) == rd)) {
                            outputFile << nopCode << "\n";
                            outputFile << nopCode << "\n";
                            addedNop = true;
                        }
                        if (!addedNop && !nextLine2.empty() &&
                            (nextLine2.substr(12, 5) == rd || nextLine2.substr(7, 5) == rd)) {
                            outputFile << nopCode << "\n";
                        }

                        i += 32;
                    }
                } else {
                    break;
                }
            }
        } while (!nextLine1.empty());
    }

    inputFile.close();
    outputFile.close();
}

void forwarding(string file) {
    ifstream inputFile(file);
    ofstream outputFile("forwarding.txt");
    string line;
    string nextLine1, nextLine2;

    if (getline(inputFile, nextLine1) && getline(inputFile, nextLine2)) {
        do {
            line = nextLine1;
            nextLine1 = nextLine2;
            if (!getline(inputFile, nextLine2)) {
                nextLine2.clear();
            }
            outputFile << line << "\n";

            int length = line.length();
            for (int i = 0; i < length;) {
                if (i + 32 <= length) {
                    string segment = line.substr(i, 32);
                    string last_seven = segment.substr(25, 7);

                    if (last_seven == L) {
                        string rd = segment.substr(20, 5);
                        bool addedNop = false;

                        if (!nextLine1.empty() && (nextLine1.substr(12, 5) == rd || nextLine1.substr(7, 5) == rd)) {
                            outputFile << nopCode << "\n";
                            outputFile << nopCode << "\n";
                            addedNop = true;
                        }
                        if (!addedNop && !nextLine2.empty() &&
                            (nextLine2.substr(12, 5) == rd || nextLine2.substr(7, 5) == rd)) {
                            outputFile << nopCode << "\n";
                        }

                        i += 32;
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
        } while (!nextLine1.empty());
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    ifstream inputFile("doc.txt");

    if (!inputFile) {
        cerr << "\nNão existe nenhum arquivo com o nome de "
                "doc.txt"
                " no diretório do projeto.";
        return 0;
    }

    float clock = 0;
    int method;

    cout << "\nMÉTODOS PIPELINE";

    cout << "\nCertifique-se de substituir o conteúdo do arquivo doc.txt no diretório pelo seu dump file!";
    cout << "\n\nQual o tempo de clock? Em nanosegundos\n";
    cin >> clock;

    cout
            << "\n\nQual técnica você deseja utilizar?\n1- Cálculo de desempenho | Sem alterações\n2- Inserção de NOP\n3- Forwarding\n0- Sair\n";
    cin >> method;

    switch (method) {
        case 1:
            cout << endl << "O desempenho foi de "
                 << performance("doc.txt", clock) << " nanosegundos.";
            break;
        case 2:
            nop("doc.txt");
            cout << endl << "O desempenho foi de "
                 << performance("nop.txt", clock) << " nanosegundos.";
            break;
        case 3:
            forwarding("doc.txt");
            cout << endl << "O desempenho foi de "
                 << performance("forwarding.txt", clock) << " nanosegundos.";
            break;
        default:
            break;
    }

    return 0;
}
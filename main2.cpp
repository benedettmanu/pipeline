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
                        string rd = segment.substr(20, 5);

                        if (!nextLine1.empty() && (nextLine1.substr(12, 5) == rd || nextLine1.substr(7, 5) == rd)) {
                            outputFile << nopCode << "\n";
                            outputFile << nopCode << "\n";
                        }
                        if (!nextLine2.empty() && (nextLine2.substr(12, 5) == rd || nextLine2.substr(7, 5) == rd)) {
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

// função que verifica se o opcode é "jal" ou representa um laço de repetição
bool isJalOrLoop(string instruction) {
    string opcode = instruction.substr(0, 7); // Extrai os 7 bits do opcode

    // verifica se o opcode é igual a "jal" (opcode 1101111) ou se representa um laço de repetição
    return (opcode == J) || (opcode == B && instruction[25] == '1');
}

// FUNÇÕES NOVAS: EXERCICIO 3  -----------------------------------------------------------
void reorderInstructions(const string &inputFile, const string &outputFile) {
    string line;
    ifstream input(inputFile);
    ofstream output(outputFile);

    while (getline(input, line)) {
        if (isJalOrLoop(line)) {
            // Se a linha contiver "jal" ou representar um laço, mantenha-a intacta
            output << line << endl;
        } else {
            // substituindo NOPs por instruções "ADDI R1, R2, 0"
            if (line == nopCode) {
                output << "00110010000010000000000000000000" << endl; // Substituir NOP por ADDI R1, R2, 0
            }
        }
    }
    input.close();
    output.close();
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

    cout << "\n\nQual técnica você deseja utilizar?\n1- Inserção de NOP\n2-Forwarding\n3- Reordenar instruções com Forwarding\n";
    cin >> method;

    switch (method) {
        case 1:
            nop("doc.txt");
            cout << endl
                 << performance("nop.txt", clock);
            break;
        case 2:
            forwarding("doc.txt");
            cout << endl
                 << performance("forwarding.txt", clock);
            break;
        case 3:
            reorderInstructions("forwarding.txt", "reordered_forwarding.txt");
            cout << endl << performance("reordered_forwarding.txt", clock);
        default:
            break;
    }
}

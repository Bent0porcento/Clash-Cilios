#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include <locale.h>
#include <cctype>

using json = nlohmann::json;
// Paricido com std, serve pra não ficar escrevendo nlohmann::json toda vez quer quiser usar o tipo json

json CarregarCartas()
{
    std::ifstream arquivo_json("cartas.json"); // abrir o json

    if (!arquivo_json.is_open()) // verificar se deu certo a abertura
    {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo cartas.json" << std::endl;
        return json();
    }

    json dados_cartas; // parsear as cartas
    try
    {
        arquivo_json >> dados_cartas; // le os dados pra colocar na nossa variavel dados_cartas
    }
    catch (json::parse_error &e) // verificacao para erro na hora do parse
    {
        std::cerr << "Erro de parse no JSON: " << e.what() << std::endl;
        return json();
    }
    return dados_cartas; // retona pra gente o dodos_cartas, equivalente a lista de cartas
}

void ImprimirCartas(const json &listadecartas)
// exige que passamos um json para poder ler, no nosso caso é o json com as cartas
{
    std::cout << "--- Catalogo de Cartas do Clash Royale ---" << std::endl
              << std::endl;

    for (const auto &carta : listadecartas) // imprime as carta de acordo com as infomarções que ele puxa do json
    {
        int id = carta["id"]; // guarda cada informação das cartas em uma variavel para imprimir dps
        std::string nome = carta["nome"];
        // talvez daria pra imprimir direto sem colocar em uma variavel mas n tenho certeza
        std::string raridade = carta["raridade"];
        std::string tipo = carta["tipo"];

        std::cout << "ID: " << id << std::endl;
        std::cout << "Nome: " << nome << std::endl; // impressões
        std::cout << "Raridade: " << raridade << std::endl;
        std::cout << "Tipo: " << tipo << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

json ProcurarCarta(const json &listadecartas, std::string n)
// exige a lista de carta e uma string, essa string é o nome da carta que a gente quer
{
    for (const auto &c : listadecartas) // percorre todo o array
    {
        if (c["nome"] == n)
        {
            return c; // quando acha a carta que tem o nome igual, retorna ela saindo do looping
        }
    }
    std::cout << "Impossivel Encontrar sua carta" << std::endl;
    return json(); // se nao achar a carta retorna um jason vazio
}

void ImprimirCarta(const json &carta) // pede somente uma carta e imprime suas informações
{
    std::cout << "ID: " << carta["id"] << std::endl;
    std::cout << "Nome: " << carta["nome"] << std::endl;
    std::cout << "Elixir: " << carta["elixir"] << std::endl;
    std::cout << "Raridade: " << carta["raridade"] << std::endl;
    std::cout << "Tipo: " << carta["tipo"] << std::endl;
}

// 1- Definir Estrutura

int TamanhoDeck = 8;

struct Deck
{
    json CartasDoDeck[8];
    double peso;
};

Deck listaDeck[10];
std::string l;
bool sinal;
int final = -1;
int aux;
char conf;
int contDeck = 0;

void ImprimirDeck(Deck &deck) // pede um deck e imprime as informações dele
{
    for (int i = 0; i < final + 1; i++)
    {
        std::cout << "Carta " << i + 1 << ": " << deck.CartasDoDeck[i]["nome"] << std::endl;
    }
    std::cout << "Peso: " << deck.peso << std::endl;
}

void CalculaPeso(Deck &deck) // pede um deck e calcula o peso dele
{
    int elixirT = 0;
    double peso;
    for (int i = 0; i < 8; i++)
    {
        elixirT += deck.CartasDoDeck[i]["elixir"].get<int>();
        // soma o elixir de cada carta acessando elas pelo array dentro do struct
    } // o programa nn consegue supor que o elexir é do tipo int,
    peso = elixirT / 8.0;
    // mesmo que no json ele esteja como int, por isso que tem q ter um cast pra int com o get<int>
    deck.peso = (std::round(peso * 10.0)) / 10.0;
    // faz com que fique com uma casa decimal, multiplicando por 10 primeiro e arredondando, depois divide pra voltar
}

Deck CriarDeck(const json &listadecarta) // passa a lista de carta e começa o processo pra criar o deck
{
    Deck novodeck;
    std::string nome;
    std::cout << "========== Comecando Montagem de Deck ==========" << std::endl;
    for (int i = 0; i < TamanhoDeck; i++)
    {
        std::cout << "Informe o nome da carta " << i + 1 << " do seu novo deck: " << std::endl;
        std::getline(std::cin, nome); // pega a linha toda e não só a proxima palavra
        for (char &c : nome)          // for each passando por todas as letras do nome passado pra deixar elas em minusculo
        {
            c = tolower(c);
        }
        novodeck.CartasDoDeck[i] = ProcurarCarta(listadecarta, nome);
        // usa o método de procurar carta pra armazenar no array de cartas do deck
        if (novodeck.CartasDoDeck[i] == json()) // verificação caso passe uma json nulo
        {
            while (novodeck.CartasDoDeck[i] == json()) // repete o processo até não ser mais um json vaizo
            {
                std::cout << "Insira novamente a carta " << i + 1 << " do seu deck: " << std::endl;
                std::getline(std::cin, nome);
                for (char &c : nome)
                {
                    c = tolower(c);
                }
                novodeck.CartasDoDeck[i] = ProcurarCarta(listadecarta, nome);
            }
        }

        for (int j = 0; j < i; j++) // percorre o deck e verifica se tem carta repitida
        {
            if (novodeck.CartasDoDeck[i] == novodeck.CartasDoDeck[j]) // se tiver repete até não ter
            {
                std::cout << "Voce ja possui essa carta em seu deck!" << std::endl;
                while (novodeck.CartasDoDeck[i] == novodeck.CartasDoDeck[j])
                {
                    std::cout << "Insira novamente a carta " << i + 1 << " do seu deck: " << std::endl;
                    std::getline(std::cin, nome);
                    for (char &c : nome)
                    {
                        c = tolower(c);
                    }
                    novodeck.CartasDoDeck[i] = ProcurarCarta(listadecarta, nome);
                }
            }
        }
    }
    contDeck++;
    CalculaPeso(novodeck);  // atribui o peso ao deck
    ImprimirDeck(novodeck); // imprime ele
    listaDeck[contDeck] = novodeck;
    final = 7;
    return novodeck; // retorna ele
}

Deck GerarLogBait(const json &listadecartas)
{
    Deck logbait;
    logbait.CartasDoDeck[0] = ProcurarCarta(listadecartas, "cavaleiro");
    logbait.CartasDoDeck[1] = ProcurarCarta(listadecartas, "barril de goblins");
    logbait.CartasDoDeck[2] = ProcurarCarta(listadecartas, "torre inferno");
    logbait.CartasDoDeck[3] = ProcurarCarta(listadecartas, "foguete");
    logbait.CartasDoDeck[4] = ProcurarCarta(listadecartas, "o tronco");
    logbait.CartasDoDeck[5] = ProcurarCarta(listadecartas, "princesa");
    logbait.CartasDoDeck[6] = ProcurarCarta(listadecartas, "gangue de goblins");
    logbait.CartasDoDeck[7] = ProcurarCarta(listadecartas, "espirito de gelo");
    final = TamanhoDeck - 1;
    CalculaPeso(logbait);
    ImprimirDeck(logbait);
    return logbait;
}

Deck GerarXBesta(const json &listadecartas)
{
    Deck Xbesta;
    Xbesta.CartasDoDeck[0] = ProcurarCarta(listadecartas, "cavaleiro");
    Xbesta.CartasDoDeck[1] = ProcurarCarta(listadecartas, "x-besta");
    Xbesta.CartasDoDeck[2] = ProcurarCarta(listadecartas, "tesla");
    Xbesta.CartasDoDeck[3] = ProcurarCarta(listadecartas, "mago de gelo");
    Xbesta.CartasDoDeck[4] = ProcurarCarta(listadecartas, "o tronco");
    Xbesta.CartasDoDeck[5] = ProcurarCarta(listadecartas, "foguete");
    Xbesta.CartasDoDeck[6] = ProcurarCarta(listadecartas, "esqueletos");
    Xbesta.CartasDoDeck[7] = ProcurarCarta(listadecartas, "tornado");
    final = TamanhoDeck - 1;
    CalculaPeso(Xbesta);
    ImprimirDeck(Xbesta);
    return Xbesta;
}

int VerificacaoString(std::string l)
{
    int k;
    while (true)
    {
        try
        {
            k = std::stoi(l);
            break;
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Por favor, informe um valor valido: " << std::endl;
            std::cin >> l;
        }
    }
    return k;
}

int VerificacaoCompleta1a8(std::string l)
{
    int k = VerificacaoString(l);
    while (k > 8 || k < 1)
    {
        std::cout << "Favor inserir um numero entre 1 e 8: " << std::endl;
        std::cin >> l;
        k = VerificacaoString(l);
    }
    return k;
}

// 2 Remover em K

void RemoverEmK(Deck &deck)
{
    sinal = false;
    if (final > -1)
    {
        std::cout << "Informe a posicao da carta que gostaria de retirar do deck" << std::endl;
        std::cin >> l;
        int k = VerificacaoCompleta1a8(l);

        std::cout << "Confirmar retirada da carta " << k << " Nome:" << deck.CartasDoDeck[k - 1]["nome"] << " Elixir:" << deck.CartasDoDeck[k - 1]["elixir"]
                  << " do seu deck? (S/n)" << std::endl;
        std::cin >> conf;
        if (conf == 'S')
        {
            aux = k - 1;
            while (aux < final)
            {
                deck.CartasDoDeck[aux] = deck.CartasDoDeck[aux + 1];
                aux++;
            }
            final--;
            CalculaPeso(deck);
            sinal = true;
        }
        else
        {
            std::cout << "Confirmacao negada" << std::endl;
        }
    }
    else
    {
        std::cout << "Deck escolhido nao e valido" << std::endl;
    }
}

int main()
{
    json lsitaDeCartas = CarregarCartas();
    Deck logbait = GerarLogBait(lsitaDeCartas);
    RemoverEmK(logbait);
    ImprimirDeck(logbait);

    return 0;
}

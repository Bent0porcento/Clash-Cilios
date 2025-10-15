#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include <locale.h>
#include <cctype>

using json = nlohmann::json; // bglh pra nn ficar escrevendo tudo o cógido

json CarregarCartas()
{
    std::ifstream arquivo_json("cartas.json"); // abrir o json

    if (!arquivo_json.is_open()) // verificar se deu certo
    {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo cartas.json" << std::endl;
        return json();
    }

    json dados_cartas; // parsear as cartas
    try
    {
        arquivo_json >> dados_cartas; // le os dadps pra colocar no array
    }
    catch (json::parse_error &e)
    {
        std::cerr << "Erro de parse no JSON: " << e.what() << std::endl;
        return json();
    }
    return dados_cartas;
}

void ImprimirCartas(const json &c)
{
    std::cout << "--- Catalogo de Cartas do Clash Royale ---" << std::endl
              << std::endl;

    for (const auto &carta : c) // imprime as carta de acordo com as infomarções que ele puxa do json
    {

        int id = carta["id"];
        std::string nome = carta["nome"];
        std::string raridade = carta["raridade"];
        std::string tipo = carta["tipo"];

        std::cout << "ID: " << id << std::endl;
        std::cout << "Nome: " << nome << std::endl;

        if (carta["elixir"].is_number()) // verificação do elixir pq o espelho nn tem elixir fixo
        {
            int elixir = carta["elixir"];
            std::cout << "Elixir: " << elixir << std::endl;
        }
        else
        {
            std::string elixir_str = carta["elixir"];
            std::cout << "Elixir: " << elixir_str << std::endl;
        }

        std::cout << "Raridade: " << raridade << std::endl;
        std::cout << "Tipo: " << tipo << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

json ProcurarCarta(const json &listadecartas, std::string n)
{
    for (const auto &c : listadecartas)
    {
        if (c["nome"] == n)
        {
            return c;
        }
    }
    std::cout << "Impossivel Encontrar sua carta" << std::endl;
    return json();
}

void ImprimirCarta(const json &carta)
{
    std::cout << "ID: " << carta["id"] << std::endl;
    std::cout << "Nome: " << carta["nome"] << std::endl;
    std::cout << "Elixir: " << carta["elixir"] << std::endl;
    std::cout << "Raridade: " << carta["raridade"] << std::endl;
    std::cout << "Tipo: " << carta["tipo"] << std::endl;
}

// 1- Definir Estrutura

struct Deck
{
    json CartasDoDeck[8];
    double peso;
};

void ImprimirDeck(Deck &deck)
{
    for (int i = 0; i < 8; i++)
    {
        std::cout << "Carta " << i + 1 << ": " << deck.CartasDoDeck[i]["nome"] << std::endl;
    }
    std::cout << "Peso: " << deck.peso << std::endl;
}

void CalculaPeso(Deck &deck)
{
    int elixirT = 0;
    double peso;
    for (int i = 0; i < 8; i++)
    {
        elixirT += deck.CartasDoDeck[i]["elixir"].get<int>();
    }
    peso = elixirT / 8.0;
    deck.peso = (std::round(peso * 10.0)) / 10.0;
}

Deck CriarDeck(const json &listadecarta)
{
    Deck novodeck;
    std::string nome;
    int elixirT = 0;
    double peso;
    std::cout << "========== Comecando Montagem de Deck ==========" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << "Informe o nome da carta " << i + 1 << " do seu novo deck: " << std::endl;
        std::getline(std::cin, nome);
        for (char &c : nome)
        {
            c = tolower(c);
        }

        for (const auto &c : listadecarta)
        {
            if (c["nome"] == nome)
            {
                novodeck.CartasDoDeck[i] = c;
            }
        }
    }
    CalculaPeso(novodeck);
    ImprimirDeck(novodeck);
    return novodeck;
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
    CalculaPeso(Xbesta);
    ImprimirDeck(Xbesta);
    return Xbesta;
}

int main()
{

    json lsitaDeCartas = CarregarCartas();

    GerarXBesta(lsitaDeCartas);

    return 0;
}

#include "../include/TabelaHash.h"

#include <iostream>

#define R 31
#define R2 29

// Função de Hashing para Strings
int hashString(const std::string& str, int M)
{
    int hashComputado = 0;
    for (unsigned char ch : str)
        hashComputado = (R * hashComputado + ch) % M;
    return hashComputado;
}

int hashDupla(const std::string& str, int M)
{
    int hashComputado = 0;
    for (unsigned char ch : str)
        hashComputado = (R2 * hashComputado + ch) % M;
    return hashComputado;
}

//////////////////////////////////
/// CONSTRUTORES E DESTRUTORES ///
//////////////////////////////////

TabelaHash_EndAberto::TabelaHash_EndAberto(unsigned int m):
M(m)
{
    tabela.resize(M);
    for(unsigned int i=0;i<M;i++)
    {
        tabela[i].ocupado=false;
        tabela[i].registro=NULL;
    }

    totalColisoes=0;
}

TabelaHash_EndFechado::TabelaHash_EndFechado(unsigned int m, unsigned int t):
M(m), TamanhoBucket(t)
{
    tabelaChaining.resize(M);

    tabelaBuckets.resize(M);
    for(unsigned int i=0;i<M; i++)
        tabelaBuckets[i].reserve(TamanhoBucket);

    totalColisoes=0;
}

TabelaHash_EndAberto::~TabelaHash_EndAberto()
{

}

TabelaHash_EndFechado::~TabelaHash_EndFechado()
{

}

////////////////////////////
/// ENDERECAMENTO ABERTO ///
////////////////////////////

bool TabelaHash_EndAberto::insereEABL(std::string chave, Registro* reg)
{
    int colisoes=0;
    bool inseriu = false;

    ///TODO: Completar funcao de insercao em tabela de enderecamento aberto com busca linear
    int endCalc, endLivre, marca;
    bool chaveExiste = false;

    marca = -1;
    endCalc = hashString(chave, M);
    endLivre = endCalc;

    if (tabela[endCalc].ocupado) {
        colisoes = 1;
    }

    do {
        if (tabela[endLivre].ocupado) {
            if (tabela[endLivre].chave == chave) {
                inseriu = true;
                break;
            } else {
                endLivre = (endLivre + 1) % tabela.size();
            }
        } else {
            inseriu = true;
            marca = endLivre;
            break;
        }
    } while (endLivre != endCalc);

    if (marca != -1) {
        tabela[marca].chave = chave;
        tabela[marca].ocupado = true;
        tabela[marca].registro = reg;
    }

    totalColisoes += colisoes;

    return inseriu;
}

Registro* TabelaHash_EndAberto::buscaEABL(std::string chave)
{
    Registro* r = NULL;

    ///TODO: Completar funcao de busca em tabela de enderecamento aberto com busca linear
    int endCalc, endLivre;
    endCalc = hashString(chave, M);
    endLivre = endCalc;

    do {
        if (tabela[endLivre].ocupado) {
            if (tabela[endLivre].chave == chave) {
                r = tabela[endLivre].registro;
                break;
            }
            endLivre = (endLivre + 1) % tabela.size();
        }
    } while (r == nullptr && endLivre != endCalc);

    return r;
}

bool TabelaHash_EndAberto::insereDoubleHashing(std::string chave, Registro* reg)
{
    int colisoes=0;
    bool inseriu = false;

    ///TODO: Completar funcao de insercao em tabela de enderecamento aberto com double hashing
    int endCalc, endLivre, marca, fatorDuploHash;
    bool chaveExiste = false;

    marca = -1;
    endCalc = hashString(chave, M);
    endLivre = endCalc;
    fatorDuploHash = hashDupla(chave, M);

    if (tabela[endCalc].ocupado) {
        colisoes = 1;
    }

    int j = 0;
    do {
        if (tabela[endLivre].ocupado) {
            if (tabela[endLivre].chave == chave) {
                inseriu = true;
                break;
            } else {
                endLivre = (endLivre + (j * fatorDuploHash)) % tabela.size();
            }
        } else {
            inseriu = true;
            marca = endLivre;
            break;
        }

        j = j + 1;
    } while (endLivre != endCalc);

    if (marca != -1) {
        tabela[marca].chave = chave;
        tabela[marca].ocupado = true;
        tabela[marca].registro = reg;
    }

    totalColisoes += colisoes;

    return inseriu;
}

Registro* TabelaHash_EndAberto::buscaDoubleHashing(std::string chave)
{
    Registro* r = NULL;

    ///TODO: Completar funcao de busca em tabela de enderecamento aberto com double hashing
    int endCalc, endLivre, fatorDuploHash;
    endCalc = hashString(chave, M);
    endLivre = endCalc;
    fatorDuploHash = hashDupla(chave, M);

    int j = 0;
    do {
        if (tabela[endLivre].ocupado) {
            if (tabela[endLivre].chave == chave) {
                r = tabela[endLivre].registro;
                break;
            }
            endLivre = (endLivre + (j * fatorDuploHash)) % tabela.size();
        }

        j = j + 1;
    } while (r == nullptr && endLivre != endCalc);

    return r;
}

int TabelaHash_EndAberto::colisoesOcorridas()
{
    return totalColisoes;
}

/////////////////////////////
/// ENDERECAMENTO FECHADO ///
/////////////////////////////

bool TabelaHash_EndFechado::insereChaining(std::string chave, Registro* reg)
{
    int colisoes=0;
    bool inseriu = false;

    ///TODO: Completar funcao de insercao em tabela de enderecamento fechado com encadeamento (chaining)
    int hashComputada;
    hashComputada = hashString(chave, M);

    Elemento elem;
    elem.chave = chave;
    elem.registro = reg;

    auto& lista = tabelaChaining[hashComputada];
    if (lista.empty())
    {
        lista.push_back(elem);
        inseriu = true;
    } else {
        colisoes = 1;

        bool encontrou = false;
        for (auto elem : lista)
        {
            if (elem.chave == chave) {
                encontrou = true;
                inseriu = true;
                break;
            }
        }

        if (!encontrou) {
            lista.push_back(elem);
            inseriu = true;
        }
    }

    totalColisoes += colisoes;

    return inseriu;
}

Registro* TabelaHash_EndFechado::buscaChaining(std::string chave)
{
    Registro* r = NULL;

    ///TODO: Completar funcao de busca em tabela de enderecamento fechado com encadeamento (chaining)
    int hashComputada;
    hashComputada = hashString(chave, M);

    for (const auto& elem : tabelaChaining[hashComputada])
    {
        if (elem.chave == chave) {
            r = elem.registro;
            break;
        }
    }

    return r;
}

bool TabelaHash_EndFechado::insereBuckets(std::string chave, Registro* reg)
{
    int colisoes=0;
    bool inseriu = false;

    ///TODO: Completar funcao de insercao em tabela de enderecamento fechado com blocos de tamanho fixo (buckets) e area de overflow
    int hashComputada = hashString(chave, M);

    auto& bucket = tabelaBuckets[hashComputada];

    Elemento elem;
    elem.chave = chave;
    elem.registro = reg;

    if (bucket.empty())
    {
        inseriu = true;
        bucket.push_back(elem);
    }
    else if (bucket.size() == bucket.capacity())
    {
        colisoes = 1;
        inseriu = true;

        overflow.push_back(elem);
    } else {
        colisoes = 1;
        inseriu = true;

        bool encontrou = false;
        for (const auto& elem : bucket) {
            if (elem.chave == chave) {
                encontrou = true;
                break;
            }
        }

        if (!encontrou) {
            bucket.push_back(elem);
        }
    }


    totalColisoes += colisoes;

    return inseriu;
}

Registro* TabelaHash_EndFechado::buscaBuckets(std::string chave)
{
    Registro* r = NULL;

    ///TODO: Completar funcao de busca em tabela de enderecamento fechado com blocos de tamanho fixo (buckets) e area de overflow

    int hashComputada;
    hashComputada = hashString(chave, M);
    const auto& bucket = tabelaBuckets[hashComputada];

    for (const auto& elem : bucket)
    {
        if (elem.chave == chave) {
            r = elem.registro;
            break;
        }
    }

    if (r == NULL)
    {
        for (const auto& elem : overflow)
        {
            if (elem.chave == chave) {
                r = elem.registro;
                break;
            }
        }
    }

    return r;
}

int TabelaHash_EndFechado::colisoesOcorridas()
{
    return totalColisoes;
}

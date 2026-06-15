#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>
#include <cstring>
using namespace std;

const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;
const int TAM_HASH = 997;

//cabeçalho das structs
struct Publicacao;
struct usuario;
struct NoLista;
struct noListPosts;
struct NoArvore;
struct NoArvorePosts;
struct MiniRede;
struct Fila;
struct NoNotificacao;
struct NoRanking;

struct Fila{
    NoNotificacao *inicio;
    NoNotificacao *fim;
};

struct NoNotificacao{
    int idPost;
    int idUser;
    NoNotificacao *prox;
    int tipo;  // 1: seguir  2: like 
};

struct Publicacao
{
    int id;
    int idAutor;
    int timestamp;
    char texto[TAM_TEXTO];
    int curtidas;
    NoLista *usersLike;

}; 
struct usuario
{
    int id;
    char username[TAM_USERNAME];
    char nome[TAM_NOME];
    NoLista *seguindo;
    noListPosts *publicacoes;
    Fila *notificacoes;
};

struct NoLista
{
    usuario *user;
    NoLista *prox;
};

struct noListPosts
{
    Publicacao *publicacao;
    noListPosts *prox;
};

struct NoArvore
{
    usuario *user;
    NoArvore *esq;
    NoArvore *dir;
    int FB = 0;
};

struct NoArvorePosts
{
    Publicacao *publicacao;
    NoArvorePosts *esq;
    NoArvorePosts *dir;
    int FB = 0;
};

struct NoRanking
{
    Publicacao *post;
    NoRanking *prox;
};




struct MiniRede
{
    NoLista *tabelaHash[TAM_HASH];
    NoArvore *raizArvore;
    NoArvorePosts *raizArvorePosts;

};

// Sugestao de structs que provavelmente serao necessarias:
// - Usuario
// - Publicacao
// - MiniRede
// - nos para lista encadeada
// - nos para arvore binaria de usuarios por id
// - nos para tabela hash de usernames
// - nos para fila de notificacoes
//
// Os campos de cada struct fazem parte do projeto dos alunos.

void inicializarMiniRede(MiniRede &rede);
void liberarMiniRede(MiniRede &rede);
void processarComandos(MiniRede &rede, std::istream &entrada, std::ostream &saida);
void cadastrarUsuario(MiniRede &rede, int id, const char username[], const char nomeCompleto[], std::ostream &saida);
void buscarUsuarioPorId(MiniRede &rede, int id, std::ostream &saida);
void buscarUsuarioPorUsername(MiniRede &rede, const char username[], std::ostream &saida);
void listarUsuarios(MiniRede &rede, std::ostream &saida);
void seguirUsuario(MiniRede &rede, int idSeguidor, int idSeguido, std::ostream &saida);
void listarSeguindo(MiniRede &rede, int idUsuario, std::ostream &saida);
void cadastrarPublicacao(MiniRede &rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream &saida);
void curtirPublicacao(MiniRede &rede, int idUsuario, int idPost, std::ostream &saida);
void consultarNotificacoes(MiniRede &rede, int idUsuario, int k, std::ostream &saida);
void gerarFeed(MiniRede &rede, int idUsuario, int k, std::ostream &saida);
void listarTopPosts(MiniRede &rede, int k, std::ostream &saida);

// TODO: declarar aqui as funcoes auxiliares escolhidas pelo grupo.
//
// Exemplos de responsabilidades auxiliares:
// - buscar usuario por id
// - buscar usuario por username
// - buscar publicacao por id
// - inserir/listar/liberar arvore
// - inserir/buscar/liberar tabela hash
// - enfileirar/desenfileirar notificacoes
// - manipular listas encadeadas
// - ordenar vetores auxiliares para feed e ranking

// nossas funcoes auxiliares
int funcaoHash(const char username[], int tamanhoTabela);
void inserirHash(NoLista *tabela[], int tamanhoTabela, usuario *novoUsuario);
usuario *buscarHash(NoLista *tabela[], int tamanhoTabela, const char usernameBuscado[]);
NoArvore *rotDir(NoArvore *y);
NoArvore *rotEsq(NoArvore *y);
NoArvore *rotEsqDir(NoArvore *a);
NoArvore *rotDirEsq(NoArvore *a);
NoArvore *casoEsquerda(NoArvore *a, bool &aumentouAltura);
NoArvore *casoDireita(NoArvore *a, bool &aumentouAltura);
NoArvore *insereAVL(NoArvore *a, usuario *novoUsuario, bool &aumentouAltura);
usuario *buscarArvore(NoArvore *no, int idBuscado);
void imprimeEmOrdem(NoArvore *No, std::ostream &saida);
bool insereLista(NoLista *&inicio, usuario *n);
void addNotificacoes(Fila &notificacoes, int tipo, int idUser, int idPost);
void arvParaLista(NoArvorePosts* raiz, NoRanking *&inicio);
NoRanking *merge(NoRanking *esq, NoRanking *dir);
void Particao(NoRanking *fonte, NoRanking *&inicio, NoRanking *&fim);
void mergeSort(NoRanking *&inicio);

#endif

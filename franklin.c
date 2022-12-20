#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

const char valores[][3] = {{"A"}, {"2"}, {"3"},  {"4"}, {"5"}, {"6"}, {"7"},
                           {"8"}, {"9"}, {"10"}, {"V"}, {"D"}, {"R"}, {"C"}};
const char naipes[][4] = {{"♥"}, {"♠"}, {"♦"}, {"♣"}};

typedef struct Carta {
  int valor;
  int naipe;
} Carta;

int findValue(char *carta_recebida) {
  if (strncmp(carta_recebida, "1", 1) == 0) {
    return 9;
  }

  for (int i = 0; i < 14; i++) {
    if (strncmp(valores[i], carta_recebida, 1) == 0) {
      return i;
    }
  }
}

int findSuit(char *carta_recebida) {
  for (int i = 0; i < 4; i++) {
    if (strstr(carta_recebida, naipes[i]) != NULL) {
      return i;
    }
  }
}

Carta *criarCarta(char *carta_recebida) {
  Carta *carta_criada = malloc(sizeof(Carta));

  carta_criada->valor = findValue(carta_recebida);
  carta_criada->naipe = findSuit(carta_recebida);

  return carta_criada;
}

int countCards(Carta **mao, int *copas_na_mao, int *espadas_na_mao, int *ouros_na_mao, int *paus_na_mao, int cartas_na_mao) {

	*copas_na_mao = 0;
	*espadas_na_mao = 0;
	*ouros_na_mao = 0;
	*paus_na_mao = 0;
	
  for (int i = 0; i < cartas_na_mao; i++) {
    switch (mao[i]->naipe) {
    case 0:
      *copas_na_mao += 1;
      break;
    case 1:
      *espadas_na_mao += 1;
      break;
    case 2:
      *ouros_na_mao += 1;
      break;
    case 3:
      *paus_na_mao += 1;
      break;
    }
  }

  int naipes_na_mao[] = {*copas_na_mao, *espadas_na_mao, *ouros_na_mao,
                         *paus_na_mao};
  int temp = 0;
  int naipe_mais_numeroso;

  for (int j = 0; j < 4; j++) {
    if (naipes_na_mao[j] > temp) {
      naipe_mais_numeroso = j;
      temp = naipes_na_mao[j];
    }
  }

  return naipe_mais_numeroso;
}

int jogadaPossivel(Carta **mao, Carta *mesa, int cartas_na_mao) {
  for (int i = 0; i < cartas_na_mao; i++) {
    if (mao[i]->valor == 0 || mao[i]->valor == 13 ||
        mao[i]->valor == mesa->valor || mao[i]->naipe == mesa->naipe) {
      return 1;
    }
  }
  return 0;
}

int escolherCarta(Carta** mao, Carta* mesa, int naipe_mais_numeroso, int cartas_na_mao){

	if(cartas_na_mao >= 8){
		goto cartas_comuns;
	}
	
	if(naipe_mais_numeroso >= 5){
		goto as;
	}

	coringa:
	for(int i = 0; i < cartas_na_mao; i++){
			if(mao[i]->valor == 13){
				return i;
			}
	}

	valete:
	for(int i = 0; i < cartas_na_mao; i++){
		if(mao[i]->valor == 11 && mao[i]->naipe == mesa->naipe){
			return i;
		}
	}

	as:
	for(int i = 0; i < cartas_na_mao; i++){
		if(mao[i]->valor == 0){
			return i;
		}
	}

	cartas_comuns:
	for(int i = 0; i < cartas_na_mao; i++){
		if(mao[i]->valor == mesa->valor || mao[i]->naipe == mesa->naipe){
			return i;
		}
	}
	
	pula_volta:
	for(int i = 0; i < cartas_na_mao; i++){
		if(mao[i]->valor == 11 && mao[i]->naipe == mesa->naipe || mao[i]->valor == 12 && mao[i]->naipe == mesa->naipe){
			return i;
		}
	}

	return 0;
}

int discard(Carta** mao, int carta_escolhida, int cartas_na_mao){
	
	for(int i = carta_escolhida; i < cartas_na_mao; i++){
		mao[i] = mao[i + 1];
	}
	
	return cartas_na_mao - 1;
}

int main() {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  char id[MAX_ID_SIZE];
  char meu_id[MAX_ID_SIZE];
  char acao[MAX_ACTION];
  char complemento[MAX_LINE];
  char complemento2[MAX_LINE];
  char temp[MAX_LINE];
  Carta *mao[60];
  for (int i = 0; i < 30; i++) {
    mao[i] = malloc(sizeof(Carta *));
  }
  Carta *mesa = malloc(sizeof(Carta));
  int cartas_na_mao = 0;
  int copas_na_mao, espadas_na_mao, ouros_na_mao, paus_na_mao;
  int naipe_mais_numeroso;
  int carta_escolhida;
	int puxar = 0;

  scanf("%*s %[^\n]", temp);

  scanf("%*s %s", meu_id);

  while (1) {
    scanf("%s", temp);

    if (strcmp(temp, "[") == 0 || strcmp(temp, "HAND") == 0) {
      continue;
    } else if (strcmp(temp, "]") == 0) {
      break;
    }
    mao[cartas_na_mao] = criarCarta(temp);
    cartas_na_mao++;
  }

  scanf("%*s %s", temp);

  mesa = criarCarta(temp);

  while (1) {
    do {
      scanf("%s %s", acao, complemento);

      if (strcmp(acao, "DISCARD") == 0) {
				puxar = 0;
        mesa = criarCarta(complemento);
        if (mesa->valor == 0 || mesa->valor == 13) {
          scanf("%s", complemento2);
          mesa->naipe = findSuit(complemento2);
        }
      }
			
			if (mesa->valor == 13 || mesa->valor == 10){
				puxar++;
			}
			fprintf(stderr, "%d\n", puxar);
    } while (strcmp(acao, "TURN") || strcmp(complemento, meu_id));
			
		// for(int i = 0; i < cartas_na_mao; i++){
		// 	if(i == 0){
		// 		fprintf(stderr, "Mesa: %s%s Cartas na Mão: %d\n", valores[mesa->valor], naipes[mesa->naipe], cartas_na_mao);
		// 	}
		// 	fprintf(stderr, "%s%s ", valores[mao[i]->valor], naipes[mao[i]->naipe]);
		// }
		
    // CORINGA (COMPRE 4)
    if (mesa->valor == 13 && puxar == 2) {
			puxar = 0;
			printf("SAY QUIABO SURDO! (╯°□°)╯︵ ┻━┻\n");
      printf("BUY 4\n");
      for (int k = 0; k < 4; k++) {
        scanf("%s", temp);
        mao[cartas_na_mao] = criarCarta(temp);
        cartas_na_mao++;
      }
      continue;
    }

    // VALETE (COMPRE 2)
    if (mesa->valor == 10 && puxar == 2) {
			puxar = 0;
			printf("SAY SÓ AGUARDE, O TEU TÁ GUARDADO... \n");
      printf("BUY 2\n");
      for (int l = 0; l < 2; l++) {
        scanf("%s", temp);
        mao[cartas_na_mao] = criarCarta(temp);
        cartas_na_mao++;
      }
      continue;
    }
		
    // SEM CARTAS DISPONÍVEIS
    if (jogadaPossivel(mao, mesa, cartas_na_mao) == 0) {
      printf("BUY 1\n");
      scanf("%s", temp);
      mao[cartas_na_mao] = criarCarta(temp);
      cartas_na_mao++;
      continue;
    }

		// ESCOLHA DA CARTA A SER DESCARTADA
		naipe_mais_numeroso = countCards(mao, &copas_na_mao, &espadas_na_mao, &ouros_na_mao, &paus_na_mao, cartas_na_mao);
    carta_escolhida = escolherCarta(mao, mesa, naipe_mais_numeroso, cartas_na_mao);
		
		// fprintf(stderr, "Carta Escolhida: %d", carta_escolhida);
		// ENVIO DA AÇÃO
		if(mao[carta_escolhida]->valor == 0 || mao[carta_escolhida]->valor == 13){
			printf("DISCARD %s%s %s\n", valores[mao[carta_escolhida]->valor], naipes[mao[carta_escolhida]->naipe], naipes[naipe_mais_numeroso]);
			mesa->valor = mao[carta_escolhida]->valor;
			mesa->naipe = naipe_mais_numeroso;
			cartas_na_mao = discard(mao, carta_escolhida, cartas_na_mao);
		}
		else{
			printf("DISCARD %s%s\n", valores[mao[carta_escolhida]->valor], naipes[mao[carta_escolhida]->naipe]);
			mesa->valor = mao[carta_escolhida]->valor;
			mesa->naipe = mao[carta_escolhida]->naipe;
			cartas_na_mao = discard(mao, carta_escolhida, cartas_na_mao);
		}
  }
  return 0;
}
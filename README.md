Montar uma rede em anel com 4 máquinas usando Socket DGRAM

Anel é unidirecional

4 ssh cada ssh em um socket

1024 até 65000 - portas

Jogo Uno

Regras  -
    - Jogador com bastão faz a jogada
    
    - Depois de escolher a carta, enviar uma mensagem informado a jogada:
    Mensagem deve dar toda a volta no anel e voltar p/ remetente.
    
    - Depois rementente envia bastão p/ próxima maquina
    
    - Uma carta de 0 a 9 de cada cor VERMELHO,AZUL,AMARELO,VERDE
    
    - 8 cartas pular o próximo
    
    - 8 cartas compra 2
    
    - Inverter o jogo e conringas não são obrigatório
    
    - (+2) sobre (+2) por conta da equipe
    
    - Informar quando falta um só carta é obrigatório (TRUCO)
    
    - cuidado com deadlock , o que não estiver especificado é livre
    
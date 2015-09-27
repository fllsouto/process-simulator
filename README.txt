#   **************************************************************************************************
#
#   Fellipe Souto Sampaio - 7990422
#   Computer Science Undergraduate - University of São Paulo (IME/USP)
#   Operational Systems - Program Exercise I - README
#   Prof: Daniel M. Batista
#
#   September, 2015
#
#   **************************************************************************************************

############################################################################################################
                                          Como Compilar?
############################################################################################################
  Para compilar o programa execute dentro da pasta 'ep1-fellipesouto/' o comando : 

$ make exec

  Isso irá chamar o makefile que irá compilar o ep1sh e o ep1. Os arquivos *.o serão movidos para a pasta
'scr/objects' e os dois binários executáveis serão movidos para a pasta 'exec/' .
Para limpar e executar a compilação de novo execute o comando : 

$ make clean

  Ele irá apagar os *.o gerados pela compilação e os binários da pasta 'exec/'

############################################################################################################
                                        Como executar?
############################################################################################################
  Para executar o binário 'ep1sh' rode o comando a seguir, dentro da pasta exec se preferir :

$ ./ep1sh

  Ao executar esse comando o shell interativo do EP estará pronto para receber os seguintes comandos:

pwd - Imprime o path completo do diretório corrente
cd <pasta> - Muda de diretório, caso não exista um erro é impresso na tela
/bin/ls -l - Lista todos os arquivos existentes no diretório corrente
./ep1 <argumentos do EP1> - Executa o simulador de processos com os devidos parâmetros passados

O shell ainda aceita comandos de edição fornecidos pela biblioteca readline, como busca de histórico usando 
as teclas para cima e para baixo, navegação na linha com as setas direita e esquerda, dentre outros.

  Por fim, para sair do ep1sh basta digitar o comando : 

$ exit

  Que irá interromper a execução do binário.

############################################################################################################
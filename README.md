<h1 align="center"> Chatroom-SD</h1>
<p align="center">Sistema de Chat construido para cadeira de Sistemas Distribuidos, com o objetivo de criar um chat público onde todos os usuários se comuniquem, e chats privados entre os usuários, vale ressaltar que o código foi desenvolvido e testado apenas no Linux(Ubuntu).</p>
<p align="center">Esse código tomou como base o seguinte sistema: https://gist.github.com/Abhey/47e09377a527acfc2480dbc5515df872, pois foi nos dado a dica de usar códigos já existentes, e modificar eles para seguir as exigências requeridas na atividade<p>

Como Rodar
============

Compilar o Código
```bash
$ make Makefile compile
```

Iniciar o Server
```bash
$ ./ChatServer
```


Iniciar os Usuários(Utilizando diferentes terminais entre si, e também diferente do terminal do servidor)
```bash
$ ./ChatClient
```

# Como funciona
## Usuário
<p>Para o usuário já ira ser printado em sua tela como fazer todos os comandos possiveis por eles, além de mostra para eles o ID_num que foi fornecido a eles, caso o usuário saia do chat, e posteriormente ele volte, um ID_num diferente ira ser dado a ele. Segue abaixo os comandos que o usuário pode utilizar, e o que ele pode receber de resposta do servidor.<p>

 * listar → Ira listar todos os usuários conectados no momento, da seguinte maneira (Caso um usuário de ID 3 saia, ele não ira ser listado):
  ```bash
  > listar
  < "ID (ID_num) (IP) (PORTA)"
  ```
 * enviar → Para enviar corretamente a mensagem para outros usuário (Caso o ID_num escolhido for o 0, a mensagem ira ser enviada a todos os usuários):
 ### Tele de quem envia a mensagem:
  ```bash    
  > enviar (ID_num) <Mensagem>
  < "Mensagem enviada a (ID_num) em [Data Horário] → [ ENVIADA COM SUCESSO. ] { ou → [ID NÃO ESTÁ ONLINE.]}"
  ```
            
 ### Tela de quem recebe a mensagem:
  ```bash
  < "Msg do ID (ID_envia) [IP:PORTA]: para (ID_recebe) [IP:PORTA][Data Horário]: <mensagem>"
  ```     
 * sair → Ao digitar sair, o usuário ira sair automaticamente do chat, e informara apenas para o servidor que ele saiu, caso ele volte, um ID_num diferente ira ser designado a ele:
  ```bash
  > sair
  < "[-] Desconectado do socket."
  ```
 * Comando errado: → Ira ter um print na tela do usuário → [!][CONDIÇÃO NÃO EXISTE!], e o sistema ira esperar a próxima ação do usuário, tanto o comando pode está errado, como também utilizar variaveis indevidamente (Ex.: ID Varelo; o ID esperado é um int, porém ele recebe umas string).
       
## Servidor
<p>O servidor vai ter apenas o comando de fechar o servidor, não tem comandos de expulsar, ou bloquear os usuários, porém é printado em sua tela todas as mensagens, tanto privadas como públicas, além de mostrar quem entra ou sai do chatroom.<p>
    
    * Exemplo de entrada e saida de usuário:
  ```bash    
  < "Conexão aceita de ID (ID_num) em [IP:PORTA]"
  < "Cliente do ID (ID_num) no socket (socket_num) em [IP:PORTA] saiu!"
  ```
### Para fechar o chatroom, o servidor tem que usar o comando: CTRL + C. 


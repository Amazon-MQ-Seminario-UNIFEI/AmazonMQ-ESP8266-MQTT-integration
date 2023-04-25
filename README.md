# AmazonMQ-ESP8266-MQTT-integration
Código para ESP8266 para se conectar ao broker na AmazonMQ utilizando MQTT e Apache ActiveMQ

Problemática proposta: A aplicação didática desenvolvida no projeto foi a utilização do Amazon MQ para realizar a comunicação entre dois dispositivos: um simulando o cliente e o outro o servidor. O backend realiza três tarefas básicas:

  - Resposta a uma mensagem de texto
  - Alterações em um arquivo de texto
  - Cálculo de uma função

A máquina cliente solicita ao servidor que execute uma dada tarefa, que a executa retornando uma mensagem de resposta. No caso exemplo adotado, foram utilizados como clientes um ESP8266 e/ou um aplicativo Flutter. Para servidor (backend), foi utilizado um código em Java em um notebook.

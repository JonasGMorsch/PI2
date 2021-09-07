# Concepção:

O objetivo do projeto é desenvolver um módulo de controle de som, tv e ar condicionado, utilizando de protocolo MQTT e um receptor infra-vermelho como entradas.
Como saídas do microcontrolador serão utilizados um emissor infra-vermelho e um amplificador operacional com controle de ganho digital 

* O servidor deve ser independente e assíncrono, ou seja, uma ação pedida para ser executada fica armazenada no servidor até ser possível entregá-la ao modulo de controle corespondente, conhecido como QOS 1.


* Controle dos dispositivos:
  * Via internet (Protocolo IPV6)
  * Pelo aplicativo [MQTT Dash](https://play.google.com/store/apps/details?id=net.routix.mqttdash&hl=en&gl=US), configurado manualmente
  * Pelo controle remoto da TV, controle de volume do som


* Módulos de controle devem fazer a comunicação via WiFi


* Automatização sistema de som:
  * Ligar e desligar a TV via infravermelho
  * Ligar e desligar amplificador junto com a tv
  * Regular o volume utilizando o controle da tv
  * Controlar ar condicionado via infravermelho

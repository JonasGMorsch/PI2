# Módulos:

## Controle de Volume:

Esta placa contém os [FM62429](http://nice.kaze.com/M62429.pdf), os capacidores de desacoplamento DC e filtro passa baixa para os canais do subwoofer. A alimentacão vem da fonte 48V que alimenta o amplificador, sendo reduzida para 12V por um conversor buck devidamente filtrado já na placa amplificadora, e por último, passando pelo 7805 e um capacitor de 470uF como etapa final adequação de tensão e rejeição á ruído. É de suma importância separar a fonte dos pré amplificadores do circuito digital, pois o volume do ruído de chavemento após o ganho de 21.5dB do amplificador se torna inaceitável.

<p align="center">
<img src="/Implemantacao/Imagens/1.jpg?raw=true" width="36%"> <img src="/Implemantacao/Imagens/2.jpg?raw=true" width="33%">
</p>

## Amplificador:

Amplificador de som, utilizando o chip [TPA3255](https://www.ti.com/lit/ds/symlink/tpa3255.pdf) modificado de 3 canais (150+150+300W) para 4 canais de 150w, com o objetivo de alimentar dois subwoofers com a impedância adequada no futuro.
Para os testes, devido as condições, será utilizado:
* Um subwoofer ativo mono, com um filtro atenuador na entrada
* Canais L e R estão sendo somados após a filtragem
* Apenas utilizada uma das saídas de subwoofer. 

Entrada de som adaptada de RCA para P2, podendo utilizar o cabo padrão da própria TV.

O ganho do [TPA3255](https://www.ti.com/lit/ds/symlink/tpa3255.pdf) é fixo em 21.5dB, suficiente para saturar o amplificador com [níveis de linha](https://en.wikipedia.org/wiki/Line_level#Nominal_levels) de uma saída de televisão.

O amplificador já conta com uma fonte 12V de baixo ruído que servia o propósito de alimentar além da linha de pré amplificadores do chip, os amplificadores operacionais originais que faziam o serviço de filtro e controle de ganho.

<p align="center">
  <img src="/Implemantacao/Imagens/3.jpg?raw=true" width="40%"> 
</p>
<p align="center">
  <img src="/Implemantacao/Imagens/4.jpg?raw=true" width="40%"> 
  <img src="/Implemantacao/Imagens/5.jpg?raw=true" width="40%"> 
</p>

## Amplificador montado:

<p align="center">
<img src="/Implemantacao/Imagens/6.jpg?raw=true" width="50%" align="center">
</p>

## Placa do microcontrolador:

Possui uma fonte 9V separada dos outros módulos, que além de separar o ruído do amplificador tem o propósito de rodar continuamente.

Resposável pela maior parte das tarefas:  
* Controlar o volume 
* Monitorar a temperatura
* Controlar a televisão
* Controlar o ar condicionado
* Receber os sinais do celular ou controle remoto.

<p align="center">
<img src="/Implemantacao/Imagens/7.jpg?raw=true" width="44%"> <img src="/Implemantacao/Imagens/8.jpg?raw=true" width="21.8%"> <img src="/Implemantacao/Imagens/9.jpg?raw=true" width="33%"> 
</p>

## Todos os Módulos conectados:
<p align="center">
<img src="/Implemantacao/Imagens/10.jpg?raw=true" width="70%">
</p>

## Outros Dispositivos:
Banana Pi, rodando o servidor MQTT e aplicativo MQTT Dash configurado:
<p align="center">
  <img src="/Implemantacao/Imagens/11.jpg?raw=true" width="50%">
  <img src="/Implemantacao/Imagens/12.jpg?raw=true" width="20%">
</p>

# Programação:

## Teste de componentes e bibliotecas:

O programa para construção do código é a IDE do [Arduino](https://www.arduino.cc/en/Guide), em conjunto com o [compilador e bibliotecas adaptadas](https://github.com/esp8266/Arduino) para o [ESP8266](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf), utilizando a versão já configurada e testada disponível neste repositório:

https://github.com/JonasGMorsch/My-Arduino-IDE-for-ESP8266

Biblioteca MQTT

https://github.com/knolleary/pubsubclient

[Código de Validacão](/Implemantacao/mqtt_esp8266/mqtt_esp8266.ino)

Controle de Volume

https://github.com/CGrassin/M62429_Arduino_Library

[Código de Validacão](/Implemantacao/FM62429/FM62429.ino)

Controle Infravermelho

https://github.com/crankyoldgit/IRremoteESP8266

[Código de Validacão](/Implemantacao/IRrecvDumpV2/IRrecvDumpV2.ino)

Monitoramento de Temperatura e Humidade:

https://github.com/JonasGMorsch/GY-21

O código já foi validado no próprio repositório

## Código Completo
[MQTT_JONAS_AMP](/Implemantacao/MQTT_JONAS_AMP/MQTT_JONAS_AMP.ino)

[Biblioteca MQTT Customizada](/Implemantacao/JONAS_MQTT.h)

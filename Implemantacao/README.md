# Bibliotecas Testadas:

### Arduino para ESP8266, contém bibliotecas para wifi, i2c e afins.
https://github.com/esp8266/Arduino

### Biblioteca MQTT
https://github.com/knolleary/pubsubclient

### Controle de Volume
https://github.com/CGrassin/M62429_Arduino_Library

Datasheet : http://datasheet.elcodis.com/pdf2/76/14/761466/fm62429.pdf

### Controle Infravermelho
https://github.com/crankyoldgit/IRremoteESP8266


# Módulos:

## Controle de Volume e filtro passa baixa:

Esta placa contém os dois FM62429, os capacidores de desacoplamento DC, filtro passa baixa para os canais do subwoofer. A alimentacão vem da fonte 48V que alimenta o amplificador, sendo reduzida para 12V por uma conversor buck devidamente filtrado, e por último, passando pelo 7805 e um capacitor de 470uF como etapa final adequação de tensão e rejeição á ruído. É de suma importância separar a fonte dos pré amplificadores do circuito digital, pois o volume do ruído de chavemento após a amplificação de 22dB do amplificador se torna inaceitável. 

<p align="center">
<img src="/Implemantacao/Imagens/1.jpg?raw=true" width="36%"> <img src="/Implemantacao/Imagens/2.jpg?raw=true" width="33%">
</p>

## Amplificador, adaptado para receber a placa de controle de volume, desacoplador DC para o sinal do subwoffer e entrada P2:

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

## Placa do microcontrolador, contendo os módulos Infravermelho e sensor de temperatura:

<p align="center">
<img src="/Implemantacao/Imagens/7.jpg?raw=true" width="44%"> <img src="/Implemantacao/Imagens/8.jpg?raw=true" width="21.8%"> <img src="/Implemantacao/Imagens/9.jpg?raw=true" width="33%"> 
</p>

## Todos os Módulos conectados:
<p align="center">
<img src="/Implemantacao/Imagens/10.jpg?raw=true" width="70%">
</p>

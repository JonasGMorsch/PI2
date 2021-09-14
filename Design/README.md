# Design

## Componentes Utilizados:

01 Node MCU v1 - Microcontrolador baseado no [Esp8266](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)

01 [GY-21](https://cdn-shop.adafruit.com/datasheets/1899_HTU21D.pdf) - Sensor de Temperatura e Humidade

02 [FM62429](http://nice.kaze.com/M62429.pdf) - Controle de Volume Digital

01 [TPA3255](https://www.ti.com/lit/ds/symlink/tpa3255.pdf) - Amplificador 4 canais

01 [VS1838](http://eeshop.unl.edu/pdf/VS1838-Infrared-Receiver-datasheet.pdf) - Receptor IR

01 [VSMY98545ADS](https://www.vishay.com/docs/84390/vsmy98545ads.pdf) - Led Infravermelho 3W

## Esquemático de Montagem utizando o [Fritzing](https://fritzing.org): 
### [Download PI2.fzz](/Design/PI2.fzz?raw=true)

<p align="center">
  <img src="/Design/PI2_schem.png?raw=true" width="70%">
</p>
 
A escolha de GPIO foi direcionada pelo seu comportamentos na inicialização: 
<p align="center">
  <img src="/Design/GPIO_Limitations_ESP8266_NodeMCU.jpg?raw=true" width="70%">
</p>

## Segundo esquemático, com todos os componentes e pinos definidos.
<p align="center">
  <img src="/Design/Circuito_Completo.BMP?raw=true" width="90%">
</p>

Devido á limitação de materiais e fabricação será utilizada placas prefuradas e fiação no lugar de uma placa de circuito impresso.


# Projeto Final SisMic 1/2023

O projeto final consistiu no uso de um ESP32 e DHT22 para enviar dados de umidade e temperatura a partir do MSP430 para o ESP32 e então para o nodeRED do HiveMQ.

## Detalhes

Recomenda-se ler primeiramente o relatório que se encontra no projeto.

## Instalando

É necessário ter instalado:
- MQTT
- HiveMQ
- Arduino IDE
- CodeComposer Studio

## Rodando

Conectar os dispositivos seguindo o exemplo que se encontra no relatório com tudo desconectado da energia para evitar problemas. Depois dos dispositivos devidamente conectados entre si, conectar o MSP430 e o ESP32 via USB ao computador. Subir o ESP32.ino no ESP32 via Arduino e subir o arquivo c via CCStudio.

Para configurar o HiveMQ, é possível usar o seguinte tutorial:
https://curtocircuito.com.br/blog/Categoria%20IoT/esp32:-node-red-editor-de-fluxo-on-line

## Duvidas?

Abrir uma PR com as informações do que já foi feito e o que deu problema. Quanto mais detalhes, melhor. Screenshots e fotos ajudam.


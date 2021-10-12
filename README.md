# CryptoDisplay

Based on project : https://github.com/witnessmenow/simple-arduino-crypto-display

Updated using different hardware and a different api that is still working

    Main Hardware:
    - NodeMCU 1.0 (ESP-12E Module)
    - OLED SPI Display (SH1106)

See ino file for pin setup and other instructions

 Example Payload:
 {
   "data":[
      {
         "id":"bitcoin",
         "rank":"1",
         "symbol":"BTC",
         "name":"Bitcoin",
         "supply":"18672456.0000000000000000",
         "maxSupply":"21000000.0000000000000000",
         "marketCapUsd":"1152624930304.1240977121428560",
         "volumeUsd24Hr":"33833128575.7846446188964901",
         "priceUsd":"61728.6194330367734010",
         "changePercent24Hr":"-2.5350684458562683",
         "vwap24Hr":"61923.3802279466865213",
         "explorer":"https://blockchain.info/"
      }
   ],
   "timestamp":1618602367803
}

[Image of Display](https://github.com/mbooch22/CryptoDisplay/blob/main/image0.jpeg)

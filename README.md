# C++ napojeni na moji MAWSGO lambdu

Velmi osobni. Smyslem je rozhrani na export vysledku z mych simulacnich experimentu, ktere jsou typicky implementovane v dockerech a bezi typicky v nejake virtualizaci, jako napr:
- AWS Lambda
- AWS Batch
- Kubernetes vsech moznych typu

## Architektura

Na mem AWS je RESTAPI/lambda prijimajici vstupni baliky dat/transakce. Podle zadani transakce umisti balik na patricne uloziste:
- soubory do S3
- ...

## Model

## Integrace

Toto pridat do Dockerfile:
```
WORKDIR /src
RUN git clone https://github.com/mhafan/mawscpp.git 
WORKDIR /src/mawscpp
RUN make install
```

Pak se ocekava nejak dodat dve esencialni informace:
- ENV_EGATE_ENDPOINT
- ENV_EGATE_APIKEY

### Zapojeni v programu
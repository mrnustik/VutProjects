## Popis programu

Nástroj isamon je program spustitelný z příkazové řádky používaný ke skenování aktivních hostů v IPv4 sítích a jejich portů.

## Příklad spuštění

```
$ isamon -h # vypíše nápovědu
$ isamon -i eth0 -n 192.168.1.0/24 # provede scanování sítě a zobrazí aktivní klienty za použití rozhraní eth0
$ isamon -n 192.168.1.0/30 # provede scanování sítě a zobrazí aktivní klienty za použití všech rozhraní
$ isamon -n 192.168.1.0/28 -t -p 22 # provede scanování sítě a zobrazí aktivní klienty s otevřeným TCP portem 22 za použití všech rozhraní
$ isamon -n 192.168.1.0/30 -t -u -w 5  # provede scanování sítě a zobrazí aktivní klienty a všechny otevřené TCP a UDP porty za použití všech rozhraní, pokud klient neodpoví do 5ms, isamon bude považovat danný port za uzavřený 
```

## Seznam souborů

 - src/* - zdrojové soubory programu
 - doc/* - dokumentace programu v LaTeXu
 - manual.pdf - programová dokumentace programu
 - README - tento soubor
 - Makefile - pro kompilaci projektu
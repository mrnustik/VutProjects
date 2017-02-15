# Projekt do Principů programovavích jazyků (IPP)


## Téma: JSON2XML.

## Zadání: 

Vytvořte skript pro konverzi JSON formátu (viz RFC 4627) do XML. Každému prvku z JSON formátu (objekt, pole, dvojice jméno-hodnota) bude odpovídat jeden párový element se jménem podle jména dvojice a obsahem podle hodnoty dvojice. Každé pole bude obaleno párovým elementem <array> a každý prvek pole bude obalen párovým elementem <item>.

JSON hodnoty typu string a number a JSON literály true, false a null budou transformovány v závislosti na parametrech skriptu na atribut value daného elementu odpovídající hodnotou (stejného tvaru jako v JSON vstupu; nezapomeňte na požadavek ouvozovkování atributů v XML) nebo na textový element v případě hodnoty typu string a number či na párový element (ve zkráceném zápise) <true/>, <false/> a <null/>. Je-li hodnotou desetinné číslo (případně s exponentem), bude na výstupu celé číslo zaokrouhleno dolů (čísla mimo meze neuvažujte).

---
### Vstupní parametry

* <b>--help</b> provede výpis nápovědy
* <b>--input=filename</b> zadaný vstupní JSON v kódování UTF-8
* <b>--output=filename</b> zadaný výstupní soubor XML v kódování UTF-8
* <b>-h=subst</b> znak používaný pro nahrazení znaků, které by porušovali standard XML (při nezadaném parametru použita pomlčka)
* <b>-n</b> negenerovat na výstupu XML hlavičku
* <b>-r=root-element</b> kořenový element zaobalující výstupní soubor, pokud není zadán výsledek nebude obalen kořenovým elementem. Pokud by řetězec root-element neodpovídal XML standardu program skončí s chybou 50.
* <b>--array-name=array-element</b> přejmenování elementu zaobalujícího pole z defaultního array na array-element. Pokud by array-element neodpovídal standardu program končí s chybou 50.
* <b>--item-name=item-element</b> analogické pro array-element jen zaobaluje prvky pole.
* <b>-s</b> hodnoty typu string budou transformováný na textové elementy místo atributů
* <b>-i</b> hodnoty typu number budou transformovány na textové elementy místo atributů
* <b>-l</b> hodnoty literálů (true, folse, null) budou transformovány na elemnty <true/>, <false/> a <null/> místo na atributy
* <b>-c</b> tento přepínač oproti implicitnímu chování aktivuje překlad problematických znaků. Pro XML problematické znaky s UTF-8 kódem menším jak 128 ve vstupních řetězcových hodnotách (ve dvojicích i polích) konvertujte na odpovídající zápisy v XML pomocí metaznaku & (např. &amp;, &lt;, &gt; atd.). Ostatní problematické znaky konvertovat nemusíte.
* <b>-a | --array-size </b> u pole bude doplněn atribut size, který bude obsahovat počet prvků v poli
* <b>-t | --index-items </b> u prvků pole bude použit atribut index s určením polohy prvku v rámci pole (indexováno od 1)
* <b>--start=n </b> inicializace čítače prvků pole (kombinovat s -t)
* <b>--types</b> obalující elementy skalárních hodnot bude doplněn o atribut type obsahující hodnotu integer pro int, real pro desetinou hodnotu, string pro řetězec a literal pro literály (true, false, null)

---
## Použití testů:
Ve skriptu <code>run_tests.sh</code> změňte proměnné <code>INTERPRETER</code> a <code>SOURCE</code>, tak aby odpovídali vašemu prostředí.

Pokud byste objevili v testech chybu napište mi a já to opravím.

V současnosti skript nekontroluje správnost výsledků, bude přidáno ve chvíli, kdy budu mít na čem testovat.
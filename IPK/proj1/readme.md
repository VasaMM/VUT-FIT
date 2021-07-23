# **Client - Server** aplikace

##### **Autor:** Václav Martinka _(xmarti76)_ FIT VUTBR 19. 03. 2018

Jedná se o dvojici aplikací, které mezi sebou vzájemně komunikuji a umožňují provádět vzáleně základní operace.
Každá operace musí probíhat ve složce (podsložce) nějakého uživatele
Základní informace se zobrazí při použití parametru _--help_

---

### Ftrestd _(server)_
Lze spouštět s následujícími parametry:
  - [-p PORT]
  - [-r ROOT_FOLDER]

Implicitně platí:
  - PORT = 6677
  - ROOT_FOLDER = ./

Server je odolný vůči většině chyb a měl by běžet dál. K ukončení serveru dojde pouze v případě nějaké chyby při jeho spouštění nebo při závažné chybě za běhu.

---

### Ftrestd _(client)_
Prvním parametrem je vždy příkaz pro server:
  - **del** = Smazání souboru
  - **get** = Přečtení souboru ze serveru
  - **put** = Uložení souboru na server
  - **lst** = Výpis obsahu adresáře
  - **mkd** = Vytoření nocé složky
  - **rmd** = Smazaní prádzné složky


Druhým parametrem je cesta k souboru/složce na serveru ve tvaru:
SERVER:PORT/USER/cesta/soubor

  - **SERVER** je adresa serveru (může být i _localhost_)
  - **PORT** je volitelný, není li zadán, vezme se implicitní 6677

Třetí parametr lze použít pouze u _PUT_ (kde je povinný) a _GET_ kde je volitelný. Tento parametr určuje lokální cestu k souboru který se nahraje na server (v případě PUT) nebo kam se uloží soubor ze serveru v případě (GET).

Server odpovídá _OK 200_ v případě, že vše proběhlo v pořádku. V opačném případě odpoví příslušný chybový kód, který client podle kontextu převede na odpovídající chybu.


---

## Známé chyby
  - Není důkladě ošetřen přístup mimo složku uživatele a je možné to obejít.
  - Zřejmě nejsou ošetřeny všechny možné chybové stavy během komunikace
  - Při čtení velkých souborů pomocí _GET_ (do 20MB se tato chyba neprojevila) je soubor od určité části poškozený, zajímavé je, že při _PUT_ se to neděje (testováno na 700MB souboru)

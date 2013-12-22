graf
====

Grafika 2013 Házi feladatok

A BME Számítógépes grafika és képfeldolgozás 2013 őszi kurzusára készült házi feladatok. Mindegyik el lett fogadva. Sikeresen védtem is őket. A fájlokat nem kozmetikáztam, ez a beadott verzió. (A neptun kódot kitöröltem persze.) Ha valamiért nem jó hogy ez itt kint van, és szeretnéd, hogy töröljem, írj egy emailt ide: david.szakallas[AT]gmail.com

Specifikáció
============

Első feladat
------------

Készítsen "Kaksiulotteisia vihaisia lintuja" programot, amelyben egy piros és egy zöld madár szerepel, valamint világos zöld talaj, a talajból kiálló tetszőleges színű kétágú csúzli, amely fekete színű széles gumival fogja körbe a kilövendő piros madarat. A csúzli tára, ahol a piros madár várja a sorsa beteljesülését, a 600x600 felbontású alkalmazói ablak bal felső sarkához képest 200 pixellel jobbra és 400 pixellel lejjebb van.

A madarak teste, szeme és szemgolyója ellipszis (nem kör!), csőrük, farktollaik és szemöldökeik háromszögek. Szárny és bóbita opcionális. A zöld madár fel-le repked akár szárnyak nélkül is, magassága az időben szinuszosan változik az ütközésig. A piros madarat csúzliból lehet kilőni a fizikai törvényeknek megfelelően, a csúzli rugóenergiáját teljes mértékben átveszi a madár induláskor. A kilövés folyamat azzal kezdődik, hogy az egér bal gombjának lenyomásával a madár ellipszis alakú testének belsejébe klikkelünk, amikor a madár a kurzorhoz ragad és követi az kurzor mozgását az egérgomb elengedéséig. A csúzli gumija végig szorosan feszül a madár fenekére. Amikor elengedjük a bal gombot a piros madarat is útjára bocsátjuk, azaz kirepül a csúzliból. Perdülettel és forgással nem kell foglalkozni. A piros madárra reptében a nehézségi erő hat, közegellenállás és felhajtóerő (nincs szárnya, amivel csapkodna) pedig nincs. Ha a piros és a zöld madár ellipszis teste ütközik (figyelem, két ellipszis ütközését pontosan kell számítani), mindketten megállnak a levegőben és a piros madár sárgává változik. Ha a piros madár kirepül a látható tartományból, automatikusan születik egy új a csúzliban fejjel lefelé a két ág "között".

A feladat megoldásához csak az előadáson eddig szerepelt OpenGL függvények használhatók, azaz: glBegin, glEnd, glColor, glVertex, glViewport.

Figyelem: a keret változott, ezért a korábbi évek kereteit nem lehet használni.

Beadási határidő: 2013. 10. 01. 23:59

Második feladat
---------------

Készítsen 2D editort, amivel egy 100m x 100m-es területet lehet berendezni, amelybe egy 50m x 50m-es ablakú kamerával nézünk bele. Az ablakot minden ‘s’ lenyomásra a (10, 20) vektor tolja el. Ha az ablak elérte a terület szélét, akkor visszaugrik a bal alsó sarokba. A program a bal egérgomb lenyomásainak helyét kontrollpontnak, az lenyomás pillanatában az óra állását pedig csomóértéknek (kontrollponthoz rendelt paraméterértéknek) tekinti, így minden bal egérgomb lenyomással egy újabb kontrollpontot vehetünk fel (max 10 kontrollpontra kell a programot felkészíteni). A kontrollpontokra egy fehér Tenziós Catmull-Rom (TCR) spline-t illeszt, ahol a tenzió mindenhol -0.5. A kontrollpontok nyugalmi állapotukban 1m sugarú piros kitöltött körök.

Space lenyomására a program aktivizálódik, a spline-t megduplázza és az új változatot 60 fokkal a kontrollpontok súlypontja (aminek koordinátáit a kontrollpontok koordinátánkénti aritmetikai közepeként számíthatunk) körül elforgatja, majd kék Bézier görbét csinál belőle (mialatt az eredeti TCR marad a helyén). Ezután egy-egy 1 m sugarú sárga kitöltött kör indul el minden görbén, mégpedig a TCR-n a paraméterezésnek megfelelő időzítéssel, azaz annyi idő alatt ér a görbe végére, ameddig a kontrollpontot felvétele tartott. A Bézier-t a saját köre úgy futja be, hogy ugyanakkor érkezzen a végére, mint a TCR-n szaladó társa. A futamok újra kezdődnek. A futás alatt a kontrollpontokat ábrázoló kitöltött körök sugara folyamatosan változik amivel azt mutatják, hogy milyennek kell lennie az ide elhelyezett súly abszolút értékének, hogy a görbe aktuális pontját a kontrollpontok ilyen súlyokkal vett kombinációjaként kapjuk meg (Bézier-nél nyilván a Berstein polinom, TCR-nél érdemes gondolkodni egy kicsit). Ha a súly pozitív, a kör piros. Ha negatív, akkor türkisz kék.

Figyelem: csak közelítőleg C2 folytonos és egzaktul C1 folytonos valódi, nem egyenletes TCR spline elfogadható, pl. Catmull-Rom és felesleges dolgokat tartalmazó Kochanek-Bartels nem. Közérthetőbben: az Internetről másolt spline-ok ellenjavaltak.

Beadási határidő: 2013. 10. 20. 23:59

Harmadik feladat
----------------

Készítsen négyhengeres sugárkövető programot. Az 1. henger tartalmazza a teljes színteret, fala optikailag sima arany, alapja megválasztható színű diffúz, teteje hiányzik, amin a fehér égbolt világít be (ezt úgy lehet figyelembe venni, hogy azok a sugarak, amelyek kilépnek a hengerből ilyen sugársűrűséggel térnek vissza). A többi henger az 1. belsejében van, tengelyük sem egymással, sem bármelyik koordinátatengellyel nem párhuzamos. A 2. anyaga optikailag sima ezüst, a 3. anyaga optikailag sima réz, a 4. anyaga optikailag sima üveg, gyémánt vagy megválasztható színű diffúz lehet. Az homogén égbolton kívül még egy vagy több optikailag sima ezüst forgásparaboloid reflektorú, nem a fókuszba helyezett pontszerű fényforrás világítja meg a teret.

A kamerát úgy kell elhelyezni, hogy a hengerek láthatók legyenek. Fotontérképet nyilván csak a diffúz felületekhez kell rendelni az optikailag sima felületekhez nem. A fotonok számát úgy kell beállítani, hogy a program 1 percen belül 1 CPU magon lefusson (ha ezt az időt túllépi, a fotonszámot érdemes csökkenteni, pl. úgy, hogy ne legyen 10^4-nél lényegesen nagyobb, de a nem lépi túl, akkor lehet több fotonnal is dolgozni). A fotontérkép felbontását pedig úgy kell meghatározni, hogy a kausztika az adott fotonszám mellett sima legyen.

Anyagtulajdonságok:

Optikailag sima anyagok:

...................r.........g.........b

Üveg (n/k)......1.5/0.0, 1.5/0.0, 1.5/0.0

Gyémánt (n/k)...2.4/0.0, 2.4/0.0 2.4/0.0

Arany (n/k).....0.17/3.1, 0.35/2.7, 1.5/1.9

Réz (n/k).......0.2/3.6, 1.1/2.6, 1.2/2.3

Ezüst (n/k).....0.14/4.1, 0.16/2.3, 0.13/3.1

Diffúz anyagok: kd(r,g,b) tetszőlegesen, jó ízléssel megválasztható.

Beadási határidő: 2013. 11. 10. 23:59

Negyedik feladat
----------------

Készítsen OpenGL programot, amely egy (fekete foltos textúrájú), de alapvetően fehér diffúz+spekuláris testű Szalámi Gólyát és zöld diffúz+erősen spekuláris testű (nyálkás) Ellipszoid Békákat jelenít meg diffúz, textúrázott terepen. A Szalámi Gólya onnan ismerszik meg, hogy ha testét a Catmull-Rom spline vázára merőlegesen bárhol kettészeljük, akkor a metszet kör alakú (a váz definíciója: ha a Szalámi Gólya testének minden felületi pontját egyszerre gyújtjuk meg, akkor a tűzfrontok a vázban találkoznak). A kör sugara a váz mentén változik, amelyet egy másik Catmull-Rom spline-nal kell megadni. A gólyának és a békáknak is két-két szemük van, a gólyának még piros kúp alakú csőre és két gólyalába van (amelyek biológiai ismereteink szerint, a mi lábunkkal ellentétben, térdben előre és nem visszafelé hajlíthatók). A gólya arányai olyanok, hogy előrehajolva a csőrének végével elérné a talajt. Az Ellipszoid Békák testrészei ellipszoidok. A terep textúrázott, nem szükségképpen sík. Tereptárgyak tetszés szerint megválaszthatók. A színteret a nap (irányfényforrás) az ég (ambiens fényforrás) és egy energiától duzzadó szentjános bogár (pontfényforrás) világítja meg.

Beadási határidő: 2013. 12. 04. 23:59

ötödik feladat
--------------

Mozgassa meg a 4. házi objektumait. A gólya lépeget előre 1 lépés/sec sebességgel. A támaszkodó láb nem csúszkál (inverz kinematika). A ‘J’ billentyű lenyomására 0.5 sec alatt 10 fokot jobbra fordul célszerűen a támaszkodó láb körül, hasonlóan a ‘B’ billentyű lenyomására balra. A SPACE hatására a nyakát előrehajtva lecsap és ha békatestet talál (végtagok nem számítanak), akkor a béka eltűnik. A gólyát csontváz animációval kell mozgatni, a lábrészek és a nyak hossza nem változhat a mozgás során. A láb részei tekinthetők merev testnek, de a gólyatest nem, így azt a csontvázra "bőrözni" kell. A békák merev testként akár véletlen irányokba ugrálhatnak, de két ugrás között 1 sec rákészülési időre van szükségük. A terepet illetően két lehetőség közül lehet választani. Az elsőben a terep sík és akkor a testeknek van árnyékuk rajta, amit a nap vet (síkra vetített árnyék algoritmus). A másik lehetőség, hogy a terep nem sík.

Beadási határidő: 2013. 12. 16. 11:00

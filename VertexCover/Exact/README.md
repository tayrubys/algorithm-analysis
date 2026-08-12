# Backtracking(Exact) Algoritmasi - Vertex Cover Problemi 

Bu bölümde, Vertex Cover probleminin, her köşe için "seç / seçme" kararının budama (pruning) ile desteklenmiş bir özyineleme (backtracking) yoluyla denenmesine dayanan, **kesin (minimum) sonucu garanti eden** çözümü açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** 2-yaklaşıklı (greedy) çözümün aksine, gerçek **minimum** vertex cover'ı kesin olarak bulmak için geliştirilmiştir. Her köşe için "örtüye dahil et" veya "etme" kararının bütün kombinasyonlarını, geçerli olmayan veya daha kötü olduğu kesinleşen dalları erken keserek dener.
- **Hangi eksikliği gidermektedir?** Greedy 2-yaklaşıklı çözüm, gerçek minimumun en fazla iki katı büyüklüğünde bir örtü bulmayı garanti eder ama **kesin** sonucu garanti etmez. Bu algoritma, bütün köşe kombinasyonlarını (gerekli budamalarla) sistemli şekilde deneyerek bu kesinlik eksikliğini giderir.
- **Önceki yönteme (greedy 2-yaklaşıklığa) göre avantajı nedir?** Sonuç olarak bulunan örtü her zaman **gerçek minimum** boyuttadır; yaklaşık değil, kesin bir çözümdür. Ayrıca `selectedCount + 1 < bestCount` gibi budama kontrolleriyle, o ana kadar bulunan en iyi çözümden zaten daha kötü olacağı belli olan dallar hiç incelenmeden atlanır.
- **Hangi durumda tercih edilir?**
  - Köşe sayısının (`V`) küçük olduğu durumlarda.
  - Minimum vertex cover'ın kesin olarak bulunmasının zorunlu olduğu durumlarda (örneğin küçük ölçekli ağ güvenliği, sensör yerleşimi gibi kesinliğin kritik olduğu senaryolarda).
- **Hangi durumda tercih edilmez?**
  - Köşe sayısının fazla olduğu durumlarda. Problem NP-Hard olduğundan, budama olsa dahi en kötü durumda bütün `2^V` kombinasyonuna yakın bir alan taranabilir; bu da büyük graflarda algoritmayı pratik olmaktan çıkarır.

## Algoritmanın Çalışma Mantığı

Algoritma, köşeleri sırayla (`index = 0`'dan `V - 1`'e kadar) ele alır ve her köşe için iki dal dener:

1. **Taban durum:** Bütün köşeler değerlendirildiğinde (`index == V`), seçilen köşe kümesinin geçerli bir vertex cover olup olmadığı `isVertexCover` fonksiyonuyla kontrol edilir. Eğer geçerliyse ve seçilen köşe sayısı (`selectedCount`), o ana kadar bulunan en iyi sonuçtan (`bestCount`) küçükse, en iyi çözüm (`bestCover`, `bestCount`) güncellenir.
2. **1. Dal — Köşeyi seç:** `index` numaralı köşe örtüye dahil edilir (`selected[index] = true`). Eğer bu seçimle birlikte seçilen köşe sayısı hâlâ mevcut en iyi çözümden azsa (`selectedCount + 1 < bestCount`), özyineleme bir sonraki köşeyle devam eder. Aksi hâlde bu dal, zaten en iyi çözümden daha kötü bir sonuç üreteceği kesinleştiği için **budanır**.
3. **2. Dal — Köşeyi seçme:** `index` numaralı köşe örtüye dahil edilmez (`selected[index] = false`). Aynı budama mantığı (`selectedCount < bestCount`) burada da uygulanır.
4. Her iki dal da bütün köşeler değerlendirilene kadar bu şekilde özyinelemeli olarak denenir; sonunda `bestCover` ve `bestCount`, bulunabilecek en küçük vertex cover'ı temsil eder.

`isVertexCover` fonksiyonu, bir köşe kümesinin geçerliliğini kontrol etmek için grafın bütün kenarlarını dolaşır ve her kenarın en az bir ucunun seçilmiş olup olmadığına bakar; eğer bir kenarın **her iki ucu da** seçilmemişse, o kenar örtülmemiş demektir ve küme geçerli bir vertex cover değildir.

### Örnek

Aşağıdaki graf verilsin (5 köşe, kenarlar):

```text
0-1, 0-2, 1-3, 2-3, 3-4
```

`bestCount` başlangıçta `V + 1 = 6` olarak ayarlanır (herhangi bir gerçek çözümden büyük bir üst sınır). Algoritma köşe `0`'dan başlayarak "seç/seçme" dallarını dener. Örneğin köşe `3`'ü seçmek, `1-3`, `2-3` ve `3-4` kenarlarının üçünü birden örter; bu nedenle `3`'ü seçen dallar genellikle daha az köşeyle geçerli bir örtüye ulaşır. Algoritma bütün dalları (budamalarla elenenler hariç) dener ve sonunda örneğin `{0, 3}` gibi 2 köşelik bir kümenin bütün kenarları örttüğünü bulur; bu, `bestCount = 2` olarak güncellenir ve daha büyük köşe sayılı çözümler bu noktadan sonra budanarak elenir.

> Backtracking, greedy çözümden farklı olarak **bütün** olası köşe kombinasyonlarını (budanmayanları) dener; bu nedenle sonuç her zaman kesin minimumdur, ancak bunun bedeli daha yüksek zaman karmaşıklığıdır.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon IsVertexCover(Secilenler, Kenarlar):
    Döngü (her (u, v) kenarı için Kenarlar içinde):
        Eğer Secilenler[u] == false VE Secilenler[v] == false ise:
            Döndür false   // kenar örtülmemiş
    Döndür true


Fonksiyon Backtrack(index, V, Secilenler, SecilenSayisi):

    // Bütün köşeler değerlendirildi
    Eğer index == V ise:
        Eğer IsVertexCover(Secilenler, Kenarlar) ise:
            Eğer SecilenSayisi < enIyiSayi ise:
                enIyiSayi = SecilenSayisi
                enIyiOrtu = Secilenler
        Döndür

    // 1. Dal: koseyi sec
    Secilenler[index] = true
    Eğer SecilenSayisi + 1 < enIyiSayi ise:
        Backtrack(index + 1, V, Secilenler, SecilenSayisi + 1)

    // 2. Dal: koseyi secme
    Secilenler[index] = false
    Eğer SecilenSayisi < enIyiSayi ise:
        Backtrack(index + 1, V, Secilenler, SecilenSayisi)
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`vector<pair<int, int>> edges`** — Grafın kenarlarını, her biri iki köşe numarasından oluşan `pair<int, int>` çiftleri hâlinde saklayan bir vektördür. Komşuluk listesi yerine kenar listesi kullanılması, `isVertexCover` fonksiyonunun bütün kenarları doğrudan dolaşmasını kolaylaştırır.

* **`vector<bool> selected` / `bestCover`** — `selected`, o anki özyineleme dalında hangi köşelerin seçildiğini tutar. `bestCover`, o ana kadar bulunan en küçük geçerli vertex cover kümesini saklar. `vector<bool>` kullanılması, `bool` dizilerine özgü bellek-verimli (bit düzeyinde sıkıştırılmış) depolama sağlar.

* **Global `bestCount` ve `bestCover` değişkenleri** — Özyinelemeli çağrılar arasında "o ana kadar bulunan en iyi çözümün" paylaşılmasını sağlar; her yeni geçerli ve daha küçük çözüm bulunduğunda bu değişkenler güncellenir.

* **Backtracking (Özyineleme + Geri Alma)** — `backtrack` fonksiyonu her köşe için iki dal (`seç` / `seçme`) dener. `selected[index]` değeri her iki dal denemesi sırasında değiştirilir; bu, aynı diziyi kullanarak farklı kombinasyonların sırayla denenmesini (klasik backtracking mantığını) sağlar.

* **Budama (`if (selectedCount + 1 < bestCount)` ve `if (selectedCount < bestCount)`)** — Algoritmanın saf kombinasyon denemesinden farkı budur: bir dal, seçilen köşe sayısı zaten mevcut en iyi çözüme eşit veya ondan fazlaysa hiç incelenmez. Bu, gereksiz alt ağaçların erken elenmesini sağlar.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak sonucun (minimum vertex cover kümesi ve boyutu) ekrana yazdırılması için kullanılır.
- `<vector>`: Kenarların (`vector<pair<int,int>>`), seçim durumlarının (`vector<bool>`) ve en iyi çözümün saklanması için kullanılır.

---

## Zaman Karmaşıklığı

Her köşe için iki dal (`seç` / `seçme`) denendiğinden, budama olmasaydı algoritma tam bir ikili karar ağacını gezerdi:

```text
O(2^V)
```

Ayrıca her yaprak düğümde (`index == V`), geçerliliği kontrol etmek için bütün kenarlar (`E` adet) dolaşılır; bu da her yaprak için ek `O(E)` maliyeti ekler.

### Best Case

En iyi durumda, budama koşulları (`selectedCount + 1 < bestCount` ve `selectedCount < bestCount`) erken devreye girerek karar ağacının büyük bir kısmını keser; özellikle `bestCount` küçük bir değere erken ulaşırsa, sonraki birçok dal hiç incelenmeden elenir.

### Worst Case — O(2^V × E)

En kötü durumda (budamanın etkili olamadığı graf yapılarında), algoritma karar ağacının neredeyse tamamını gezer ve her yaprakta kenar kontrolü yapar. Bu durumda zaman karmaşıklığı:

```text
O(2^V × E)
```

olarak ifade edilir.

### Average Case

Ortalama durumda budamanın etkinliği grafın yapısına (kenar yoğunluğuna, köşelerin birbirine bağlanma biçimine) bağlıdır; kesin bir formül verilemez, ancak pratikte budama sayesinde tam `2^V`'den önemli ölçüde daha az dal incelenir.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(V + E)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

`edges` vektörü, grafın kenarlarını saklamak için `O(E)` bellek kullanır.

`selected` ve `bestCover` vektörleri, her biri `V` elemanlı olduğundan `O(V)` bellek kullanır.

Özyinelemeli çağrıların oluşturduğu fonksiyon çağrı yığını (call stack), en fazla `V` derinliğine ulaşabilir; bu da `O(V)` ek bellek gerektirir.

Bu nedenle algoritmanın toplam alan karmaşıklığı:

```text
O(V + E)
```

olur.
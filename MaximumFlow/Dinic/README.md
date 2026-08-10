
# Dinic Algoritması — Maximum Flow

Bu bölümde bir akış ağında (flow network) kaynak (source) düğümden hedef (sink) düğüme gönderilebilecek maksimum akış miktarını bulan **Dinic Algoritması** açıklanmaktadır. Dinic, Ford-Fulkerson ve Edmonds-Karp algoritmalarından farklı olarak, BFS ve DFS'i aynı anda (katmanlı bir yapıda) kullanarak çok daha yüksek performans sağlayan modern bir yöntemdir.

## Algoritmanın Geliştirilme Amacı

* **Bu algoritma neden geliştirilmiştir?** Ağlardaki maksimum akış problemini (taşıma, trafik, eşleştirme) daha büyük ölçekli verilerde, çok daha kısa sürede çözebilmek için geliştirilmiştir.
* **Hangi eksikliği gidermektedir?** Ford-Fulkerson, kapasiteler çok büyük olduğunda yavaşlar (pseudo-polinomsal). Edmonds-Karp ise kapasite bağımlılığını çözer ancak her seferinde sadece tek bir yol bulup tüm sistemi baştan taradığı için gereksiz zaman kaybeder. Dinic algoritması, ağ üzerinde bir kerede birden fazla yolu aynı anda bularak bu israfı ortadan kaldırır.
* **Önceki yöntemlere göre avantajı nedir?** İki fazlı (BFS + DFS) yapısı sayesinde aynı uzunluktaki *bütün* artırıcı yolları tek bir geçişte bulur. Zaman karmaşıklığı $O(V^2 \cdot E)$ seviyesine düşerek yoğun (çok kenarlı) graflarda eski yöntemlere kıyasla dramatik bir hız artışı sağlar.
* **Hangi durumda tercih edilir?**
* Büyük ölçekli ve yoğun graflarda maksimum akış arandığında.
* Performansın ve hızın kritik olduğu yarışma programlaması (competitive programming) senaryolarında.
* Bipartite eşleştirme (Bipartite Matching) problemlerinde (Dinic'in özel yapısı bu problemleri $O(E \sqrt{V})$ sürede çözer).


* **Hangi durumda tercih edilmez?**
* Çok küçük graflarda, kod karmaşıklığına (BFS ve DFS'in iç içe kullanımına) değmeyebilir; bu durumlarda Edmonds-Karp yeterli olabilir.



## Algoritmanın Çalışma Mantığı

Dinic algoritması iki ana evre (faz) halinde tekrar tekrar çalışır.

1. **Seviye Grafı (Level Graph) Oluşturma (BFS ile):** Artık ağ (residual graph) üzerinde sadece boş kapasitesi olan kenarlar kullanılarak düğümlere kaynaktan uzaklıklarına göre bir "seviye" (katman) değeri atanır.
2. **Bloklama Akışı (Blocking Flow) Gönderme (DFS ile):** Oluşturulan seviye grafiği üzerinde DFS ile yollar aranır. Kritik kural: **DFS sadece bir üst seviyedeki düğüme (ileriye) gidebilir**. Geriye veya aynı seviyeye dönmek yasaktır. Bu evre, o seviye grafiği üzerindeki tüm yollar tıkanana (bloklanana) kadar defalarca çalışır.

Her adımda:

1. `BFS` çalıştırılarak düğümlere `level` (seviye) değerleri atanır.
2. Eğer BFS hedef düğüme (sink) ulaşamazsa algoritma biter.
3. Hedefe ulaşıldıysa, bir `start` dizisi (DFS'in daha önce denediği "ölü" (çıkmaz) yolları tekrar denemesini engelleyen bir hızlandırıcı) sıfırlanır.
4. `DFS` (koddaki `sendFlow` fonksiyonu) çalıştırılarak hedefe akış gönderilir. Bulunan darboğaz (`temp_flow`) kadar kapasiteler güncellenir (ileri yön azalır, ters yön artar).
5. DFS sıfır (0) dönene kadar çalıştırılmaya devam edilir; dönen tüm akışlar toplam akışa eklenir. Bloklama akışı bitince 1. adıma dönülür ve tamamen yeni bir seviye grafı oluşturulur.

### Örnek

Koddaki ağ (`0` kaynak ve `5` hedef) ele alındığında, Dinic'in farkı şurada ortaya çıkar:
Edmonds-Karp aynı uzunlukta iki farklı yol olduğunda bunları iki ayrı BFS taraması ile bulurken, Dinic ilk BFS taramasında bir seviye grafı çizer ve hemen ardından çalıştırdığı DFS ile *ikisini de tek fazda* bulur ve kapasiteleri günceller.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon BFS(Kaynak, Hedef):
    Tüm dugumlerin Seviyesini -1 yap
    Seviye[Kaynak] = 0
    Kuyruk = [Kaynak]

    Döngü (Kuyruk boş değilken):
        u = Kuyruk.cikar()
        Döngü (u'nun tüm komşuları v için):
            Eğer Seviye[v] < 0 VE Kenar.Kapasite > Kenar.Akış ise:
                Seviye[v] = Seviye[u] + 1
                Kuyruk.ekle(v)

    Döndür (Seviye[Hedef] >= 0) // Hedefe ulaşıldı mı?


Fonksiyon SendFlow(u, MevcutAkış, Hedef, StartDizisi):
    Eğer u == Hedef ise Döndür MevcutAkış

    Döngü (StartDizisi[u]'dan başlayarak tüm komşular v için; StartDizisi[u]++):
        Eğer Seviye[v] == Seviye[u] + 1 VE Kenar.Kapasite > Kenar.Akış ise:
            YolDarboğazı = min(MevcutAkış, Kenar.Kapasite - Kenar.Akış)
            TempAkış = SendFlow(v, YolDarboğazı, Hedef, StartDizisi)

            Eğer TempAkış > 0 ise:
                Kenar.Akış += TempAkış
                TersKenar.Akış -= TempAkış
                Döndür TempAkış

    Döndür 0 // Yol tıkalı


Fonksiyon Dinic(Kaynak, Hedef):
    ToplamAkış = 0

    Döngü (BFS(Kaynak, Hedef) == True olduğu sürece):
        StartDizisi = tüm düğümler için 0

        Döngü (Akış = SendFlow(Kaynak, Sonsuz, Hedef, StartDizisi); Akış > 0 olduğu sürece):
            ToplamAkış += Akış

    Döndür ToplamAkış

```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`struct Edge`** — Kenarları temsil eden yapı. `v` (hedef düğüm), `flow` (mevcut akış), `C` (maksimum kapasite) ve `rev` (ters kenarın indeksi) bilgilerini tutar.
* **`vector<vector<Edge>> adj`** — Grafı tutan dinamik komşuluk listesi (Adjacency List). Matris yerine liste kullanılması bellek tasarrufu ve DFS/BFS döngülerinde komşulara hızlı erişim sağlar.
* **`vector<int> level`** — BFS tarafından oluşturulan seviye grafını tutar. Düğümlerin kaynağa olan uzaklığını (adım sayısını) belirtir. Ziyaret dizisi (`visited`) görevi de görür (değer `< 0` ise ziyaret edilmemiştir).
* **`vector<int> start`** — DFS'in (`sendFlow`) en büyük optimizasyonudur. Bir düğümün komşularını tararken hedefe ulaşamayan "ölü/tıkanmış" yolları hatırlar. Sonraki DFS çağrılarında döngünün sıfırdan değil, kaldığı yerden (`start[u]`) devam etmesini sağlayarak tekrarlı işlemleri önler.
* **`queue<int> q`** — BFS için seviye oluşturma aşamasında kullanılan kuyruk.

---

## Kullanılan Kütüphaneler

* `<iostream>`: Konsol giriş/çıkış işlemleri (`cout`) için.
* `<vector>`: Graf, seviye ve start dizilerini tutan dinamik bellek yapıları (`std::vector`) için. Dinamik boyutlandırma sağlayarak eski tarz C pointer (`new`/`delete`) kalabalığını önler.
* `<queue>`: BFS algoritmasında düğümleri işlemek için (`std::queue`).
* `<climits>`: Başlangıç akış değerini (`INT_MAX`) belirlemek için.

---

## Zaman Karmaşıklığı

Dinic algoritmasının asıl gücü zaman karmaşıklığındadır. Genel (en kötü durum) zaman karmaşıklığı şudur:

```text
O(V² × E)

```

(Burada V: Düğüm sayısı, E: Kenar sayısıdır).

### Neden Edmonds-Karp'tan ( $O(V \cdot E^2)$ ) Daha Hızlı?

Edmonds-Karp her seferinde bir yol bulmak için tüm grafı tararken ($O(E)$), Dinic bir kere BFS ile katman oluşturur ve bu katman üzerinde aynı uzunluktaki *tüm* yolları tek seferde (bloklama akışı ile) bulur. Özellikle kenar sayısının ($E$) yüksek olduğu yoğun graflarda Dinic çok daha avantajlıdır.

### Best Case

Düğüm ve kenarların çok düzenli olduğu, bloklama akışlarının çok hızlı dolduğu ağlarda algoritma anında sonlanır. Bipartite (İki parçalı) graflarda eşleştirme problemi çözerken zaman karmaşıklığı mucizevi bir şekilde **$O(E \sqrt{V})$** değerine düşer.

### Worst Case

Tüm yolların en uzun ve en kötü şekilde dağıldığı senaryoda bile en fazla $V$ adet seviye grafı (faz) oluşturulur. Her fazdaki bloklama akışını bulmak en fazla $O(V \cdot E)$ zaman alır. Bu nedenle en kötü durum kesin olarak **$O(V^2 \cdot E)$** ile sınırlıdır.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(V + E)**

Algoritma komşuluk matrisi (adjacency matrix) yerine dinamik boyutlu **komşuluk listesi (adjacency list)** kullanır.

* `adj` (Komşuluk listesi): Her kenar ve ters kenarı depolandığı için `O(E)` alan kullanır.
* `level` dizisi: `O(V)` alan kullanır.
* `start` dizisi: Her fazda yeniden oluşturulur, `O(V)` alan kullanır.
* `queue`: En fazla `O(V)` eleman tutar.
* DFS Çağrı Yığını (Call Stack): Derinlik en fazla `V` olabileceği için `O(V)` alan kullanır.

Baskın terimler toplandığında alan karmaşıklığı:

```text
O(V + E)

```

olur. Bu durum, $O(V^2)$ alan kullanan matris tabanlı Ford-Fulkerson ve Edmonds-Karp uygulamalarına göre çok daha bellek dostudur. Büyük düğüm sayılı graflarda bile bellek taşması (Out of Memory) yaşatmaz.
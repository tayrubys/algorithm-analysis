# Jarvis March Algoritması — Convex Hull

Bu bölümde, düzlemdeki bir nokta kümesinin dış bükey zarfını (convex hull) bulmak için kullanılan **Jarvis March** (diğer adıyla **Gift Wrapping**) yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Bir nokta kümesinin dış bükey zarfını, bütün nokta üçlülerini deneyerek bulmanın yarattığı yüksek zaman maliyetini azaltmak; bunun yerine zarfın en soldaki noktasından başlayarak, her adımda geriye kalan noktaların en dışında kalanını bularak zarfı adım adım "sarmak" için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Kaba kuvvet (brute force) yaklaşımında her nokta çifti/üçlüsü için diğer bütün noktaların hangi tarafta kaldığının kontrol edilmesi gerekir; bu da nokta sayısı arttıkça ciddi bir performans kaybına yol açar. Jarvis March, yalnızca zarfın üzerindeki noktaları ziyaret ederek bu gereksiz kontrolleri azaltır.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı anlaşılırlığı ve uygulama kolaylığıdır. Sadece üç nokta arasındaki dönüş yönünü (orientation) hesaplayan basit bir fonksiyona dayanır; karmaşık sıralama veya yığın (stack) tabanlı yapılar gerektirmez. Ayrıca zarf üzerindeki nokta sayısı (h) az olduğunda, nokta sayısına göre sıralama yapan yöntemlere kıyasla oldukça verimli çalışabilir.
- **Hangi durumda tercih edilir?**
  - Nokta sayısına (n) kıyasla dış bükey zarf üzerindeki nokta sayısının (h) az olduğu durumlarda.
  - Basit ve kolay anlaşılır bir uygulama isteniyorsa.
  - Eğitim amaçlı olarak convex hull mantığının adım adım gösterilmesi gerekiyorsa.
- **Hangi durumda tercih edilmez?**
  - Dış bükey zarf üzerindeki nokta sayısının (h), toplam nokta sayısına (n) yakın olduğu durumlarda (Örn: noktaların çoğu bir çember üzerinde dizilmişse). Bu durumda algoritmanın karmaşıklığı `O(n * h)` ifadesinde `h`, `n`'e yaklaştığı için performans kötüleşir ve `Graham Scan` gibi `O(n log n)` çalışan yöntemler daha avantajlı hale gelir.

## Algoritmanın Çalışma Mantığı

Algoritma, noktalardan en soldakini (x koordinatı en küçük olanı) başlangıç noktası olarak seçer. Bu nokta kesinlikle dış bükey zarfın bir parçasıdır, çünkü ondan daha solda hiçbir nokta yoktur.

Her adımda:

1. Mevcut nokta zarf listesine eklenir.
2. Bir sonraki aday nokta olarak rastgele bir nokta (örneğin sıradaki nokta) seçilir.
3. Diğer bütün noktalar tek tek kontrol edilir.
4. Eğer kontrol edilen nokta, mevcut nokta ile aday nokta arasındaki doğrunun daha dışında kalıyorsa (saat yönünün tersine dönüş varsa), aday nokta bu yeni nokta ile değiştirilir.
5. Bütün noktalar kontrol edildikten sonra bulunan en dış nokta, zarfın bir sonraki noktası olarak kabul edilir ve mevcut nokta olarak güncellenir.
6. Bu işlem, başlangıç noktasına tekrar dönülene kadar tekrarlanır.
7. Başlangıç noktasına dönüldüğünde zarf tamamlanmış olur ve zarf üzerindeki noktalar sırasıyla listelenir.

Dönüş yönü, üç nokta arasındaki `orientation` fonksiyonu ile belirlenir: sonuç `0` ise noktalar aynı doğru üzerindedir, `1` ise saat yönünde, `2` ise saat yönünün tersinedir. Jarvis March, her adımda saat yönünün tersinde kalan en uç noktayı arar.

### Örnek

Aşağıdaki noktalar verilsin:

```text
(0,0), (1,1), (2,2), (2,0), (1,2), (0,2)
```

Algoritma önce en soldaki noktayı bulur: `(0,0)`. Buradan başlayarak, her adımda kalan noktalar arasından en dışta kalan nokta seçilir ve zarfa eklenir. Bu noktalar arasında `(1,1)` gibi iç noktalar zarfa dahil edilmez, çünkü diğer noktalara göre daha içeride kalır. İşlem, başlangıç noktası olan `(0,0)`'a tekrar ulaşılana kadar sürer ve sonuçta zarfı oluşturan dış noktalar sırasıyla elde edilir.

> Jarvis March her adımda **bütün** noktaları tarayarak bir sonraki zarf noktasını bulur. Bu nedenle zarf üzerindeki nokta sayısı arttıkça yapılan tarama sayısı da artar.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon JarvisMarch(Noktalar, NoktaSayisi):

    // Convex hull noktalarini tutacak liste
    Hull = boş liste

    // En soldaki noktayi bul
    l = 0
    Döngü (i = 1'den NoktaSayisi - 1'e kadar):
        Eğer Noktalar[i].x < Noktalar[l].x ise:
            l = i

    p = l

    Döngü:
        // Mevcut noktayi hull'a ekle
        Hull.ekle(Noktalar[p])

        // Aday sonraki nokta olarak p + 1'i sec
        q = (p + 1) mod NoktaSayisi

        // Diger butun noktalari kontrol et
        Döngü (i = 0'dan NoktaSayisi - 1'e kadar):
            Eğer Orientation(Noktalar[p], Noktalar[i], Noktalar[q]) == SaatYonununTersi ise:
                q = i

        // Bir sonraki zarf noktasina gec
        p = q

    Döngü Kadar (p == l)

    Döndür Hull
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`struct Point`** — Bir noktanın `x` ve `y` koordinatlarını bir arada tutar. Noktaları ayrı ayrı iki değişken yerine tek bir yapı içinde tutmak, kodun okunabilirliğini ve taşınabilirliğini artırır.

* **`vector<Point> points`** — Girdi olarak verilen bütün noktaları saklar. Boyutu programın başında belirlenip dinamik olarak yönetildiği için sabit boyutlu bir dizi yerine `vector` kullanılmıştır.

* **`vector<Point> hull`** — Dış bükey zarfı oluşturan noktaları, bulunuş sırasına göre saklar. En kötü durumda bütün noktalar zarfın parçası olabileceğinden boyutu baştan sabitlenmez, ihtiyaç oldukça büyür.

* **`orientation(Point p, Point q, Point r)`** — Üç noktanın birbirine göre dönüş yönünü (saat yönü, saat yönünün tersi veya aynı doğru üzerinde) hesaplayan yardımcı fonksiyondur. Algoritmanın "en dışta kalan noktayı bulma" mantığının temelini oluşturur.

* **`do...while` döngüsü** — Başlangıç noktasından başlanıp tekrar aynı noktaya dönülene kadar çalışması gerektiği için, en az bir kez çalışması garanti edilen `do...while` yapısı tercih edilmiştir.

* **`%` (mod) operatörü** — `(p + 1) % n` ifadesiyle, dizinin sonuna gelindiğinde başa dönülmesi sağlanır; noktalar arasında dairesel bir gezinme mantığı kurar.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak zarf üzerindeki noktaların ekrana yazdırılması için kullanılır.
- `<vector>`: Girdi noktalarının ve hesaplanan zarf noktalarının saklanması için kullanılır.

---

## Zaman Karmaşıklığı

Zarf üzerindeki her nokta için, kalan bütün noktalar taranarak bir sonraki zarf noktası bulunur:

```cpp
for (int i = 0; i < n; i++)
```

Bu iç tarama, `do...while` döngüsü içinde zarf üzerindeki her nokta için tekrarlanır. Zarf üzerindeki toplam nokta sayısı `h` olmak üzere, toplam zaman karmaşıklığı:

```text
O(N × H)
```

şeklinde ifade edilir.

### Best Case — O(N)

En iyi durumda dış bükey zarf yalnızca çok az sayıda noktadan oluşur (örneğin `h` sabit ve küçük bir değerse, ya da zarf üçgen gibi minimum sayıda noktadan oluşuyorsa). Bu durumda dış döngü sabit sayıda çalışır ve toplam karmaşıklık noktaların tek tur taranmasına yakınsar:

```text
O(N)
```

### Average Case — O(N × H)

Ortalama durumda zarf üzerindeki nokta sayısı, toplam nokta sayısından bağımsız, veri dağılımına bağlı bir değerdir. Rastgele dağılmış noktalarda `h` genellikle `n`'den küçük kalır, bu nedenle ortalama durum karmaşıklığı genel olarak:

```text
O(N × H)
```

şeklinde kabul edilir.

### Worst Case — O(N²)

En kötü durumda, örneğin bütün noktalar bir çember üzerine dizildiğinde, noktaların tamamı dış bükey zarfın parçası olur (`h = n`). Bu durumda:

```text
O(N × H) = O(N × N) = O(N²)
```

olur.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada dış bükey zarfı oluşturan noktaları saklamak için bir vektör kullanılmıştır:

```cpp
vector<Point> hull;
```

En kötü durumda (`h = n`) bütün noktalar zarfın bir parçası olabileceğinden, bu vektörün en fazla `N` eleman tutması gerekebilir.

`l`, `p`, `q` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Noktaları tutan `points` vektörü girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bu nedenle algoritmanın ek alan karmaşıklığı:

```text
O(N)
```

olur.
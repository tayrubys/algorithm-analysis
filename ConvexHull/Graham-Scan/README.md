# Graham Scan Algoritması — Convex Hull

Bu bölümde, düzlemdeki bir nokta kümesinin dış bükey zarfını (convex hull) bulmak için kullanılan **Graham Scan** yöntemi açıklanmaktadır. Yöntem, noktaları önce bir referans noktaya göre açısal olarak sıralar, ardından yığın (stack) mantığıyla zarfı oluşturur.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Convex hull problemini, bütün nokta üçlülerinin denenmesi gibi yüksek maliyetli yaklaşımlar yerine, noktaları önce açısal olarak sıralayıp ardından tek bir geçişte zarfı oluşturarak daha verimli çözmek için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Jarvis March gibi yöntemlerde zarf üzerindeki her nokta için bütün noktaların yeniden taranması gerekir; bu da zarf üzerindeki nokta sayısı arttıkça performansı düşürür. Graham Scan, noktaları bir kez sıraladıktan sonra her noktayı yalnızca sabit sayıda işleme tabi tutarak bu tekrar taramayı ortadan kaldırır.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı, zarf üzerindeki nokta sayısından (h) bağımsız olarak her koşulda `O(N log N)` ile çalışmasıdır. Sıralama sonrası zarf oluşturma adımı doğrusal zamanda tamamlanır; bu da onu büyük nokta kümeleri için Jarvis March gibi `O(N × H)` çalışan yöntemlere kıyasla daha öngörülebilir ve genellikle daha hızlı kılar.
- **Hangi durumda tercih edilir?**
  - Nokta sayısının fazla olduğu ve zarf üzerindeki nokta sayısının da yüksek olabileceği durumlarda.
  - Öngörülebilir ve garanti edilmiş `O(N log N)` performansına ihtiyaç duyulduğunda.
  - Noktaların tamamı önceden biliniyorsa (yani çevrim dışı / offline bir hesaplama söz konusuysa).
- **Hangi durumda tercih edilmez?**
  - Noktalar sisteme tek tek, sırayla geldiği ve zarfın anlık olarak güncellenmesi gereken çevrim içi (online) sistemlerde; çünkü algoritma çalışmaya başlamadan önce bütün noktaların sıralanması gerekir.
  - Nokta sayısının çok az olduğu ve ek sıralama maliyetinin gereksiz kaldığı basit durumlarda; bu gibi durumlarda daha basit yöntemler yeterli olabilir.

## Algoritmanın Çalışma Mantığı

Algoritma önce en küçük `y` koordinatına sahip noktayı (eşitlik durumunda en küçük `x` koordinatına sahip olanı) referans nokta (`p0`) olarak seçer. Bu nokta kesinlikle dış bükey zarfın bir parçasıdır.

Ardından bütün noktalar, `p0`'a göre yaptıkları polar açıya göre saat yönünün tersine sıralanır. Açılar eşitse, `p0`'a daha yakın olan nokta önce gelir.

Sıralama tamamlandıktan sonra:

1. Sıralanmış noktalar sırayla bir yığına (`st`) eklenir.
2. Her yeni nokta eklenmeden önce, yığının son iki elemanı ile yeni nokta arasındaki dönüş yönüne bakılır.
3. Eğer son iki nokta ile yeni nokta saat yönünde bir dönüş oluşturuyorsa veya aynı doğru üzerindeyse, yığının en üstündeki nokta çıkarılır; çünkü bu nokta dış bükey zarfın bir parçası olamaz.
4. Bu kontrol, yığındaki son iki nokta ile yeni nokta saat yönünün tersine bir dönüş oluşturana kadar tekrarlanır.
5. Ardından yeni nokta yığına eklenir.
6. Bütün noktalar işlendiğinde, yığında kalan noktalar dış bükey zarfı oluşturur.

### Örnek

Aşağıdaki nokta kümesi verilsin:

```text
(0,0), (1,-4), (-1,-5), (-5,-3), (-3,-1),
(-1,-3), (-2,-2), (-1,-1), (-2,-1), (-1,1)
```

Algoritma önce en küçük `y` değerine sahip noktayı bulur: `(-1,-5)`. Bu nokta referans (`p0`) olarak seçilir. Kalan bütün noktalar `p0`'a göre polar açılarına göre sıralanır. Sıralama sonrası noktalar tek tek yığına eklenirken, saat yönünde dönüşe veya aynı doğru üzerinde kalmaya neden olan iç noktalar (örneğin `(-2,-2)`, `(-1,-3)` gibi diğer noktaların gerisinde kalanlar) yığından çıkarılır. Sonuçta yığında kalan noktalar, kümenin dış bükey zarfını oluşturur.

> Graham Scan, Jarvis March'ın aksine noktaları tekrar tekrar taramaz. Bunun yerine bir kez sıralama yapar ve ardından her noktayı yığına yalnızca bir kez ekleyip en fazla bir kez çıkararak zarfı oluşturur.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon GrahamScan(Noktalar, NoktaSayisi):

    Eğer NoktaSayisi < 3 ise:
        Hata döndür

    // En küçük y değerine (esitlikte en küçük x değerine) sahip noktayi bul
    p0 = Noktalar içindeki en alt-sol nokta

    // Noktalari p0'a göre polar açilarina göre sirala
    // Açilar esitse p0'a daha yakin olan önce gelir
    Sırala(Noktalar, p0'a göre açı ve uzaklık)

    // Convex hull noktalarini tutacak yigin
    Yigin = boş liste

    Döngü (i = 0'dan NoktaSayisi - 1'e kadar):

        // Son iki nokta ile yeni nokta saat yönünde veya ayni dogruysa
        Döngü (Yigin.boyut > 1 VE
               Orientation(Yigin[son-1], Yigin[son], Noktalar[i]) >= 0):
            Yigin'den son elemani çikar

        // Yeni noktayi yigina ekle
        Yigin.ekle(Noktalar[i])

    Eğer Yigin.boyut < 3 ise:
        Hata döndür

    Döndür Yigin
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`struct Point`** — Bir noktanın `x` ve `y` koordinatlarını `double` tipinde tutar. Ondalıklı koordinatların desteklenmesi, hesaplamalarda hassasiyet kaybını önler. Yapı içinde tanımlanan `operator==`, iki noktanın koordinat bazında eşitliğini karşılaştırmak için kullanılır.

* **`orientation(Point a, Point b, Point c)`** — Üç noktanın oluşturduğu dönüş yönünü çapraz çarpım (cross product) mantığıyla hesaplar; sonucun işaretine göre `-1` (saat yönü), `+1` (saat yönünün tersi) veya `0` (aynı doğru üzerinde) döndürür. Hem sıralama hem de yığın güncelleme adımlarının temelini oluşturur.

* **`distSq(Point a, Point b)`** — İki nokta arasındaki mesafenin karesini hesaplar. Karekök alma işleminden kaçınılarak gereksiz işlem maliyeti önlenir; sıralamada yalnızca mesafelerin karşılaştırılması yeterlidir.

* **`min_element` ve lambda ifadesi** — `<algorithm>` kütüphanesinden gelen `min_element`, `p0` referans noktasını bulmak için kullanılır. Lambda içinde `make_pair(y, x)` karşılaştırması yapılarak önce `y`, eşitlikte `x` değerine göre en küçük nokta seçilir.

* **`sort` ve lambda ifadesi** — Noktaları `p0`'a göre polar açı ve mesafeye göre sıralamak için kullanılır. `orientation` sonucu `0` ise (aynı doğru üzerindeyse) `distSq` ile mesafe karşılaştırması yapılır; aksi hâlde dönüş yönüne göre sıralanır.

* **`vector<Point> st`** — Dış bükey zarf üzerindeki noktaları bir yığın (stack) gibi kullanarak saklar. Yeni nokta eklenmeden önce uygun olmayan noktaların çıkarılması, `push_back` ve `pop_back` işlemleriyle gerçekleştirilir.

* **`while` döngüsü içinde `pop_back`** — Yığının son iki noktası ile eklenmek istenen yeni nokta arasında saat yönünde veya doğrusal bir ilişki varsa, yığının en üstündeki nokta zarftan çıkarılır. Bu, algoritmanın zarfa ait olmayan noktaları elemesini sağlayan temel mekanizmadır.

* **`vector<vector<int>>`** — Fonksiyonun girdi ve çıktısı, koordinatları tam sayı çiftleri olarak temsil eden iç içe vektörler biçimindedir. Hata durumunda (`n < 3` veya sonuçta 3'ten az nokta kalması) `{{-1}}` özel değeri döndürülerek hata bildirimi yapılır.

---

## Kullanılan Kütüphaneler

```cpp
#include <bits/stdc++.h>
```

Bu başlık, `<iostream>`, `<vector>`, `<algorithm>` ve `<cmath>` gibi birçok standart C++ kütüphanesini tek seferde programa dahil eder:

- `<iostream>`: `cout` ile sonuç noktalarının ekrana yazdırılması için kullanılır.
- `<vector>`: Girdi noktalarının, sıralanmış noktaların ve zarf yığınının saklanması için kullanılır.
- `<algorithm>`: `min_element` ve `sort` fonksiyonları için kullanılır.

`<bits/stdc++.h>` standart bir C++ başlık dosyası olmadığı için bazı derleyiciler tarafından desteklenmeyebilir; taşınabilirlik gerektiren projelerde ilgili başlıkların ayrı ayrı eklenmesi önerilir.

---

## Zaman Karmaşıklığı

Algoritmanın en maliyetli adımı, noktaların polar açıya göre sıralanmasıdır:

```cpp
sort(a.begin(), a.end(), ...);
```

Bu sıralama işlemi:

```text
O(N log N)
```

zaman alır.

Sıralama sonrasında, her nokta yığına en fazla bir kez eklenir ve en fazla bir kez çıkarılır (amortize analiz). Bu nedenle zarf oluşturma adımı:

```text
O(N)
```

zaman alır.

Toplamda sıralama adımı baskın olduğu için genel zaman karmaşıklığı:

```text
O(N log N)
```

olur ve bu karmaşıklık en iyi, ortalama ve en kötü durumda değişmez.

### Best / Average / Worst Case — O(N log N)

Jarvis March'tan farklı olarak Graham Scan'in performansı, dış bükey zarf üzerindeki nokta sayısından (h) bağımsızdır. Sıralama işlemi her koşulda `O(N log N)` sürdüğü ve zarf oluşturma adımı her koşulda `O(N)` sürdüğü için, algoritmanın en iyi, ortalama ve en kötü durum karmaşıklığı aynıdır:

```text
O(N log N)
```

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada girdi noktalarının `Point` yapısına dönüştürülmüş hâli için bir vektör kullanılır:

```cpp
vector<Point> a;
```

Bu vektör en fazla `N` eleman tutar.

Dış bükey zarfı oluşturan noktaları saklamak için ayrı bir yığın vektörü kullanılır:

```cpp
vector<Point> st;
```

En kötü durumda (bütün noktalar zarfın parçası olduğunda) bu vektör de en fazla `N` eleman tutabilir.

`p0`, `i` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Girdi olarak verilen `points` vektörü girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bu nedenle algoritmanın ek alan karmaşıklığı:

```text
O(N)
```

olur.
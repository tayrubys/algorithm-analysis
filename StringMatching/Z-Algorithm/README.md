# Z Algoritması — String Matching

Bu bölümde String Matching (Dizi Eşleştirme) probleminin, örüntü (pattern) ile metni özel bir ayraç karakteriyle birleştirip **Z dizisi** üzerinden çözülmesini sağlayan **Z Algoritması** açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Naive algoritmanın her konumda örüntüyü baştan karşılaştırma zorunluluğunu ve Rabin-Karp'ın hash çakışması (spurious hit) riskini ortadan kaldırarak, doğrusal (lineer) zamanda garantili bir dizi eşleştirme çözümü sunmak için geliştirilmiştir.
- **Hangi eksikliği gidermektedir?** Naive algoritmada aynı karakterlerin farklı pencerelerde tekrar tekrar karşılaştırılması sorununu giderir. Z dizisi, bir kez hesaplandıktan sonra, önceden hesaplanmış bilgiyi (`z-box`) yeniden kullanarak gereksiz karşılaştırmaları önler.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı, en kötü durumda dahi **kesin olarak** `O(N + M)` zaman karmaşıklığı garanti etmesidir. Rabin-Karp'ın aksine hash çakışmasına bağlı performans düşüşü yaşanmaz; Naive algoritmanın aksine karakterler tekrar tekrar karşılaştırılmaz.
- **Hangi durumda tercih edilir?**
  - Kesin olarak doğrusal zamanlı (lineer) bir çözüme ihtiyaç duyulan durumlarda.
  - Metin ve örüntünün büyük olduğu, performans garantisinin (worst-case dahil) önemli olduğu durumlarda.
  - Örüntünün metin içindeki tüm eşleşme konumlarının bulunması gereken durumlarda.
- **Hangi durumda tercih edilmez?**
  - Örüntü ile metnin birleştirilmesi (`pattern + '$' + text`) nedeniyle ek bellek (`O(N + M)`) kullanımının kısıtlı olduğu, bellek açısından çok sınırlı ortamlarda.
  - Ayraç karakterinin (`$`) hem örüntüde hem metinde asla geçmediğinden emin olunması gereken durumlarda; aksi hâlde yanlış sonuçlar üretilebilir.

## Algoritmanın Çalışma Mantığı

Örüntü (`pattern`) ile metin (`text`), aralarına metinde ve örüntüde geçmeyen bir ayraç karakteri (`$`) konularak tek bir dizide birleştirilir: `s = pattern + '$' + text`. Bu birleşik dizi için **Z dizisi** hesaplanır. `Z[i]`, `s` dizisinin `i`. konumundan başlayan alt dizisinin, `s` dizisinin başıyla (yani örüntüyle) eşleşen en uzun ortak önek uzunluğunu verir.

Z dizisinin hesaplanmasında:

1. `l` ve `r`, o ana kadar bulunmuş en sağa uzanan eşleşme aralığını (**z-box**) temsil eder; başlangıçta ikisi de `0`'dır.
2. Her `i` konumu için, eğer `i` mevcut z-box (`l..r`) içindeyse, daha önce hesaplanmış `Z[i-l]` değeri kullanılarak işe kaldığı yerden devam edilir (gereksiz tekrar karşılaştırma yapılmaz).
3. Eşleşme z-box dışına doğru genişletilebiliyorsa, karakterler tek tek karşılaştırılarak `Z[i]` artırılır.
4. Eğer bu genişleme önceki z-box'tan daha sağa ulaşırsa, `l` ve `r` güncellenir.
5. Z dizisi hesaplandıktan sonra, örüntünün uzunluğundan (`m`) sonraki (yani metin kısmına denk gelen) indekslerde `Z[i] == m` koşulunu sağlayan her konum, bir eşleşmedir.
6. Eşleşmenin metindeki gerçek başlangıç indeksi `i - m - 1` formülüyle hesaplanır (ayraç ve örüntü uzunluğu çıkarılır).
7. Bütün eşleşme konumları bulunduktan sonra sonuç listesi döndürülür.

### Örnek

Metin ve örüntü aşağıdaki gibi olsun:

```text
Metin (text)     : "aabxaabxaa"
Örüntü (pattern) : "aab"
```

Birleştirilmiş dizi:

```text
s = "aab" + "$" + "aabxaabxaa" = "aab$aabxaabxaa"
```

Z dizisinin ilgili (`m+1`'den itibaren, yani metin kısmına denk gelen) değerleri:

```text
i = 4  → Z[4] = 3 (m'e eşit) → Eşleşme! pos = 4 - 3 - 1 = 0
i = 8  → Z[8] = 3 (m'e eşit) → Eşleşme! pos = 8 - 3 - 1 = 4
```

Algoritma metinde toplam **2 eşleşme** bulur: `0 4`.

> Ayraç karakteri (`$`) sayesinde `Z[i]` değeri asla örüntünün uzunluğunu (`m`) aşamaz; bu da `Z[i] == m` kontrolünün güvenilir bir eşleşme göstergesi olmasını sağlar.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon zFunction(s):

    n = s uzunluğu
    z = n boyutunda dizi (başlangıçta sıfırlarla dolu)

    l = 0, r = 0   // mevcut z-box'ın sol ve sağ sınırları

    Döngü (i = 1'den n - 1'e kadar):

        // Eğer i mevcut z-box içindeyse
        Eğer i <= r ise:
            k = i - l

            // Daha önce hesaplanan değeri tekrar kullan
            z[i] = min(r - i + 1, z[k])

        // Gerekirse eşleşmeyi z-box dışına doğru genişlet
        Döngü (i + z[i] < n VE s[z[i]] == s[i + z[i]]):
            z[i] = z[i] + 1

        // Daha büyük bir z-box oluştuysa l ve r'yi güncelle
        Eğer i + z[i] - 1 > r ise:
            l = i
            r = i + z[i] - 1

    Döndür z


Fonksiyon search(text, pattern):

    // Örüntü ve metni ayraçla birleştir
    s = pattern + '$' + text

    // Birleşik dizi için Z dizisini hesapla
    z = zFunction(s)

    pos = boş vektör
    m = pattern uzunluğu

    // Z dizisini kontrol et (metin kısmına karşılık gelen indekslerden başlayarak)
    Döngü (i = m + 1'den z uzunluğu - 1'e kadar):

        // Z değeri örüntü uzunluğuna eşitse eşleşme vardır
        Eğer z[i] == m ise:

            // Metin içerisindeki gerçek başlangıç indeksini kaydet
            pos.ekle(i - m - 1)

    Döndür pos
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`string &s`, `string &text`, `string &pattern`** — Sırasıyla birleştirilmiş dizi, aranacak ana metin ve aranan örüntüyü referans olarak saklar; gereksiz kopyalamayı önler.

* **`vector<int> z`** — Birleştirilmiş dizinin her konumu için hesaplanan Z değerlerini tutar; boyutu `s` dizisinin uzunluğu kadardır (`vector<int> z(n)`).

* **`int l`, `int r`** — O ana kadar bulunmuş, en sağa uzanan eşleşme aralığını (**z-box**) temsil eder. `l` sol sınırı, `r` sağ sınırı tutar; bu sayede daha önce yapılan karşılaştırmaların sonucu tekrar kullanılabilir.

* **`int k = i - l`** — Mevcut `i` konumunun z-box içindeki karşılığını (z-box'ın başından itibaren kaç adım ileride olduğunu) hesaplar; `z[k]` değeri bu sayede yeniden kullanılabilir.

* **`while (i + z[i] < n && s[z[i]] == s[i + z[i]])`** — Eşleşmeyi z-box dışına doğru genişleten döngüdür; karakterler eşleştiği sürece `z[i]` artırılır.

* **`string s = pattern + '$' + text`** — Örüntü ile metni, aralarında hiçbirinde geçmeyen bir ayraç (`$`) karakteriyle birleştirir. Bu birleştirme, Z dizisi hesaplamasının hem örüntüyü hem metni tek bir yapı üzerinden işlemesini sağlar.

* **`vector<int> pos`** — Bulunan eşleşmelerin metindeki gerçek başlangıç indekslerini tutar; `push_back` ile dinamik olarak büyür.

* **`i - m - 1`** — Birleşik dizideki `i` indeksinden, eklenen örüntü (`m` karakter) ve ayraç (`1` karakter) çıkarılarak eşleşmenin metindeki gerçek başlangıç indeksi elde edilir.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<vector>`: Z dizisini ve bulunan eşleşme indekslerini saklamak için kullanılır.

> Kodda `string` türü kullanılmasına rağmen `<string>` başlığı ayrıca eklenmemiştir; bu genellikle `<iostream>` gibi başka bir standart başlığın dolaylı olarak `<string>`'i dahil etmesinden kaynaklanır. Taşınabilirlik açısından `<string>` başlığının ayrıca eklenmesi önerilir.

---

## Zaman Karmaşıklığı

Z dizisini hesaplayan döngü:

```cpp
for (int i = 1; i < n; i++)
```

birleşik dizinin uzunluğu kadar (`N + M + 1`) çalışır. İçindeki `while` döngüsü karakterleri karşılaştırırken ilerler, ancak `l` ve `r` sınırları sayesinde **her karakter en fazla bir kez ileri yönde karşılaştırılır**; z-box mantığı geriye dönük tekrar karşılaştırmayı engeller.

Bu nedenle Z dizisinin hesaplanması **amortize edilmiş (amortized)** olarak `O(N + M)` sürede tamamlanır.

`search` fonksiyonundaki son döngü:

```cpp
for (int i = m + 1; i < z.size(); i++)
```

Z dizisi üzerinde tek bir geçiş yaptığı için `O(N + M)` sürede çalışır.

Bu nedenle toplam zaman karmaşıklığı:

```text
O(N + M)
```

olur (`N`: metin uzunluğu, `M`: örüntü uzunluğu).

### Best Case — O(N + M)

Z algoritmasının en önemli özelliği, **best, average ve worst case'te aynı doğrusal karmaşıklığı** vermesidir. Z-box mekanizması sayesinde en iyi durumda da her karakter en fazla bir kez ileri yönde ziyaret edilir. Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N + M)
```

olur.

### Average Case — O(N + M)

Ortalama durumda da z-box mantığı aynı şekilde çalışır; hiçbir girdi düzeni algoritmayı doğrusal sınırın üzerine çıkaramaz. Bu nedenle ortalama durum karmaşıklığı da:

```text
O(N + M)
```

şeklindedir.

### Worst Case — O(N + M)

En kötü durumda dahi (örneğin metin ve örüntünün tamamen aynı karakterden oluştuğu `"aaaa...a"` gibi durumlarda), her karakter en fazla bir kez ileri yönde karşılaştırılır ve z-box sayesinde geriye dönük tekrar karşılaştırma yapılmaz. Bu nedenle en kötü durum zaman karmaşıklığı da:

```text
O(N + M)
```

olur. Bu, Z algoritmasını Naive (`O(N × M)`) ve en kötü durumda Rabin-Karp'a (`O(N × M)`) göre garanti edilmiş bir avantajlı konuma taşır.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N + M)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada birleştirilmiş dizi (`s = pattern + '$' + text`) ve Z dizisi (`vector<int> z`), her ikisi de `N + M + 1` boyutunda ek bellek gerektirir.

```cpp
string s = pattern + '$' + text;
vector<int> z(n);
```

`l`, `r`, `i`, `k`, `m` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Bulunan eşleşme indekslerini saklamak için kullanılan `pos` vektörü, en kötü durumda `O(N)` boyutuna ulaşabilir.

Bu nedenle algoritmanın toplam ek alan karmaşıklığı:

```text
O(N + M)
```

olur. Bu, Naive ve Rabin-Karp algoritmalarının `O(1)` ek alan kullanımına kıyasla Z algoritmasının önemli bir dezavantajıdır.
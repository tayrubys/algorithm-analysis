# KMP (Knuth-Morris-Pratt) Algoritması — String Matching

Bu bölümde String Matching (Dizi Eşleştirme) probleminin, örüntünün kendi içindeki tekrar eden önek-sonek (prefix-suffix) yapısından yararlanarak, uyumsuzluk durumunda metin işaretçisini asla geri almadan çözülmesini sağlayan **KMP (Knuth-Morris-Pratt)** yöntemi açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Naive algoritmanın bir uyumsuzluk durumunda metin işaretçisini yalnızca bir sonraki konuma kaydırıp örüntüyü baştan karşılaştırma zorunluluğunu ortadan kaldırmak için geliştirilmiştir. Örüntünün kendi içindeki tekrar eden desenler önceden analiz edilerek, uyumsuzluk anında ne kadar ileri atlanabileceği hesaplanır.
- **Hangi eksikliği gidermektedir?** Naive algoritmada metin işaretçisinin (`i`) her uyumsuzlukta geri sarılıp aynı karakterlerin tekrar tekrar karşılaştırılması sorununu giderir. KMP'de metin işaretçisi **asla geri gitmez**; yalnızca örüntü işaretçisi (`j`), önceden hesaplanan `lps` dizisi kullanılarak uygun bir konuma atlar.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı, Z algoritması gibi kesin `O(N + M)` zaman karmaşıklığı garanti etmesi, ancak bunu Z algoritmasının aksine ek bir birleştirilmiş dizi oluşturmadan, yalnızca örüntü uzunluğunda (`O(M)`) ek bellek kullanarak yapmasıdır.
- **Hangi durumda tercih edilir?**
  - Kesin olarak doğrusal zamanlı (lineer) bir çözüme ihtiyaç duyulan, ancak bellek kullanımının da önemli olduğu durumlarda.
  - Metnin tamamının belleğe alınamadığı, akış (stream) hâlinde işlendiği durumlarda (metin işaretçisinin asla geri gitmemesi bu tür kullanıma uygundur).
  - Örüntünün kendi içinde tekrar eden desenler barındırdığı durumlarda (bu durumlarda `lps` dizisi büyük atlamalar sağlayarak performansı artırır).
- **Hangi durumda tercih edilmez?**
  - Örüntünün çok kısa olduğu veya hiç tekrar eden desen içermediği (`lps` dizisinin tamamen sıfırlardan oluştuğu) durumlarda; bu gibi durumlarda KMP'nin ön işleme adımı (constructLps) ekstra bir avantaj sağlamaz.
  - Yalnızca tek bir arama yapılacaksa ve kodun basitliğinin ön işleme adımından daha öncelikli olduğu durumlarda (Naive algoritma yeterli olabilir).

## Algoritmanın Çalışma Mantığı

KMP algoritması iki aşamadan oluşur: **ön işleme (lps dizisinin oluşturulması)** ve **arama**.

### 1. Aşama — `lps` (Longest Prefix Suffix) Dizisinin Oluşturulması

`lps[i]`, örüntünün `0..i` aralığındaki alt dizisinin, kendisinden farklı olan en uzun önekinin aynı zamanda sonek olduğu uzunluğu tutar.

1. `lps[0]` her zaman `0`'dır (tek karakterin öz-eşleşen bir öneki/soneki olamaz).
2. `len` değişkeni, o ana kadar bulunan ortak önek-sonek uzunluğunu tutar.
3. `pat[i] == pat[len]` ise, `len` artırılır ve `lps[i] = len` atanır.
4. Uyumsuzluk varsa ve `len != 0` ise, `len`, bir önceki `lps[len-1]` değerine geri döndürülerek (metin başa sarılmadan) daha kısa bir önek-sonek adayı denenir.
5. Uyumsuzluk varsa ve `len == 0` ise, `lps[i] = 0` atanır ve bir sonraki karaktere geçilir.

### 2. Aşama — Arama

1. `i` metin (`txt`) işaretçisini, `j` örüntü (`pat`) işaretçisini temsil eder; ikisi de `0`'dan başlar.
2. `txt[i] == pat[j]` ise, her iki işaretçi de bir artırılır.
3. Eğer `j`, örüntü uzunluğuna (`m`) ulaştıysa, tam bir eşleşme bulunmuştur; başlangıç indeksi (`i - j`) sonuç listesine eklenir ve `j`, `lps[j-1]` değerine geri döndürülerek aramaya (üst üste binen eşleşmeleri de kaçırmadan) devam edilir.
4. Uyumsuzluk varsa ve `j != 0` ise, `j`, `lps[j-1]` değerine atlar (metin işaretçisi **geri gitmez**).
5. Uyumsuzluk varsa ve `j == 0` ise, yalnızca `i` bir artırılır.
6. Metnin sonuna ulaşılana kadar bu işlem tekrarlanır ve bulunan tüm eşleşme indeksleri döndürülür.

### Örnek

Metin ve örüntü aşağıdaki gibi olsun:

```text
Metin (txt)   : "aabaacaadaabaaba"
Örüntü (pat)  : "aaba"
```

Öncelikle örüntü için `lps` dizisi hesaplanır:

```text
pat  : a  a  b  a
lps  : 0  1  0  1
```

Arama sırasında, uyumsuzluk oluştuğunda `j` işaretçisi `lps` dizisi sayesinde metin baştan taranmadan doğru konuma atlar:

```text
Konum 0: a-a-b-a tamamen eşleşir → Eşleşme bulundu! (j, lps[3]=1'e döner, karşılaştırma kaldığı yerden sürer)
...
Konum 9: a-a-b-a tamamen eşleşir → Eşleşme bulundu!
Konum 12: a-a-b-a tamamen eşleşir → Eşleşme bulundu!
```

Algoritma metinde toplam **3 eşleşme** bulur: `0 9 12`.

> KMP'de bir uyumsuzluk oluştuğunda metin işaretçisi (`i`) hiçbir zaman geri gitmez; yalnızca örüntü işaretçisi (`j`) `lps` dizisi sayesinde uygun bir konuma atlar. Bu, algoritmanın doğrusal zaman garantisinin temelidir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon constructLps(pat, lps):

    len = 0        // önceki ortak önek-sonek uzunluğu
    lps[0] = 0      // lps[0] her zaman 0

    i = 1
    Döngü (i < pat uzunluğu):

        // Karakterler eşleşirse ortak önek-sonek uzunluğunu artır
        Eğer pat[i] == pat[len] ise:
            len = len + 1
            lps[i] = len
            i = i + 1

        // Uyumsuzluk varsa
        Değilse:
            Eğer len != 0 ise:
                // lps dizisini kullanarak bir önceki lps değerine geri dön
                len = lps[len - 1]
            Değilse:
                // Eşleşen bir önek yoksa lps[i]'yi 0 yap
                lps[i] = 0
                i = i + 1


Fonksiyon search(pat, txt):

    n = txt uzunluğu
    m = pat uzunluğu

    lps = m boyutunda dizi
    res = boş vektör

    constructLps(pat, lps)

    i = 0   // txt için indeks
    j = 0   // pat için indeks

    Döngü (i < n):

        Eğer txt[i] == pat[j] ise:
            i = i + 1
            j = j + 1

            // Tüm pattern eşleştiyse başlangıç indeksini kaydet
            Eğer j == m ise:
                res.ekle(i - j)

                // Gereksiz karşılaştırmaları önlemek için j'yi uygun lps değeriyle güncelle
                j = lps[j - 1]

        // Uyumsuzluk varsa
        Değilse:
            // Gereksiz karşılaştırmaları önlemek için önceki indeksin lps değerini kullan
            Eğer j != 0 ise:
                j = lps[j - 1]
            Değilse:
                i = i + 1

    Döndür res
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`string &pat`, `string &txt`** — Sırasıyla aranan örüntüyü ve içinde arama yapılacak ana metni referans olarak saklar; gereksiz kopyalamayı önler.

* **`vector<int> &lps`** — "Longest Prefix Suffix" (en uzun önek-sonek) dizisidir; `constructLps` fonksiyonunda hesaplanır ve `search` fonksiyonuna referans olarak aktarılır. Boyutu örüntü uzunluğu (`m`) kadardır.

* **`int len`** — `constructLps` fonksiyonu içinde, o ana kadar bulunan ortak önek-sonek uzunluğunu tutan yardımcı değişkendir.

* **`int i`, `int j`** — `search` fonksiyonunda sırasıyla metin ve örüntü üzerindeki işaretçileri temsil eder. `i` yalnızca ileri gider (asla azalmaz); `j`, `lps` dizisi sayesinde geri sıçrayabilir.

* **`vector<int> res`** — Bulunan eşleşmelerin başlangıç indekslerini tutar; `push_back` ile dinamik olarak büyür.

* **`j = lps[j - 1]`** — Algoritmanın temel optimizasyonudur. Hem tam bir eşleşme bulunduğunda hem de uyumsuzluk oluştuğunda, örüntü işaretçisinin metin baştan taranmadan doğru konuma "atlamasını" sağlar.

* **`i - j`** — Tam eşleşme anında (`j == m`), metindeki eşleşmenin başlangıç indeksini hesaplamak için kullanılır (mevcut metin konumundan örüntü uzunluğu kadar geri sayılır).

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <string>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<string>`: Metin ve örüntü verilerini `string` türünde saklamak için kullanılır.
- `<vector>`: `lps` dizisini ve bulunan eşleşme indekslerini saklamak için kullanılır.

---

## Zaman Karmaşıklığı

### Ön İşleme (`constructLps`) Karmaşıklığı

`constructLps` fonksiyonundaki `while` döngüsü, `i` her zaman ileri gittiği ve `len` her azaldığında bir önceki adaya döndüğü için, toplamda örüntü uzunluğu kadar (`M`) çalışır:

```cpp
while (i < pat.length())
```

Bu nedenle ön işleme adımı `O(M)` sürede tamamlanır.

### Arama (`search`) Karmaşıklığı

`search` fonksiyonundaki ana döngü:

```cpp
while (i < n)
```

`i` işaretçisi asla geri gitmediği için en fazla `N` kez ilerler. `j` işaretçisi geri sıçrasa da, bu sıçramaların toplam sayısı, `j`'nin daha önce ileri gittiği toplam adım sayısını aşamaz (**amortized analysis**). Bu nedenle arama adımı `O(N)` sürede tamamlanır.

Toplam zaman karmaşıklığı:

```text
O(N + M)
```

olur (`N`: metin uzunluğu, `M`: örüntü uzunluğu).

### Best Case — O(N + M)

KMP algoritmasının en önemli özelliği, **best, average ve worst case'te aynı doğrusal karmaşıklığı** vermesidir. Metin işaretçisinin asla geri gitmemesi, girdi düzeninden bağımsız olarak bu garantiyi sağlar. Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N + M)
```

olur.

### Average Case — O(N + M)

Ortalama durumda da işaretçilerin ileri-geri hareketi aynı amortize analiz mantığıyla sınırlıdır; hiçbir girdi düzeni algoritmayı doğrusal sınırın üzerine çıkaramaz. Bu nedenle ortalama durum karmaşıklığı da:

```text
O(N + M)
```

şeklindedir.

### Worst Case — O(N + M)

En kötü durumda dahi (örneğin örüntünün tamamen tek bir karakterden oluştuğu `"aaaa"` gibi durumlarda), `j` işaretçisinin geri sıçramaları toplamda `i`'nin ilerleme sayısını aşamaz. Bu nedenle en kötü durum zaman karmaşıklığı da:

```text
O(N + M)
```

olur. Bu, KMP'yi — tıpkı Z algoritması gibi — Naive (`O(N × M)`) ve en kötü durumda Rabin-Karp'a (`O(N × M)`) göre garanti edilmiş bir avantajlı konuma taşır.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(M)** *(eşleşme listesi hariç)*

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada yalnızca örüntü uzunluğunda (`m`) bir `lps` dizisi tutulur:

```cpp
vector<int> lps(m);
```

`len`, `i`, `j`, `n`, `m` gibi diğer değişkenler sabit miktarda bellek kullandığı için `O(1)` alan tüketir.

Metin (`txt`) ve örüntü (`pat`) girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bulunan eşleşme indekslerini saklamak için kullanılan `res` vektörü, en kötü durumda `O(N)` boyutuna ulaşabilir.

Bu nedenle algoritmanın kendi çalışma mantığı için gereken ek alan:

```text
O(M)
```

olur. Bu, Z algoritmasının `O(N + M)` ek alan kullanımına kıyasla KMP'nin önemli bir avantajıdır — çünkü KMP, örüntü ile metni birleştiren yeni bir dizi oluşturmaz.
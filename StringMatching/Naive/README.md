# Naive (Kaba Kuvvet) Algoritması — String Matching

Bu bölümde String Matching (Dizi Eşleştirme) probleminin, örüntüyü (pattern) metin içindeki her olası konumda tek tek deneyerek çözülmesini sağlayan **Naive** yöntem açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Bir metin içinde belirli bir örüntünün geçtiği tüm konumları bulma probleminde, en basit ve doğrudan çözümü sağlamak için geliştirilmiştir. Metnin her konumundan başlayarak örüntünün karakter karakter karşılaştırılması fikrine dayanır.
- **Hangi eksikliği gidermektedir?** Herhangi bir ön işleme veya karmaşık veri yapısı gerektirmeden, dizi eşleştirme problemine hızlıca uygulanabilecek, anlaşılması ve kodlanması kolay bir temel çözüm sunar.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı basitliğidir. Ek bellek veya ön hesaplama (KMP'deki başarısızlık fonksiyonu, Rabin-Karp'taki hash gibi) gerektirmez; doğrudan uygulanabilir.
- **Hangi durumda tercih edilir?**
  - Metin ve örüntü boyutlarının küçük olduğu durumlarda.
  - Basit ve hızlı kodlanabilir bir çözümün yeterli olduğu durumlarda.
  - Örüntünün metin içinde yalnızca bir kez veya nadiren arandığı durumlarda (ön işleme maliyetinin gereksiz olduğu durumlar).
- **Hangi durumda tercih edilmez?**
  - Metin ve örüntü boyutlarının büyük olduğu durumlarda (Örn: metin uzunluğu milyonlarca karakter). Naive algoritma en kötü durumda her konumda örüntünün tamamını karşılaştırdığı için performansı ciddi şekilde düşer ve KMP, Rabin-Karp gibi yöntemlere göre çok daha yavaş kalır.

## Algoritmanın Çalışma Mantığı

Metin baştan sona tek tek taranır. Metnin her konumu için örüntünün o konumdan başlayarak eşleşip eşleşmediği karakter karakter kontrol edilir.

Her konum için:

1. Metindeki mevcut konum belirlenir.
2. Örüntünün ilk karakterinden başlanarak metinle karşılaştırma yapılır.
3. Karakterler eşleştiği sürece karşılaştırmaya devam edilir.
4. Bir uyumsuzluk bulunursa karşılaştırma durdurulur ve metindeki bir sonraki konuma geçilir.
5. Örüntünün tüm karakterleri eşleşirse, o konum bir eşleşme olarak kaydedilir.
6. Metnin sonuna kadar bu işlem tekrarlanır.
7. Bütün konumlar denendikten sonra bulunan eşleşmelerin listesi (veya sayısı) döndürülür.

### Örnek

Metin ve örüntü aşağıdaki gibi olsun:

```text
Metin (txt)   : "aabaacaadaabaaba"
Örüntü (pat)  : "aaba"
```

Karşılaştırmaların gerçekleşmesi şu şekildedir:

```text
Konum 0: a-a-b-a tamamen eşleşir → Eşleşme bulundu!
Konum 1: a-b uyumsuzluk (b ≠ a) → durdurulur
Konum 2: b ≠ a uyumsuzluk → durdurulur
...
Konum 9: a-a-b-a tamamen eşleşir → Eşleşme bulundu!
Konum 12: a-a-b-a tamamen eşleşir → Eşleşme bulundu!
```

Algoritma metinde toplam **3 eşleşme** bulur: `0 9 12`.

> Naive algoritma bir konumda uyumsuzluk bulunca örüntü hakkında elde ettiği bilgiyi kullanmaz, bir sonraki konuma sadece bir kaydırarak geçer. Bu nedenle aynı karakterler tekrar tekrar karşılaştırılabilir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon search(pat, txt):

    m = pat uzunluğu   // pattern uzunluğu
    n = txt uzunluğu   // text uzunluğu

    // Bulunan eşleşme indekslerini saklayacak bir vektör oluştur
    result = boş vektör

    // Pattern'i text üzerinde kaydırarak tara (0'dan n-m'e kadar)
    Döngü (i = 0'dan (n - m)'e kadar):

        // Bu konumda kaç karakterin eşleştiğini kontrol etmek için j
        Döngü (j = 0'dan m - 1'e kadar):

            // Pattern ve text'teki karakterler eşleşmezse döngüden çık
            Eğer txt[j + i] != pat[j] ise:
                Döngüden çık

        // Pattern tamamen eşleştiyse (j, m'e ulaştıysa)
        Eğer j == m ise:

            // Bulunan indeksi result vektörüne ekle
            result.ekle(i)

    // Bulunan tüm eşleşme indekslerini döndür
    Döndür result
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`const string &pat`, `const string &txt`** — Sırasıyla aranan örüntüyü ve içinde arama yapılacak ana metni referans olarak saklar. `const &` kullanımı, gereksiz kopyalama yapılmasını önler ve fonksiyon içinde değiştirilmelerini engeller.

* **`int m`, `int n`** — `m`, `pat` dizisinin uzunluğunu; `n`, `txt` dizisinin uzunluğunu tutar: `int m = pat.length(); int n = txt.length();`

* **`vector<int> result`** — Bulunan eşleşmelerin başlangıç indekslerini tutar. En kötü durumda örüntü metnin her konumunda eşleşebileceği için boyutu sabit değildir, `push_back` ile dinamik olarak büyür.

* **`break`** — `txt[j+i] != pat[j]` koşulu sağlandığında (yani bir uyumsuzluk tespit edildiğinde) iç karşılaştırma döngüsünü sonlandırır; örüntünün kalan karakterleri gereksiz yere kontrol edilmez.

* **`.length()`** — Metin ve örüntünün uzunluklarını bulmak için kullanılır.

* **`push_back`** — Eşleşme bulunan (`j == m` koşulunun sağlandığı) her `i` indeksini `result` vektörünün sonuna ekler.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <string>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<string>`: Metin ve örüntü verilerini `string` türünde saklamak için kullanılır.
- `<vector>`: Bulunan eşleşme konumlarını saklamak için kullanılır.

Kodda bu kütüphaneler yerine:

```cpp
#include <bits/stdc++.h>
```

kullanılmıştır. Bu başlık birçok standart C++ kütüphanesini programa ekler. Ancak standart bir C++ başlık dosyası olmadığı için bazı derleyiciler tarafından desteklenmeyebilir.

---

## Zaman Karmaşıklığı

Metindeki her başlangıç konumunu gezen dış döngü:

```cpp
for (int i = 0; i <= n - m; i++)
```

toplam `N - M + 1` kez çalışır (`N`: metin uzunluğu, `M`: örüntü uzunluğu).

Her konumda örüntü karakterlerini karşılaştıran iç döngü:

```cpp
for (j = 0; j < m; j++) {
    if (txt[j + i] != pat[j]) {
        break;
    }
}
```

en kötü durumda `M` karaktere kadar kontrol yapabilir.

Bu nedenle toplam zaman karmaşıklığı:

```text
O((N - M + 1) × M)
```

olur. `M`, `N`'ye göre genellikle küçük kabul edildiğinde bu ifade:

```text
O(N × M)
```

şeklinde gösterilir.

### Best Case — O(N)

En iyi durumda her konumda ilk karakterde uyumsuzluk bulunur (örüntünün ilk karakteri metindeki karakterlerle hemen hemen hiç eşleşmez).

İç döngü her konum için yalnızca bir kez çalışır ve hemen sonlanır. Bu nedenle en iyi durum zaman karmaşıklığı:

```text
O(N)
```

olur.

### Average Case — O(N × M)

Ortalama durumda bir konumda eşleşmenin devam edip etmeyeceği, alfabenin büyüklüğüne ve metnin/örüntünün yapısına bağlıdır.

Genel olarak her konumda birkaç karakter karşılaştırıldıktan sonra uyumsuzluk oluşur; ancak periyodik veya tekrarlayan karakter içeren metinlerde daha fazla karşılaştırma gerekebilir. Temel Naive uygulamasının ortalama durum karmaşıklığı genel olarak:

```text
O(N × M)
```

şeklinde kabul edilir.

### Worst Case — O(N × M)

En kötü durumda her konumda örüntünün neredeyse tamamı karşılaştırılır ve son karakterde uyumsuzluk bulunur.

Örneğin metin ve örüntü:

```text
Metin  : "AAAAAAAAAAAAAAAA"
Örüntü : "AAAAB"
```

olsun.

Her konumda örüntünün ilk 4 karakteri (`A`) metinle eşleşir, yalnızca son karakterde (`B ≠ A`) uyumsuzluk oluşur. Bu durum her konum için tekrarlanır.

Toplam karşılaştırma sayısı yaklaşık olarak:

```text
(N - M + 1) × M
```

olur. Bu ifade `N × M` ile orantılı olduğu için en kötü durum zaman karmaşıklığı:

```text
O(N × M)
```

olur.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(1)** *(eşleşme listesi hariç)*

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritmada karşılaştırma işlemi için yalnızca sabit sayıda değişken (`i`, `j`) kullanılmıştır. Bu değişkenler `O(1)` alan tüketir.

```cpp
int i, j;
```

Metin (`text`) ve örüntü (`pattern`) girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.

Bulunan eşleşme konumlarını saklamak için kullanılan `matches` vektörü, en kötü durumda (örneğin metin ve örüntü tamamen aynı karakterden oluşuyorsa) `O(N)` boyutuna ulaşabilir. Bu nedenle eşleşme listesi dahil edildiğinde alan karmaşıklığı `O(N)` olarak da ifade edilebilir; ancak algoritmanın kendi çalışma mantığı (karşılaştırma süreci) için gereken ek alan:

```text
O(1)
```

olur.
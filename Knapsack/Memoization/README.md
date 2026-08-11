#  Memoization Çözümü - 0/1 Knapsack Problemi 

Bu bölümde, 0/1 Knapsack probleminin, saf özyinelemeli çözümdeki tekrar hesaplamaları önlemek için bir **memoization (hafızaya alma) tablosu** kullanılarak çözülmesini sağlayan yöntem açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** Saf özyinelemeli çözümde aynı `(n, capacity)` ikilisiyle birden fazla kez karşılaşılmasının (örtüşen alt problemler / overlapping subproblems) yol açtığı gereksiz tekrar hesaplamaları ortadan kaldırmak için geliştirilmiştir. Her alt problemin sonucu bir kez hesaplanır ve daha sonra tekrar ihtiyaç duyulduğunda doğrudan tablodan okunur.
- **Hangi eksikliği gidermektedir?** Saf özyinelemeli çözümde her eşya için iki dala ayrılan karar ağacında, aynı `(n, capacity)` durumuna farklı yollardan defalarca ulaşılabilir ve bu durum her seferinde yeniden hesaplanır. Memoization, bu durumu bir kez hesaplayıp tabloya kaydederek, aynı alt problemin tekrar tekrar çözülmesinin önüne geçer.
- **Önceki yönteme (saf özyinelemeye) göre avantajı nedir?** Fonksiyonun mantığı ve yapısı neredeyse aynı kalırken, yalnızca bir `dp` tablosu eklenerek zaman karmaşıklığı üstel (`O(2^N)`) seviyeden çok daha düşük bir seviyeye (`O(N × Kapasite)`) indirilir. Kodun okunabilirliği ve özyinelemeli yapısı korunur; sadece "bu alt problem daha önce çözüldü mü?" kontrolü eklenir.
- **Hangi durumda tercih edilir?**
  - Eşya sayısının (`n`) ve kapasitenin (`capacity`) saf özyineleme için çok büyük, ancak tam (bottom-up) dinamik programlama tablosunu baştan doldurmak yerine yalnızca gerçekten ihtiyaç duyulan alt problemlerin hesaplanmasının istendiği durumlarda.
  - Problemin özyinelemeli (top-down) yapısının korunmasının istendiği, ancak performansın da önemli olduğu durumlarda.
- **Hangi durumda tercih edilmez?**
  - Kapasite değerinin çok büyük olduğu durumlarda; çünkü `dp` tablosu `(n + 1) × (capacity + 1)` boyutunda bellek gerektirir ve bu, kapasite arttıkça hızla büyür.
  - Fonksiyon çağrı yığınının (call stack) derinliğinin sorun yaratabileceği çok büyük `n` değerlerinde; bu durumlarda döngü tabanlı (bottom-up) dinamik programlama çözümü tercih edilebilir.

## Algoritmanın Çalışma Mantığı

Fonksiyon, saf özyinelemeli çözümle aynı "al / alma" mantığını kullanır; ancak her çağrının başında ve sonunda ek iki kontrol yapılır:

1. **Taban durum:** Eşya kalmadığında (`n == 0`) veya kapasite dolduğunda (`capacity == 0`) `0` döndürülür.
2. **Tablo kontrolü:** `dp[n][capacity]` değeri `-1` değilse, bu alt problem daha önce çözülmüş demektir; sonuç doğrudan tablodan okunup döndürülür, yeniden hesaplama yapılmaz.
3. **Eşyayı almama (exclude):** Fonksiyon, bir eksik eşya ve aynı kapasiteyle kendini tekrar çağırır.
4. **Eşyayı alma (include):** Eşyanın ağırlığı kapasiteye sığıyorsa, fonksiyon bir eksik eşya ve azaltılmış kapasiteyle tekrar çağrılır; sonuca eşyanın değeri eklenir.
5. **Sonucu kaydet:** İki durumdan elde edilen değerlerin büyüğü hem `dp[n][capacity]` tablosuna yazılır hem de fonksiyonun sonucu olarak döndürülür.

Bu sayede her `(n, capacity)` ikilisi için hesaplama yalnızca **bir kez** yapılır; aynı duruma farklı bir dal üzerinden tekrar ulaşılırsa, tekrar hesaplama yapılmadan doğrudan tablodaki hazır sonuç kullanılır.

### Örnek

Aşağıdaki girdiler verilsin:

```text
weights = {1, 2, 3, 6, 7, 4}
values  = {10, 20, 25, 40, 60, 70}
capacity = 7
```

Fonksiyon `ksMemo(weights, values, 6, 7, dp)` çağrısıyla başlar. Örneğin `(n = 3, capacity = 4)` durumuna, farklı eşya seçim sıralarıyla (bazı eşyaları alıp bazılarını almayarak) birden fazla yoldan ulaşılabilir. Saf özyinelemeli çözümde bu durum her defasında yeniden hesaplanırken, memoization ile bu durum ilk karşılaşıldığında `dp[3][4]` hücresine yazılır; aynı duruma ikinci kez ulaşıldığında sonuç doğrudan bu hücreden okunur. Bütün alt problemler bu şekilde en fazla bir kez hesaplanarak `Maximum value` sonucuna ulaşılır.

> Saf özyinelemeli çözümdeki karar ağacının yapısı burada da aynıdır; fark, ağacın aynı dalına ikinci kez uğranıldığında o dalın tekrar gezilmeyip doğrudan sonucun okunmasıdır.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon KnapsackMemo(Agirliklar, Degerler, n, Kapasite, dp):

    // Taban durum: eşya kalmadıysa veya kapasite dolduysa
    Eğer n == 0 VEYA Kapasite == 0 ise:
        Döndür 0

    // Bu alt problem daha önce çözüldüyse, tablodan döndür
    Eğer dp[n][Kapasite] != -1 ise:
        Döndür dp[n][Kapasite]

    // Eşyayı almama durumu
    Disarida = KnapsackMemo(Agirliklar, Degerler, n - 1, Kapasite, dp)

    // Eşyayı alma durumu (kapasiteye sığıyorsa)
    Icerde = 0
    Eğer Agirliklar[n - 1] <= Kapasite ise:
        Icerde = Degerler[n - 1] +
                 KnapsackMemo(Agirliklar, Degerler, n - 1, Kapasite - Agirliklar[n - 1], dp)

    // Sonucu tabloya kaydet ve döndür
    dp[n][Kapasite] = Maksimum(Icerde, Disarida)
    Döndür dp[n][Kapasite]
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`int weights[]` ve `int values[]`** — Eşyaların sırasıyla ağırlıklarını ve değerlerini saklayan sabit boyutlu diziler.

* **`vector<vector<int>> dp`** — `(n + 1) × (capacity + 1)` boyutunda iki boyutlu bir vektördür. `dp[i][c]`, `i` eşya ve `c` kapasiteyle ulaşılabilecek maksimum değeri saklar. Başlangıçta bütün hücreler `-1` ile doldurulur; bu değer, ilgili alt problemin henüz çözülmediğini belirtir.

* **`dp[n][capacity] != -1` kontrolü** — Bir alt problemin daha önce çözülüp çözülmediğini anlamak için kullanılır. Bu kontrol, algoritmanın temelini oluşturan **memoization** tekniğinin ta kendisidir; aynı alt problemin tekrar hesaplanmasını engeller.

* **Özyineleme (Recursion) + Tabloya Yazma** — Fonksiyon kendi içinde saf özyinelemeli çözümdeki gibi iki kez çağrılır (`exclude` ve `include`); fark, hesaplanan sonucun fonksiyondan dönmeden önce `dp` tablosuna yazılmasıdır.

* **`&dp` (referans ile geçirme)** — `dp` tablosu fonksiyona referans (`&`) ile geçirilir. Bu sayede her özyinelemeli çağrıda tablonun bir kopyası oluşturulmaz; bütün çağrılar aynı tablo üzerinde okuma/yazma yapar.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
#include <vector>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.
- `<vector>`: İki boyutlu `dp` memoization tablosunun oluşturulması ve fonksiyonlar arasında referansla paylaşılması için kullanılır.

> **Not:** Kodda kullanılan `max()` fonksiyonu standart olarak `<algorithm>` başlık dosyasında tanımlıdır. `<vector>` başlığının dolaylı olarak bunu içermesi bazı derleyicilerde hatasız derlemeyi sağlayabilir; ancak taşınabilirlik açısından `<algorithm>` başlığının da eklenmesi önerilir.

---

## Zaman Karmaşıklığı

Toplam farklı `(n, capacity)` durumu sayısı en fazla:

```text
(N + 1) × (Kapasite + 1)
```

kadardır. Memoization sayesinde her durum **en fazla bir kez** hesaplanır; tablodan okuma işlemi `O(1)` sürer. Bu nedenle toplam zaman karmaşıklığı:

```text
O(N × Kapasite)
```

olur.

### Best / Average / Worst Case — O(N × Kapasite)

Saf özyinelemeli çözümden farklı olarak, memoization ile her `(n, capacity)` durumu en fazla bir kez hesaplandığından, algoritmanın en iyi, ortalama ve en kötü durum karmaşıklığı arasında önemli bir fark yoktur. Her koşulda karmaşıklık:

```text
O(N × Kapasite)
```

ile sınırlıdır. Bu, saf özyinelemeli çözümün üstel (`O(2^N)`) karmaşıklığına kıyasla önemli bir iyileştirmedir.

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N × Kapasite)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Memoization tablosu, her `(n, capacity)` durumunun sonucunu saklamak için `(N + 1) × (Kapasite + 1)` boyutunda bellek kullanır:

```cpp
vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, -1));
```

Bu nedenle tablonun kullandığı bellek:

```text
O(N × Kapasite)
```

olur.

Ayrıca, özyinelemeli çağrıların oluşturduğu fonksiyon çağrı yığını (call stack) en fazla `N` derinliğine ulaşabilir, bu da `O(N)` ek bellek gerektirir. Ancak `O(N × Kapasite)` terimi baskın olduğundan, algoritmanın toplam alan karmaşıklığı:

```text
O(N × Kapasite)
```

şeklinde ifade edilir.

`weights` ve `values` dizileri girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.
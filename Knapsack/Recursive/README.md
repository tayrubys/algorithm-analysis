# Recursive Algoritmasi - 0/1 Knapsack Problemi

Bu bölümde, 0/1 Knapsack probleminin, her eşya için "al" veya "alma" kararının özyineleme (recursion) ile denenmesine dayanan **saf özyinelemeli (brute-force recursive)** çözümü açıklanmaktadır.

## Algoritmanın Geliştirilme Amacı

- **Bu algoritma neden geliştirilmiştir?** 0/1 Knapsack probleminin, her eşya için "çantaya al" veya "alma" seçeneklerinin bütün kombinasyonlarını doğal ve anlaşılır bir şekilde deneyerek kesin (optimum) sonucu bulmak için geliştirilmiştir. Problemin özyinelemeli yapısını (her adımda daha küçük bir alt probleme indirgenmesini) doğrudan koda yansıtır.
- **Hangi eksikliği gidermektedir?** Bütün olası eşya kombinasyonlarını iç içe döngülerle veya bit maskeleriyle tek tek üretmenin getirdiği karmaşık ve okunması zor kodu ortadan kaldırır. Her eşya için "al/alma" kararını bir fonksiyonun kendini çağırmasıyla ifade ederek, problemin **optimal alt yapı (optimal substructure)** özelliğini açıkça gösterir.
- **Önceki yöntemlere göre avantajı nedir?** En büyük avantajı basitliği ve problemin mantığını birebir yansıtmasıdır. Her eşya için yalnızca iki karar (dahil et / etme) olduğundan kod kısa ve anlaşılırdır. Ayrıca bu çözüm, ileride **memoization** veya **dinamik programlama (DP)** ile optimize edilecek çözümlerin temelini oluşturur.
- **Hangi durumda tercih edilir?**
  - Eşya sayısının (`n`) küçük olduğu durumlarda.
  - Problemin mantığının (optimal alt yapı, örtüşen alt problemler) öğretilmesi veya anlaşılması amaçlandığında.
  - Dinamik programlama çözümüne geçmeden önce doğru bir referans (baseline) çözüm elde etmek istendiğinde.
- **Hangi durumda tercih edilmez?**
  - Eşya sayısının fazla olduğu durumlarda. Her eşya için iki dala ayrılan bir karar ağacı oluştuğundan, çözüm süresi eşya sayısıyla birlikte **katlanarak (üstel)** artar ve büyük girdilerde pratik olarak kullanılamaz hâle gelir.

## Algoritmanın Çalışma Mantığı

Fonksiyon, eşyaları sondan başa doğru (`n - 1`'den `0`'a) tek tek ele alır. Her eşya için iki olasılık değerlendirilir:

1. **Eşyayı alma (exclude):** Fonksiyon, bir eksik eşya ve aynı kapasiteyle kendini tekrar çağırır.
2. **Eşyayı alma (include):** Eğer eşyanın ağırlığı mevcut kapasiteye sığıyorsa, fonksiyon bir eksik eşya ve eşyanın ağırlığı kadar azaltılmış kapasiteyle kendini tekrar çağırır; bulunan değere mevcut eşyanın değeri eklenir.
3. Bu iki durumdan elde edilen değerlerin **büyüğü** o adımın sonucu olarak döndürülür.
4. **Taban durum (base case):** Eşya kalmadığında (`n == 0`) veya kapasite dolduğunda (`capacity == 0`) özyineleme durur ve `0` döndürülür; çünkü ne eklenecek eşya ne de kullanılabilir alan kalmıştır.

Bu mantık, her eşya için "al" ya da "alma" kararlarının oluşturduğu bir **ikili karar ağacı (binary decision tree)** üzerinde gezinme olarak düşünülebilir; fonksiyon bu ağacın bütün yapraklarını dolaşarak en yüksek değeri veren yolu bulur.

### Örnek

Aşağıdaki girdiler verilsin:

```text
weights = {1, 2, 3, 6, 7, 4}
values  = {10, 20, 25, 40, 60, 70}
capacity = 7
```

Fonksiyon en son eşyadan (`4` ağırlık, `70` değer) başlayarak geriye doğru ilerler. Her eşya için önce "alma" dalı, sonra (kapasiteye sığıyorsa) "al" dalı hesaplanır ve iki dalın sonuçları karşılaştırılır. Örneğin son eşya (ağırlık `4`, değer `70`) dahil edildiğinde kalan kapasite `3` olur ve geri kalan eşyalarla bu kapasiteye en uygun kombinasyon aranır. Bütün dallar gezildikten sonra, `capacity = 7` sınırını aşmayan en yüksek toplam değer `Maximum value` olarak döndürülür.

> Bu özyinelemeli çözüm, aynı `(n, capacity)` ikilisiyle birden fazla kez karşılaşabilir (örtüşen alt problemler). Bu tekrar hesaplamalar, algoritmanın büyük girdilerde yavaş kalmasının temel nedenidir.

---

## Sözde Kod (Pseudo-Code)

```text
Fonksiyon Knapsack(Agirliklar, Degerler, n, Kapasite):

    // Taban durum: eşya kalmadıysa veya kapasite dolduysa
    Eğer n == 0 VEYA Kapasite == 0 ise:
        Döndür 0

    // Eşyayı almama durumu
    Disarida = Knapsack(Agirliklar, Degerler, n - 1, Kapasite)

    // Eşyayı alma durumu (kapasiteye sığıyorsa)
    Icerde = 0
    Eğer Agirliklar[n - 1] <= Kapasite ise:
        Icerde = Degerler[n - 1] +
                 Knapsack(Agirliklar, Degerler, n - 1, Kapasite - Agirliklar[n - 1])

    // İki durumdan maksimum olanı döndür
    Döndür Maksimum(Icerde, Disarida)
```

---

## Kullanılan Veri Yapıları ve C++ Özellikleri

* **`int weights[]` ve `int values[]`** — Eşyaların sırasıyla ağırlıklarını ve değerlerini saklayan sabit boyutlu diziler. `weights[i]` ve `values[i]`, aynı `i` indeksindeki eşyaya ait ağırlık ve değeri temsil eder.

* **Özyineleme (Recursion)** — `ksRecur` fonksiyonu kendi içinde iki kez çağrılır: biri eşyayı hariç tutan (`exclude`), diğeri (kapasite uygunsa) eşyayı dahil eden (`include`) durum için. Bu, problemin "her eşya için al/alma kararı" yapısını doğrudan koda yansıtır.

* **Taban durum (`n == 0 || capacity == 0`)** — Özyinelemenin sonsuza gitmesini engeller; ya eşyalar tükendiğinde ya da kapasite dolduğunda özyineleme durur ve `0` değeri döndürülür.

* **`max(include, exclude)`** — Eşyayı alma ve almama durumlarından elde edilen değerlerden büyük olanını seçerek, o adım için en iyi (optimal) kararı belirler. Bu satır, algoritmanın "açgözlü" değil, **bütün olasılıkları deneyip en iyisini seçen** bir yapıya sahip olduğunu gösterir.

---

## Kullanılan Kütüphaneler

```cpp
#include <iostream>
```

- `<iostream>`: `cout` kullanılarak ekrana çıktı yazdırmak için kullanılır.

> **Not:** Kodda kullanılan `max()` fonksiyonu standart olarak `<algorithm>` başlık dosyasında tanımlıdır. Bazı derleyicilerde `<iostream>` diğer başlıkları dolaylı olarak içerdiği için kod hatasız derlenebilir; ancak taşınabilirlik açısından `<algorithm>` başlığının da programa eklenmesi önerilir.

---

## Zaman Karmaşıklığı

Her eşya için fonksiyon kendini **iki kez** çağırır (`include` ve `exclude`):

```cpp
int exclude = ksRecur(weights, values, n - 1, capacity);
...
include = values[n - 1] + ksRecur(weights, values, n - 1, capacity - weights[n - 1]);
```

Bu durum, `n` derinliğinde ve her düğümde iki dala ayrılan bir ikili karar ağacı oluşturur. Toplam düğüm (çağrı) sayısı, eşya sayısıyla birlikte katlanarak artar:

```text
O(2^N)
```

### Best / Average / Worst Case — O(2^N)

Kapasitenin erken dolması (`capacity == 0`) bazı dalların erken sonlanmasını sağlasa da, bu durum girdiye bağlı olduğundan garanti edilemez. Genel olarak algoritma hiçbir tekrar hesaplama önleme (memoization) mekanizması içermediğinden, en iyi, ortalama ve en kötü durumda da karar ağacının büyük bölümü gezilir. Bu nedenle algoritmanın zaman karmaşıklığı her koşulda üstel kabul edilir:

```text
O(2^N)
```

---

## Alan Karmaşıklığı

**Alan karmaşıklığı: O(N)**

### Ek Bellek Kullanımı ve Veri Yapılarının Etkisi

Algoritma, sonuç için ek bir dizi veya veri yapısı kullanmaz; bütün hesaplama fonksiyon çağrı yığını (call stack) üzerinden yürütülür.

Özyineleme her adımda `n` değerini bir azalttığından, çağrı yığınının en fazla derinliği eşya sayısı kadardır:

```text
n → n-1 → n-2 → ... → 0
```

Bu nedenle çağrı yığınının kullandığı ek bellek:

```text
O(N)
```

olur.

`weights` ve `values` dizileri girdinin kendisi olduğu için ek alan karmaşıklığına dahil edilmez.